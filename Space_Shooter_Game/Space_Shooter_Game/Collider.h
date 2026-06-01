#pragma once

#include <map>
#include <string>

enum class EnemyType;

enum class ColliderShape {
    Rectangle,
    Oval
};

enum class ColliderPositionMode {
    Absolute,
    OwnerBounds
};

struct ColliderDefinition {
    std::string name;
    ColliderShape shape = ColliderShape::Rectangle;
    ColliderPositionMode positionMode = ColliderPositionMode::OwnerBounds;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    float expandX = 0.0f;
    float expandY = 0.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
};

struct Collider {
    std::string name;
    ColliderShape shape = ColliderShape::Rectangle;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    static Collider Rectangle(const std::string& name, float x, float y, float width, float height);
    static Collider Oval(const std::string& name, float x, float y, float width, float height);

    float Left() const;
    float Right() const;
    float Top() const;
    float Bottom() const;
    float CenterX() const;
    float CenterY() const;

    bool Intersects(const Collider& other) const;
};

class ColliderRegistry {
public:
    static ColliderRegistry& GetInstance();

    bool LoadFromFile(const std::string& filePath);
    void LoadAll();
    const ColliderDefinition& GetDefinition(const std::string& name) const;
    bool HasCollider(const std::string& name) const;
    static const char* GetColliderNameForEnemyType(EnemyType type);

    Collider CreateCollider(const std::string& name,
                            float ownerX,
                            float ownerY,
                            float ownerWidth,
                            float ownerHeight) const;

    Collider CreateColliderAt(const std::string& name, float anchorX, float anchorY) const;

private:
    ColliderRegistry();

    void EnsureLoaded() const;
    void RegisterDefaults();
    void RegisterDefinition(const ColliderDefinition& definition);

    mutable bool m_loaded = false;
    std::map<std::string, ColliderDefinition> m_definitions;
};
