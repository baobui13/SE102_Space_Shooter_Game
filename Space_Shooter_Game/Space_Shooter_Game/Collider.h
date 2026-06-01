#pragma once

#include <map>
#include <string>

enum class ColliderShape {
    Rectangle,
    Circle
};

struct ColliderDefinition {
    std::string name;
    ColliderShape shape = ColliderShape::Rectangle;
    bool useOwnerSize = true;
    float width = 0.0f;
    float height = 0.0f;
    float radius = 0.0f;
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
    float radius = 0.0f;

    static Collider Rectangle(const std::string& name, float x, float y, float width, float height);
    static Collider Circle(const std::string& name, float centerX, float centerY, float radius);

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
    const ColliderDefinition& GetDefinition(const std::string& name) const;

    Collider CreateCollider(const std::string& name,
                            float ownerX,
                            float ownerY,
                            float ownerWidth,
                            float ownerHeight) const;

    Collider CreateRectangleCollider(const std::string& name,
                                     float x,
                                     float y,
                                     float width,
                                     float height) const;

    Collider CreateCircleCollider(const std::string& name,
                                  float centerX,
                                  float centerY,
                                  float radius) const;

private:
    ColliderRegistry();

    void EnsureLoaded() const;
    void RegisterDefaults();
    void RegisterDefinition(const ColliderDefinition& definition);

    mutable bool m_loaded = false;
    std::map<std::string, ColliderDefinition> m_definitions;
};
