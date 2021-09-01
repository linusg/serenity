/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Optional.h>
#include <AK/Result.h>
#include <AK/String.h>
#include <LibConfig/Language/AST.h>
#include <LibConfig/Language/Lexer.h>
#include <LibConfig/Language/Parser.h>
#include <LibConfig/Language/Token.h>

namespace Config {

#define try_consume(type)                          \
    do {                                           \
        auto token_or_error = consume(type);       \
        if (token_or_error.is_error())             \
            return token_or_error.release_error(); \
    } while (0)

#define try_consume_as(type, name)              \
    auto name##_or_error = consume(type);       \
    if (name##_or_error.is_error())             \
        return name##_or_error.release_error(); \
    auto name = name##_or_error.release_value()

#define try_consume_if_not_done(type)                  \
    do {                                               \
        if (!done()) {                                 \
            auto token_or_error = consume(type);       \
            if (token_or_error.is_error())             \
                return token_or_error.release_error(); \
        }                                              \
    } while (0)

#define try_parse(name)                         \
    auto name##_or_error = parse_##name();      \
    if (name##_or_error.is_error())             \
        return name##_or_error.release_error(); \
    auto name = name##_or_error.release_value()

Parser::Parser(Lexer& lexer)
    : m_lexer(lexer)
    , m_current_token(lexer.next())
{
}

Result<NonnullRefPtr<AST>, Parser::Error> Parser::parse()
{
    auto ast = create<AST>();
    m_ast = ast;

    while (!done()) {
        if (match(Token::Type::Newline)) {
            consume();
            continue;
        }

        if (match_comment()) {
            try_parse(comment);
            continue;
        }

        if (match_section()) {
            try_parse(section);
            m_ast->ensure_section({}, section);
            continue;
        }

        if (match_entry()) {
            try_parse(entry);
            continue;
        }

        return error(String::formatted("Unexpected token {}", consume().name()));
    }

    m_ast = nullptr;
    return ast;
}

bool Parser::done() const
{
    return match(Token::Type::Eof);
}

bool Parser::match(Token::Type type) const
{
    return m_current_token.type() == type;
}

bool Parser::match_comment() const
{
    return match(Token::Type::Comment);
}

bool Parser::match_section() const
{
    return match(Token::Type::LeftBracket);
}

bool Parser::match_entry() const
{
    return match(Token::Type::Identifier);
}

Token Parser::consume()
{
    auto old_token = m_current_token;
    m_current_token = m_lexer.next();
    return old_token;
}

Result<Token, Parser::Error> Parser::consume(Token::Type type)
{
    if (!match(type))
        return error(String::formatted("Unexpected token {}, expected {}", consume().name(), Token::name(type)));
    return consume();
}

Result<String, Parser::Error> Parser::parse_comment()
{
    try_consume_as(Token::Type::Comment, comment);
    try_consume_if_not_done(Token::Type::Newline);
    return String { comment.value() };
}

Result<String, Parser::Error> Parser::parse_section()
{
    try_consume(Token::Type::LeftBracket);
    try_consume_as(Token::Type::Identifier, identifier);
    try_consume(Token::Type::RightBracket);
    try_consume_if_not_done(Token::Type::Newline);
    auto section_name = identifier.value();
    if (m_ast->sections().contains(section_name))
        return error(String::formatted("Duplicate section '{}'", section_name), identifier);
    return String { section_name };
}

Result<NonnullRefPtr<Entry>, Parser::Error> Parser::parse_entry()
{
    try_consume_as(Token::Type::Identifier, identifier);
    try_consume(Token::Type::Colon);
    try_parse(type);
    try_consume(Token::Type::Equals);
    try_parse(value);
    try_consume_if_not_done(Token::Type::Newline);
    return create<Entry>(identifier.value(), move(value), type, Vector<String> {});
}

Result<TypeVariant, Parser::Error> Parser::parse_type()
{
#define return_type_or_list_of_type(T, ...)                           \
    do {                                                              \
        if (is_list)                                                  \
            return TypeVariant { Type::List<T> { { __VA_ARGS__ } } }; \
        else                                                          \
            return TypeVariant { T { __VA_ARGS__ } };                 \
    } while (0)

    bool is_list = false;

    try_consume_as(Token::Type::Identifier, identifier);
    if (match(Token::Type::LeftBracket)) {
        try_consume(Token::Type::LeftBracket);
        try_consume(Token::Type::RightBracket);
        is_list = true;
    }

    if (identifier.value() == "enum"sv) {
        try_parse(enum_members);
        return_type_or_list_of_type(Type::Enum, move(enum_members));
    }

    if (identifier.value() == "i32"sv) {
        auto range = parse_range<i32>();
        if (range.is_error())
            return range.release_error();
        return_type_or_list_of_type(Type::Number<i32>, range.release_value());
    }

    if (identifier.value() == "string"sv)
        return_type_or_list_of_type(Type::String);

    if (identifier.value() == "path"sv)
        return_type_or_list_of_type(Type::Path);

    return error("Invalid type", identifier);
}

Result<Vector<String>, Parser::Error> Parser::parse_enum_members()
{
    Vector<String> members;
    try_consume(Token::Type::LeftParen);
    do {
        if (!members.is_empty())
            try_consume(Token::Type::Comma);
        try_consume_as(Token::Type::Identifier, member);
        members.append(member.value());
    } while (match(Token::Type::Comma));
    try_consume(Token::Type::RightParen);
    return members;
}

template<typename T>
Result<Range<T>, Parser::Error> Parser::parse_range()
{
    try_consume(Token::Type::LeftParen);
    try_consume_as(Token::Type::Number, min_number);
    try_consume(Token::Type::PeriodPeriod);
    try_consume_as(Token::Type::Number, max_number);
    try_consume(Token::Type::RightParen);
    auto min = min_number.value().to_int<T>();
    if (!min.has_value())
        return error(String::formatted("Invalid minimum value {} for type"), min_number);
    auto max = max_number.value().to_int<T>();
    if (!max.has_value())
        return error(String::formatted("Invalid maximum value {} for type"), max_number);
    return Range<T> { *min, *max };
}

Result<String, Parser::Error> Parser::parse_value()
{
    if (match(Token::Type::String)) {
        auto token = consume();
        return String { token.value().substring_view(1, token.value().length() - 2) };
    }
    if (match(Token::Type::Number) || match(Token::Type::Identifier))
        return String { consume().value() };
    return error(String::formatted("Unexpected token {}, expected String, Number, or Identifier", consume().name()));
}

};
