/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Forward.h>
#include <AK/Result.h>
#include <AK/String.h>
#include <LibConfig/Language/AST.h>
#include <LibConfig/Language/Token.h>

namespace Config {

class Parser {
public:
    struct Error {
        String message;
        Token token;
    };

    explicit Parser(Lexer&);

    Result<NonnullRefPtr<AST>, Error> parse();

private:
    Error error(String message, Optional<Token> token = {})
    {
        return Error { move(message), token.value_or(m_current_token) };
    }

    bool done() const;

    Token consume();
    Result<Token, Error> consume(Token::Type);

    bool match(Token::Type) const;
    bool match_comment() const;
    bool match_section() const;
    bool match_entry() const;

    Result<String, Error> parse_comment();
    Result<String, Error> parse_section();
    Result<NonnullRefPtr<Entry>, Error> parse_entry();
    Result<TypeVariant, Error> parse_type();
    Result<Vector<String>, Error> parse_enum_members();
    template<typename T>
    Result<Range<T>, Error> parse_range();
    Result<String, Error> parse_value();

    Lexer& m_lexer;
    Token m_current_token;
    RefPtr<AST> m_ast;
};

};
