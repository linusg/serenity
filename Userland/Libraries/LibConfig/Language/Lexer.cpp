/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/CharacterTypes.h>
#include <LibConfig/Language/Lexer.h>

namespace Config {

Lexer::Lexer(StringView input)
    : GenericLexer(input)
{
}

Token Lexer::next()
{
    auto consume = [this](size_t count = 1) {
        auto sv = this->consume(count);
        if (sv.contains('\n')) {
            m_line++;
            m_column = 1;
        } else {
            m_column++;
        }
        return sv;
    };

    while (next_is(' ') || next_is('\t'))
        consume();

    auto start_position = SourcePosition { m_line, m_column, m_index };

    auto emit_token = [&](auto type) -> Token {
        auto end_position = SourcePosition { m_line, m_column - 1, m_index };
        auto value = m_input.substring_view(start_position.offset, m_index - start_position.offset);
        return { type, start_position, end_position, value };
    };

    if (is_eof())
        return emit_token(Token::Type::Eof);

    switch (peek()) {
    case ',':
        consume();
        return emit_token(Token::Type::Comma);
    case ':':
        consume();
        return emit_token(Token::Type::Colon);
    case '=':
        consume();
        return emit_token(Token::Type::Equals);
    case '[':
        consume();
        return emit_token(Token::Type::LeftBracket);
    case '(':
        consume();
        return emit_token(Token::Type::LeftParen);
    case '\n':
        consume();
        return emit_token(Token::Type::Newline);
    case ']':
        consume();
        return emit_token(Token::Type::RightBracket);
    case ')':
        consume();
        return emit_token(Token::Type::RightParen);
    }

    if (next_is(".."sv)) {
        consume(2);
        return emit_token(Token::Type::PeriodPeriod);
    }

    if (next_is('#')) {
        consume();
        while (!next_is('\n') && !is_eof())
            consume();
        return emit_token(Token::Type::Comment);
    }

    if (next_is('"')) {
        consume();
        while (!next_is('\n') && !is_eof()) {
            if (next_is(R"(\")"sv)) {
                consume(2);
                continue;
            }
            if (consume() == R"(")"sv)
                return emit_token(Token::Type::String);
        }
        return emit_token(Token::Type::UnterminatedString);
    }

    if (next_is('_') || next_is(is_ascii_alpha)) {
        consume();
        while (next_is('_') || next_is(is_ascii_alphanumeric))
            consume();
        return emit_token(Token::Type::Identifier);
    }

    if (next_is(is_ascii_alphanumeric)) {
        // TODO: decimal points, _ digit separator
        while (next_is(is_ascii_alphanumeric))
            consume();
        return emit_token(Token::Type::Number);
    }

    consume();
    return emit_token(Token::Type::Invalid);
}

};
