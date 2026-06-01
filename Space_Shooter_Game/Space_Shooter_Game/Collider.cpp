#include "Collider.h"
#include "ConfigUtils.h"
#include "SimpleJson.h"

#include <algorithm>
#include <cmath>

namespace {
constexpr const char* DEFAULT_COLLIDER_CONFIG = "config/colliders/colliders.json";

float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

bool ReadBool(const JsonValue& object, const std::string& key, bool fallback) {
    const JsonValue* value = object.Find(key);
    return value ? value->AsBool(fallback) : fallback;
}

ColliderShape ShapeFromString(const std::string& value) {
    if (value == "circle" || value == "Circle") {
        return ColliderShape::Circle;
    }
    return ColliderShape::Rectangle;
}

ColliderDefinition ParseDefinition(const JsonValue& value) {
    ColliderDefinition definition;
    definition.name = value.At("name").AsStringOr("default");
    definition.shape = ShapeFromString(value.At("shape").AsStringOr("rectangle"));
    definition.useOwnerSize = ReadBool(value, "useOwnerSize", true);
    definition.width = ReadFloat(value, "width", 0.0f);
    definition.height = ReadFloat(value, "height", 0.0f);
    definition.radius = ReadFloat(value, "radius", 0.0f);
    definition.offsetX = ReadFloat(value, "offsetX", 0.0f);
    definition.offsetY = ReadFloat(value, "offsetY", 0.0f);
    return definition;
}

float DistanceSquared(float ax, float ay, float bx, float by) {
    const float dx = ax - bx;
    const float dy = ay - by;
    return dx * dx + dy * dy;
}

bool RectIntersectsRect(const Collider& a, const Collider& b) {
    return a.Left() < b.Right() &&
        a.Right() > b.Left() &&
        a.Top() < b.Bottom() &&
        a.Bottom() > b.Top();
}

bool CircleIntersectsCircle(const Collider& a, const Collider& b) {
    const float radius = a.radius + b.radius;
    return DistanceSquared(a.CenterX(), a.CenterY(), b.CenterX(), b.CenterY()) <= radius * radius;
}

bool RectIntersectsCircle(const Collider& rect, const Collider& circle) {
    const float nearestX = std::clamp(circle.CenterX(), rect.Left(), rect.Right());
    const float nearestY = std::clamp(circle.CenterY(), rect.Top(), rect.Bottom());
    return DistanceSquared(circle.CenterX(), circle.CenterY(), nearestX, nearestY) <= circle.radius * circle.radius;
}
}

Collider Collider::Rectangle(const std::string& name, float x, float y, float width, float height) {
    Collider collider;
    collider.name = name;
    collider.shape = ColliderShape::Rectangle;
    collider.x = x;
    collider.y = y;
    collider.width = width;
    collider.height = height;
    collider.radius = 0.0f;
    return collider;
}

Collider Collider::Circle(const std::string& name, float centerX, float centerY, float radius) {
    Collider collider;
    collider.name = name;
    collider.shape = ColliderShape::Circle;
    collider.x = centerX;
    collider.y = centerY;
    collider.width = radius * 2.0f;
    collider.height = radius * 2.0f;
    collider.radius = radius;
    return collider;
}

float Collider::Left() const {
    return shape == ColliderShape::Circle ? x - radius : x;
}

float Collider::Right() const {
    return shape == ColliderShape::Circle ? x + radius : x + width;
}

float Collider::Top() const {
    return shape == ColliderShape::Circle ? y - radius : y;
}

float Collider::Bottom() const {
    return shape == ColliderShape::Circle ? y + radius : y + height;
}

float Collider::CenterX() const {
    return shape == ColliderShape::Circle ? x : x + width * 0.5f;
}

float Collider::CenterY() const {
    return shape == ColliderShape::Circle ? y : y + height * 0.5f;
}

bool Collider::Intersects(const Collider& other) const {
    if (shape == ColliderShape::Rectangle && other.shape == ColliderShape::Rectangle) {
        return RectIntersectsRect(*this, other);
    }

    if (shape == ColliderShape::Circle && other.shape == ColliderShape::Circle) {
        return CircleIntersectsCircle(*this, other);
    }

    if (shape == ColliderShape::Rectangle) {
        return RectIntersectsCircle(*this, other);
    }

    return RectIntersectsCircle(other, *this);
}

ColliderRegistry& ColliderRegistry::GetInstance() {
    static ColliderRegistry instance;
    return instance;
}

ColliderRegistry::ColliderRegistry() {
    RegisterDefaults();
}

bool ColliderRegistry::LoadFromFile(const std::string& filePath) {
    try {
        const JsonValue root = SimpleJson::ParseFile(ResolveConfigPath(filePath));
        const JsonValue& colliders = root.Contains("colliders") ? root.At("colliders") : root;

        for (const auto& value : colliders.AsArray()) {
            RegisterDefinition(ParseDefinition(value));
        }

        m_loaded = true;
        return true;
    }
    catch (...) {
        m_loaded = true;
        return false;
    }
}

const ColliderDefinition& ColliderRegistry::GetDefinition(const std::string& name) const {
    EnsureLoaded();
    auto it = m_definitions.find(name);
    if (it != m_definitions.end()) {
        return it->second;
    }
    return m_definitions.at("default");
}

Collider ColliderRegistry::CreateCollider(const std::string& name,
                                          float ownerX,
                                          float ownerY,
                                          float ownerWidth,
                                          float ownerHeight) const {
    const ColliderDefinition& definition = GetDefinition(name);

    if (definition.shape == ColliderShape::Circle) {
        const float radius = definition.radius > 0.0f
            ? definition.radius
            : std::min(ownerWidth, ownerHeight) * 0.5f;
        const float centerX = ownerX + ownerWidth * 0.5f + definition.offsetX;
        const float centerY = ownerY + ownerHeight * 0.5f + definition.offsetY;
        return Collider::Circle(definition.name, centerX, centerY, radius);
    }

    const float width = (definition.useOwnerSize || definition.width <= 0.0f)
        ? ownerWidth
        : definition.width;
    const float height = (definition.useOwnerSize || definition.height <= 0.0f)
        ? ownerHeight
        : definition.height;
    const float x = ownerX + (ownerWidth - width) * 0.5f + definition.offsetX;
    const float y = ownerY + (ownerHeight - height) * 0.5f + definition.offsetY;
    return Collider::Rectangle(definition.name, x, y, width, height);
}

Collider ColliderRegistry::CreateRectangleCollider(const std::string& name,
                                                   float x,
                                                   float y,
                                                   float width,
                                                   float height) const {
    const ColliderDefinition& definition = GetDefinition(name);
    return Collider::Rectangle(definition.name, x + definition.offsetX, y + definition.offsetY, width, height);
}

Collider ColliderRegistry::CreateCircleCollider(const std::string& name,
                                                float centerX,
                                                float centerY,
                                                float radius) const {
    const ColliderDefinition& definition = GetDefinition(name);
    const float finalRadius = radius > 0.0f ? radius : definition.radius;
    return Collider::Circle(definition.name, centerX + definition.offsetX, centerY + definition.offsetY, finalRadius);
}

void ColliderRegistry::EnsureLoaded() const {
    if (!m_loaded) {
        const_cast<ColliderRegistry*>(this)->LoadFromFile(DEFAULT_COLLIDER_CONFIG);
    }
}

void ColliderRegistry::RegisterDefaults() {
    RegisterDefinition({ "default", ColliderShape::Rectangle, true });
    RegisterDefinition({ "player", ColliderShape::Rectangle, true });
    RegisterDefinition({ "enemy", ColliderShape::Rectangle, true });
    RegisterDefinition({ "boss", ColliderShape::Rectangle, true });
    RegisterDefinition({ "player_bullet", ColliderShape::Rectangle, true });
    RegisterDefinition({ "enemy_bullet", ColliderShape::Rectangle, true });
    RegisterDefinition({ "exploding_bullet", ColliderShape::Rectangle, true });
    RegisterDefinition({ "exploding_explosion", ColliderShape::Rectangle, true });
    RegisterDefinition({ "laser", ColliderShape::Rectangle, true });
    RegisterDefinition({ "exp_orb", ColliderShape::Rectangle, true });
    RegisterDefinition({ "exp_orb_collect", ColliderShape::Circle, false, 0.0f, 0.0f, 30.0f });
    RegisterDefinition({ "circle_ground_attack", ColliderShape::Circle, false });
    RegisterDefinition({ "melee_contact", ColliderShape::Rectangle, true });
}

void ColliderRegistry::RegisterDefinition(const ColliderDefinition& definition) {
    if (definition.name.empty()) {
        return;
    }
    m_definitions[definition.name] = definition;
}
