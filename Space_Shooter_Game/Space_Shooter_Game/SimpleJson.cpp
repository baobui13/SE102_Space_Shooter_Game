#include "SimpleJson.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
class JsonParser {
public:
    explicit JsonParser(const std::string& text)
        : m_text(text) {}

    JsonValue Parse() {
        SkipWhitespace();
        JsonValue value = ParseValue();
        SkipWhitespace();
        if (!IsAtEnd()) {
            throw std::runtime_error("Unexpected characters after JSON document.");
        }
        return value;
    }

private:
    const std::string& m_text;
    size_t m_pos = 0;

    bool IsAtEnd() const {
        return m_pos >= m_text.size();
    }

    char Peek() const {
        return IsAtEnd() ? '\0' : m_text[m_pos];
    }

    char Advance() {
        return IsAtEnd() ? '\0' : m_text[m_pos++];
    }

    void SkipWhitespace() {
        while (!IsAtEnd() && std::isspace(static_cast<unsigned char>(Peek()))) {
            ++m_pos;
        }
    }

    bool Match(char expected) {
        if (Peek() != expected) {
            return false;
        }
        ++m_pos;
        return true;
    }

    void Expect(char expected) {
        if (!Match(expected)) {
            throw std::runtime_error("Invalid JSON syntax.");
        }
    }

    JsonValue ParseValue() {
        SkipWhitespace();
        const char c = Peek();
        if (c == '"') return JsonValue(ParseString());
        if (c == '{') return JsonValue(ParseObject());
        if (c == '[') return JsonValue(ParseArray());
        if (c == 't') return ParseLiteral("true", JsonValue(true));
        if (c == 'f') return ParseLiteral("false", JsonValue(false));
        if (c == 'n') return ParseLiteral("null", JsonValue());
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
            return JsonValue(ParseNumber());
        }
        throw std::runtime_error("Invalid JSON value.");
    }

    JsonValue ParseLiteral(const char* literal, JsonValue value) {
        const std::string expected(literal);
        if (m_text.compare(m_pos, expected.size(), expected) != 0) {
            throw std::runtime_error("Invalid JSON literal.");
        }
        m_pos += expected.size();
        return value;
    }

    std::string ParseString() {
        Expect('"');
        std::string result;
        while (!IsAtEnd()) {
            const char c = Advance();
            if (c == '"') {
                return result;
            }
            if (c != '\\') {
                result.push_back(c);
                continue;
            }

            if (IsAtEnd()) {
                throw std::runtime_error("Unterminated JSON escape sequence.");
            }

            const char escaped = Advance();
            switch (escaped) {
            case '"': result.push_back('"'); break;
            case '\\': result.push_back('\\'); break;
            case '/': result.push_back('/'); break;
            case 'b': result.push_back('\b'); break;
            case 'f': result.push_back('\f'); break;
            case 'n': result.push_back('\n'); break;
            case 'r': result.push_back('\r'); break;
            case 't': result.push_back('\t'); break;
            case 'u':
                // Config files in this project use ASCII keys/paths. Skip unicode escapes safely.
                for (int i = 0; i < 4 && !IsAtEnd(); ++i) {
                    Advance();
                }
                result.push_back('?');
                break;
            default:
                throw std::runtime_error("Invalid JSON escape sequence.");
            }
        }
        throw std::runtime_error("Unterminated JSON string.");
    }

    double ParseNumber() {
        const size_t start = m_pos;
        if (Peek() == '-') Advance();
        while (std::isdigit(static_cast<unsigned char>(Peek()))) Advance();
        if (Peek() == '.') {
            Advance();
            while (std::isdigit(static_cast<unsigned char>(Peek()))) Advance();
        }
        if (Peek() == 'e' || Peek() == 'E') {
            Advance();
            if (Peek() == '+' || Peek() == '-') Advance();
            while (std::isdigit(static_cast<unsigned char>(Peek()))) Advance();
        }
        return std::stod(m_text.substr(start, m_pos - start));
    }

    JsonValue::Array ParseArray() {
        Expect('[');
        JsonValue::Array result;
        SkipWhitespace();
        if (Match(']')) {
            return result;
        }

        while (true) {
            result.push_back(ParseValue());
            SkipWhitespace();
            if (Match(']')) {
                return result;
            }
            Expect(',');
        }
    }

    JsonValue::Object ParseObject() {
        Expect('{');
        JsonValue::Object result;
        SkipWhitespace();
        if (Match('}')) {
            return result;
        }

        while (true) {
            SkipWhitespace();
            if (Peek() != '"') {
                throw std::runtime_error("Expected JSON object key.");
            }
            std::string key = ParseString();
            SkipWhitespace();
            Expect(':');
            result.emplace(std::move(key), ParseValue());
            SkipWhitespace();
            if (Match('}')) {
                return result;
            }
            Expect(',');
        }
    }
};
}

JsonValue::JsonValue(bool value)
    : m_type(Type::Bool), m_bool(value) {}

JsonValue::JsonValue(double value)
    : m_type(Type::Number), m_number(value) {}

JsonValue::JsonValue(std::string value)
    : m_type(Type::String), m_string(std::move(value)) {}

JsonValue::JsonValue(Array value)
    : m_type(Type::Array), m_array(std::move(value)) {}

JsonValue::JsonValue(Object value)
    : m_type(Type::Object), m_object(std::move(value)) {}

bool JsonValue::AsBool(bool fallback) const {
    return IsBool() ? m_bool : fallback;
}

double JsonValue::AsNumber(double fallback) const {
    return IsNumber() ? m_number : fallback;
}

const std::string& JsonValue::AsString() const {
    if (!IsString()) {
        return NullValue().m_string;
    }
    return m_string;
}

std::string JsonValue::AsStringOr(const std::string& fallback) const {
    return IsString() ? m_string : fallback;
}

const JsonValue::Array& JsonValue::AsArray() const {
    if (!IsArray()) {
        return NullValue().m_array;
    }
    return m_array;
}

const JsonValue::Object& JsonValue::AsObject() const {
    if (!IsObject()) {
        return NullValue().m_object;
    }
    return m_object;
}

bool JsonValue::Contains(const std::string& key) const {
    return Find(key) != nullptr;
}

const JsonValue* JsonValue::Find(const std::string& key) const {
    if (!IsObject()) {
        return nullptr;
    }
    auto it = m_object.find(key);
    return it == m_object.end() ? nullptr : &it->second;
}

const JsonValue& JsonValue::At(const std::string& key) const {
    const JsonValue* value = Find(key);
    return value ? *value : NullValue();
}

const JsonValue& JsonValue::NullValue() {
    static const JsonValue nullValue;
    return nullValue;
}

JsonValue SimpleJson::Parse(const std::string& text) {
    return JsonParser(text).Parse();
}

JsonValue SimpleJson::ParseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("Cannot open JSON file: " + filePath);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return Parse(buffer.str());
}

std::string SimpleJson::Stringify(const JsonValue& value, int indent) {
    const std::string INDENT(indent * 2, ' ');
    const std::string INNER((indent + 1) * 2, ' ');

    switch (value.GetType()) {
    case JsonValue::Type::Null:
        return "null";
    case JsonValue::Type::Bool:
        return value.AsBool() ? "true" : "false";
    case JsonValue::Type::Number: {
        double n = value.AsNumber();
        // Use integer repr when it is a whole number
        if (n == static_cast<long long>(n))
            return std::to_string(static_cast<long long>(n));
        std::ostringstream oss;
        oss << n;
        return oss.str();
    }
    case JsonValue::Type::String: {
        // Basic escaping for common characters
        std::string result = "\"";
        for (char c : value.AsString()) {
            switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result.push_back(c);
            }
        }
        result += '"';
        return result;
    }
    case JsonValue::Type::Array: {
        const auto& arr = value.AsArray();
        if (arr.empty()) return "[]";
        std::string result = "[\n";
        for (size_t i = 0; i < arr.size(); ++i) {
            result += INNER + Stringify(arr[i], indent + 1);
            if (i + 1 < arr.size()) result += ",";
            result += "\n";
        }
        result += INDENT + "]";
        return result;
    }
    case JsonValue::Type::Object: {
        const auto& obj = value.AsObject();
        if (obj.empty()) return "{}";
        std::string result = "{\n";
        size_t i = 0;
        for (const auto& kv : obj) {
            result += INNER + "\"" + kv.first + "\": " + Stringify(kv.second, indent + 1);
            if (i + 1 < obj.size()) result += ",";
            result += "\n";
            ++i;
        }
        result += INDENT + "}";
        return result;
    }
    }
    return "null";
}

bool SimpleJson::WriteFile(const std::string& filePath, const JsonValue& value) {
    // Create parent directories if they don't exist
    std::filesystem::path p(filePath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
    std::ofstream file(filePath);
    if (!file) {
        return false;
    }
    file << Stringify(value);
    return file.good();
}
