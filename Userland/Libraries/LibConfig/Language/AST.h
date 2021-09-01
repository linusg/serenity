/*
 * Copyright (c) 2021, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Forward.h>
#include <AK/HashMap.h>
#include <AK/NonnullRefPtrVector.h>
#include <AK/RefCounted.h>
#include <AK/String.h>
#include <AK/Variant.h>
#include <LibConfig/Forward.h>

namespace Config {

template<typename T>
requires IsIntegral<T>
struct Range {
    T min;
    T max;
};

struct Type {
    template<typename T>
    requires IsIntegral<T>
    struct Number {
        Range<T> range;
    };

    struct Enum {
        Vector<String> members;
    };

    struct String {
    };

    struct Path {
    };

    template<typename T>
    struct List {
        T type;
    };
};

using TypeVariant = Variant<
    Type::Number<i32>,
    Type::List<Type::Number<i32>>,
    Type::Enum,
    Type::List<Type::Enum>,
    Type::String,
    Type::List<Type::String>,
    Type::Path,
    Type::List<Type::Path>>;

class Entry : public RefCounted<Entry> {
public:
    Entry(String key, String value, TypeVariant type, Vector<String> comments)
        : m_key(move(key))
        , m_value(move(value))
        , m_type(move(type))
        , m_comments(move(comments))
    {
    }
    virtual ~Entry() = default;

    String const& key() const { return m_key; }
    String const& value() const { return m_value; }
    TypeVariant const& type() const { return m_type; }
    Vector<String> const& comments() const { return m_comments; }

private:
    String m_key;
    String m_value;
    TypeVariant m_type;
    Vector<String> m_comments;
};

class AST : public RefCounted<AST> {
public:
    using Sections = OrderedHashMap<String, NonnullRefPtrVector<Entry>>;

    AST() = default;
    virtual ~AST() = default;

    Sections const& sections() const { return m_sections; }
    void dump() const;

    void ensure_section(Badge<Parser>, String const&);

private:
    Sections m_sections;
};

};
