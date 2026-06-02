#pragma once

#include <map>
#include <string>
#include <vector>

class JsonValue {
public:
    enum class Type {
        Null,
        Bool,
        Number,
        String,
        Array,
        Object
    };

    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;

    JsonValue() = default;
    explicit JsonValue(bool value);
    explicit JsonValue(double value);
    explicit JsonValue(std::string value);
    explicit JsonValue(Array value);
    explicit JsonValue(Object value);

    Type GetType() const { return m_type; }
    bool IsNull() const { return m_type == Type::Null; }
    bool IsBool() const { return m_type == Type::Bool; }
    bool IsNumber() const { return m_type == Type::Number; }
    bool IsString() const { return m_type == Type::String; }
    bool IsArray() const { return m_type == Type::Array; }
    bool IsObject() const { return m_type == Type::Object; }

    bool AsBool(bool fallback = false) const;
    double AsNumber(double fallback = 0.0) const;
    const std::string& AsString() const;
    std::string AsStringOr(const std::string& fallback) const;
    const Array& AsArray() const;
    const Object& AsObject() const;

    bool Contains(const std::string& key) const;
    const JsonValue* Find(const std::string& key) const;
    const JsonValue& At(const std::string& key) const;

    static const JsonValue& NullValue();

private:
    Type m_type = Type::Null;
    bool m_bool = false;
    double m_number = 0.0;
    std::string m_string;
    Array m_array;
    Object m_object;
};

class SimpleJson {
public:
    static JsonValue Parse(const std::string& text);
    static JsonValue ParseFile(const std::string& filePath);

    // Serialize a JsonValue to a formatted JSON string
    static std::string Stringify(const JsonValue& value, int indent = 0);

    // Write a JsonValue to a file (creates parent dirs if needed)
    static bool WriteFile(const std::string& filePath, const JsonValue& value);
};
