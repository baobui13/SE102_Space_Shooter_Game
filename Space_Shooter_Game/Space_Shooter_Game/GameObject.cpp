#include "GameObject.h"
#include "GameContext.h"

GameObject::GameObject(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height),
    m_vx(0.0f), m_vy(0.0f), m_isActive(true), m_colliderName("default") {
}

void GameObject::Update(float dt, ::GameContext& ctx) {
    m_x += m_vx * dt;
    m_y += m_vy * dt;
    m_anim.Update(dt);
}

void GameObject::Render(Graphics& gfx) {
    if (!m_isActive) return;
    m_anim.Render(gfx, m_x, m_y, m_width, m_height);
}

Collider GameObject::GetCollider() const {
    return ColliderRegistry::GetInstance().CreateCollider(
        m_colliderName,
        m_x,
        m_y,
        m_width,
        m_height);
}

bool GameObject::CheckCollision(const GameObject& other) const {
    if (!m_isActive || !other.IsActive()) return false;
    return GetCollider().Intersects(other.GetCollider());
}

bool GameObject::CheckCollision(const Collider& other) const {
    if (!m_isActive) return false;
    return GetCollider().Intersects(other);
}
