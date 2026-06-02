#include "Collider.h"
#include "BaseEnemy.h"
#include "ConfigUtils.h"
#include "SimpleJson.h"

#include <cmath>

namespace {
constexpr const char* DEFAULT_COLLIDER_CONFIG = "config/colliders/colliders.json";

float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

float MaxFloat(float a, float b) {
    return a > b ? a : b;
}

float MinFloat(float a, float b) {
    return a < b ? a : b;
}

ColliderShape ShapeFromString(const std::string& value) {
    if (value == "oval" || value == "Oval" || value == "circle" || value == "Circle") {
        return ColliderShape::Oval;
    }
    return ColliderShape::Rectangle;
}

ColliderPositionMode PositionModeFromString(const std::string& value) {
    if (value == "absolute" || value == "Absolute") {
        return ColliderPositionMode::Absolute;
    }
    return ColliderPositionMode::OwnerBounds;
}

ColliderDefinition ParseDefinition(const JsonValue& value) {
    ColliderDefinition definition;
    definition.name = value.At("name").AsStringOr("default");
    definition.shape = ShapeFromString(value.At("shape").AsStringOr("rectangle"));
    definition.positionMode = PositionModeFromString(value.At("positionMode").AsStringOr("ownerBounds"));
    definition.x = ReadFloat(value, "x", 0.0f);
    definition.y = ReadFloat(value, "y", 0.0f);
    definition.width = ReadFloat(value, "width", 0.0f);
    definition.height = ReadFloat(value, "height", 0.0f);
    definition.expandX = ReadFloat(value, "expandX", 0.0f);
    definition.expandY = ReadFloat(value, "expandY", 0.0f);
    definition.offsetX = ReadFloat(value, "offsetX", 0.0f);
    definition.offsetY = ReadFloat(value, "offsetY", 0.0f);
    return definition;
}

bool RectIntersectsRect(const Collider& a, const Collider& b) {
    return a.Left() < b.Right() &&
        a.Right() > b.Left() &&
        a.Top() < b.Bottom() &&
        a.Bottom() > b.Top();
}

bool PointInOval(float px, float py, const Collider& oval) {
    const float rx = oval.width * 0.5f;
    const float ry = oval.height * 0.5f;
    if (rx <= 0.0f || ry <= 0.0f) {
        return false;
    }

    const float dx = (px - oval.CenterX()) / rx;
    const float dy = (py - oval.CenterY()) / ry;
    return dx * dx + dy * dy <= 1.0f;
}

bool PointInRect(float px, float py, const Collider& rect) {
    return px >= rect.Left() && px <= rect.Right() && py >= rect.Top() && py <= rect.Bottom();
}

bool RectIntersectsOval(const Collider& rect, const Collider& oval) {
    if (!RectIntersectsRect(rect, oval)) {
        return false;
    }

    const float rx = oval.width * 0.5f;
    const float ry = oval.height * 0.5f;
    if (rx <= 0.0f || ry <= 0.0f) {
        return false;
    }

    const float normalizedLeft = (rect.Left() - oval.CenterX()) / rx;
    const float normalizedRight = (rect.Right() - oval.CenterX()) / rx;
    const float normalizedTop = (rect.Top() - oval.CenterY()) / ry;
    const float normalizedBottom = (rect.Bottom() - oval.CenterY()) / ry;
    const float closestX = MaxFloat(normalizedLeft, MinFloat(0.0f, normalizedRight));
    const float closestY = MaxFloat(normalizedTop, MinFloat(0.0f, normalizedBottom));

    return closestX * closestX + closestY * closestY <= 1.0f;
}

bool OvalIntersectsOval(const Collider& a, const Collider& b) {
    if (!RectIntersectsRect(a, b)) {
        return false;
    }

    if (PointInOval(a.CenterX(), a.CenterY(), b) || PointInOval(b.CenterX(), b.CenterY(), a)) {
        return true;
    }

    constexpr int SAMPLE_COUNT = 24;
    const float aCx = a.CenterX();
    const float aCy = a.CenterY();
    const float aRx = a.width * 0.5f;
    const float aRy = a.height * 0.5f;
    for (int i = 0; i < SAMPLE_COUNT; ++i) {
        const float angle = static_cast<float>(i) * 2.0f * 3.1415926535f / static_cast<float>(SAMPLE_COUNT);
        if (PointInOval(aCx + std::cos(angle) * aRx, aCy + std::sin(angle) * aRy, b)) {
            return true;
        }
    }

    const float bCx = b.CenterX();
    const float bCy = b.CenterY();
    const float bRx = b.width * 0.5f;
    const float bRy = b.height * 0.5f;
    for (int i = 0; i < SAMPLE_COUNT; ++i) {
        const float angle = static_cast<float>(i) * 2.0f * 3.1415926535f / static_cast<float>(SAMPLE_COUNT);
        if (PointInOval(bCx + std::cos(angle) * bRx, bCy + std::sin(angle) * bRy, a)) {
            return true;
        }
    }

    return false;
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
    return collider;
}

Collider Collider::Oval(const std::string& name, float x, float y, float width, float height) {
    Collider collider;
    collider.name = name;
    collider.shape = ColliderShape::Oval;
    collider.x = x;
    collider.y = y;
    collider.width = width;
    collider.height = height;
    return collider;
}

float Collider::Left() const {
    return x;
}

float Collider::Right() const {
    return x + width;
}

float Collider::Top() const {
    return y;
}

float Collider::Bottom() const {
    return y + height;
}

float Collider::CenterX() const {
    return x + width * 0.5f;
}

float Collider::CenterY() const {
    return y + height * 0.5f;
}

bool Collider::Intersects(const Collider& other) const {
    if (shape == ColliderShape::Rectangle && other.shape == ColliderShape::Rectangle) {
        return RectIntersectsRect(*this, other);
    }

    if (shape == ColliderShape::Oval && other.shape == ColliderShape::Oval) {
        return OvalIntersectsOval(*this, other);
    }

    if (shape == ColliderShape::Rectangle) {
        return RectIntersectsOval(*this, other);
    }

    return RectIntersectsOval(other, *this);
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

bool ColliderRegistry::HasCollider(const std::string& name) const {
    EnsureLoaded();
    return m_definitions.find(name) != m_definitions.end();
}

void ColliderRegistry::LoadAll() {
    LoadFromFile(DEFAULT_COLLIDER_CONFIG);
}

const char* ColliderRegistry::GetColliderNameForEnemyType(EnemyType type) {
    switch (type) {
    case EnemyType::Melee_Basic:
        return "enemy_melee_basic";
    case EnemyType::Melee_Fast:
        return "enemy_melee_fast";
    case EnemyType::Melee_Spawner:
        return "enemy_melee_spawner";
    case EnemyType::Ranged_Basic:
        return "enemy_ranged_basic";
    case EnemyType::Ranged_Burst:
        return "enemy_ranged_burst";
    case EnemyType::Boss_Stage1:
        return "boss";
    default:
        return "enemy";
    }
}

Collider ColliderRegistry::CreateCollider(const std::string& name,
                                          float ownerX,
                                          float ownerY,
                                          float ownerWidth,
                                          float ownerHeight) const {
    const ColliderDefinition& definition = GetDefinition(name);
    const bool ownerBounds = definition.positionMode == ColliderPositionMode::OwnerBounds;

    float width = definition.width;
    float height = definition.height;
    float x = definition.x + definition.offsetX;
    float y = definition.y + definition.offsetY;

    if (ownerBounds) {
        const bool useOwnerWidth = width <= 0.0f;
        const bool useOwnerHeight = height <= 0.0f;
        width = useOwnerWidth
            ? MaxFloat(0.0f, ownerWidth + definition.expandX * 2.0f)
            : width;
        height = useOwnerHeight
            ? MaxFloat(0.0f, ownerHeight + definition.expandY * 2.0f)
            : height;
        x = useOwnerWidth
            ? ownerX - definition.expandX + definition.offsetX
            : ownerX + (ownerWidth - width) * 0.5f + definition.offsetX;
        y = useOwnerHeight
            ? ownerY - definition.expandY + definition.offsetY
            : ownerY + (ownerHeight - height) * 0.5f + definition.offsetY;
    }

    if (definition.shape == ColliderShape::Oval) {
        return Collider::Oval(definition.name, x, y, width, height);
    }
    return Collider::Rectangle(definition.name, x, y, width, height);
}

Collider ColliderRegistry::CreateColliderAt(const std::string& name, float anchorX, float anchorY) const {
    const ColliderDefinition& definition = GetDefinition(name);
    const float x = anchorX + definition.x + definition.offsetX;
    const float y = anchorY + definition.y + definition.offsetY;

    if (definition.shape == ColliderShape::Oval) {
        return Collider::Oval(definition.name, x, y, definition.width, definition.height);
    }
    return Collider::Rectangle(definition.name, x, y, definition.width, definition.height);
}

void ColliderRegistry::EnsureLoaded() const {
    if (!m_loaded) {
        const_cast<ColliderRegistry*>(this)->LoadFromFile(DEFAULT_COLLIDER_CONFIG);
    }
}

void ColliderRegistry::RegisterDefaults() {
    RegisterDefinition({ "default", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "player", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 0.0f, 0.0f, -5.0f, -5.0f });
    RegisterDefinition({ "enemy", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 0.0f, 0.0f, -6.0f, -6.0f });
    RegisterDefinition({ "enemy_melee_basic", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 96.0f, 96.0f });
    RegisterDefinition({ "enemy_melee_fast", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 28.0f, 28.0f });
    RegisterDefinition({ "enemy_melee_spawner", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 120.0f, 120.0f });
    RegisterDefinition({ "enemy_ranged_basic", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 52.0f, 52.0f });
    RegisterDefinition({ "enemy_ranged_burst", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 60.0f, 60.0f });
    RegisterDefinition({ "boss", ColliderShape::Oval, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 420.0f, 640.0f });
    RegisterDefinition({ "player_bullet", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "enemy_bullet", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 0.0f, 0.0f, -4.0f, -4.0f });
    RegisterDefinition({ "exploding_bullet", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "exploding_explosion", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "laser", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "laser_hitbox", ColliderShape::Rectangle, ColliderPositionMode::Absolute, -3.0f, -300.0f, 6.0f, 300.0f });
    RegisterDefinition({ "exp_orb", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "exp_orb_collect", ColliderShape::Oval, ColliderPositionMode::Absolute, -15.0f, -15.0f, 30.0f, 30.0f });
    RegisterDefinition({ "talent_orb", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "talent_orb_collect", ColliderShape::Oval, ColliderPositionMode::Absolute, -16.0f, -16.0f, 32.0f, 32.0f });
    RegisterDefinition({ "circle_ground_attack", ColliderShape::Oval, ColliderPositionMode::OwnerBounds });
    RegisterDefinition({ "melee_contact", ColliderShape::Rectangle, ColliderPositionMode::OwnerBounds, 0.0f, 0.0f, 0.0f, 0.0f, 4.0f, 4.0f });
}

void ColliderRegistry::RegisterDefinition(const ColliderDefinition& definition) {
    if (definition.name.empty()) {
        return;
    }
    m_definitions[definition.name] = definition;
}
