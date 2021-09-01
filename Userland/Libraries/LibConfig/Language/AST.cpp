/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Badge.h>
#include <LibConfig/Language/AST.h>
#include <LibConfig/Language/Parser.h>

namespace Config {

void AST::ensure_section(Badge<Parser>, String const& name)
{
    m_sections.ensure(name);
}

void AST::dump() const
{
    for (auto& section : m_sections) {
        outln("Entry {{");
        outln("    key = \"{}\",", entry->key());
        out("    type = ");
        entry->type().visit(
            [](Type::String const&) {
                out("Type::String {{}}");
            },
            [](Type::Path const&) {
                out("Type::Path {{}}");
            },
            [](Type::Enum const& enum_) {
                outln("Type::Enum {{");
                outln("        members = {{", String::join(", "sv, enum_.members));
                outln("            {}", String::join(",\n            "sv, enum_.members));
                outln("        }}", String::join(", "sv, enum_.members));
                out("    }}");
            },
            [](Type::Number<i32> const& number) {
                outln("Type::Number<i32> {{");
                outln("        range = Range<i32> {{");
                outln("            min = {},", number.range.min);
                outln("            max = {}", number.range.max);
                outln("        }}");
                out("    }}");
            },
            [](Type::List<Type::String> const&) {
                out("Type::List<Type::String> {{}}");
            },
            [](Type::List<Type::Path> const&) {
                out("Type::List<Type::Path> {{}}");
            },
            [](Type::List<Type::Enum> const&) {
                out("Type::List<Type::Enum> {{}}");
            },
            [](Type::List<Type::Number<i32>> const& number_list) {
                outln("Type::List<Type::Number<i32>> {{");
                outln("        range = Range<i32> {{");
                outln("            min = {},", number_list.type.range.min);
                outln("            max = {}", number_list.type.range.max);
                outln("        }}");
                out("    }}");
            });
        outln(",");
        outln("    value = \"{}\"", entry->value());
        outln("}}");
    }
}

};
