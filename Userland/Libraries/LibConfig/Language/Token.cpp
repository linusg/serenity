/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibConfig/Language/Token.h>

namespace Config {

StringView Token::name(Type type)
{
    switch (type) {
#define __ENUMERATE_CONFIG_TOKEN(type) \
    case Type::type:                   \
        return #type##sv;
        ENUMERATE_CONFIG_TOKENS
#undef __ENUMERATE_CONFIG_TOKEN
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView Token::name() const
{
    return name(m_type);
}

}
