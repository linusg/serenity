/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/StringView.h>

namespace Config {

#define ENUMERATE_CONFIG_TOKENS            \
    __ENUMERATE_CONFIG_TOKEN(Invalid)      \
    __ENUMERATE_CONFIG_TOKEN(Colon)        \
    __ENUMERATE_CONFIG_TOKEN(Comma)        \
    __ENUMERATE_CONFIG_TOKEN(Comment)      \
    __ENUMERATE_CONFIG_TOKEN(Eof)          \
    __ENUMERATE_CONFIG_TOKEN(Equals)       \
    __ENUMERATE_CONFIG_TOKEN(Identifier)   \
    __ENUMERATE_CONFIG_TOKEN(LeftBracket)  \
    __ENUMERATE_CONFIG_TOKEN(LeftParen)    \
    __ENUMERATE_CONFIG_TOKEN(Newline)      \
    __ENUMERATE_CONFIG_TOKEN(Number)       \
    __ENUMERATE_CONFIG_TOKEN(PeriodPeriod) \
    __ENUMERATE_CONFIG_TOKEN(RightBracket) \
    __ENUMERATE_CONFIG_TOKEN(RightParen)   \
    __ENUMERATE_CONFIG_TOKEN(String)       \
    __ENUMERATE_CONFIG_TOKEN(UnterminatedString)

struct SourcePosition {
    size_t line { 0 };
    size_t column { 0 };
    size_t offset { 0 };
};

class Token {
public:
    enum class Type {
#define __ENUMERATE_CONFIG_TOKEN(x) x,
        ENUMERATE_CONFIG_TOKENS
#undef __ENUMERATE_CONFIG_TOKEN
    };

    Token(Type type, SourcePosition start_position, SourcePosition end_position, StringView value)
        : m_type(type)
        , m_start_position(start_position)
        , m_end_position(end_position)
        , m_value(value)
    {
    }

    [[nodiscard]] Type type() const { return m_type; }
    [[nodiscard]] StringView value() const { return m_value; }
    [[nodiscard]] SourcePosition start_position() const { return m_start_position; }
    [[nodiscard]] SourcePosition end_position() const { return m_end_position; }

    [[nodiscard]] static StringView name(Type);
    [[nodiscard]] StringView name() const;

private:
    Type m_type;
    SourcePosition m_start_position;
    SourcePosition m_end_position;
    StringView m_value;
};

}
