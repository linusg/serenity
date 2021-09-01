/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/GenericLexer.h>
#include <LibConfig/Language/Token.h>

namespace Config {

class Lexer : private GenericLexer {
public:
    explicit Lexer(StringView);

    [[nodiscard]] StringView source() const { return m_input; }
    [[nodiscard]] Token next();

private:
    size_t m_line { 1 };
    size_t m_column { 1 };
};

};
