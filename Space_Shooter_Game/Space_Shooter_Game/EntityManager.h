#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "GameObject.h"
#include "GameContext.h"
#include "ExpOrb.h" // Needed for dynamic_cast if used in EntityManager, but it's used in GameplayScene

// EntityManager: Quản lý tập trung vòng đời và tương tác của các đối tượng trong game
class EntityManager {
private:
    std::vector<std::unique_ptr<GameObject>> m_entities;
    std::vector<std::unique_ptr<GameObject>> m_pendingEntities; // Tránh sửa vector khi đang duyệt

public:
    void AddEntity(std::unique_ptr<GameObject> entity) {
        m_pendingEntities.push_back(std::move(entity));
    }

    void UpdateAll(float dt, ::GameContext& ctx) {
        // Thêm các entity mới vào danh sách chính
        if (!m_pendingEntities.empty()) {
            for (auto& entity : m_pendingEntities) {
                m_entities.push_back(std::move(entity));
            }
            m_pendingEntities.clear();
        }

        // Cập nhật từng entity
        for (auto& entity : m_entities) {
            if (entity->IsActive()) {
                entity->Update(dt, ctx);
            }
        }

        // Dọn dẹp các entity đã bị hủy
        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                [](const std::unique_ptr<GameObject>& e) { return !e->IsActive(); }),
            m_entities.end()
        );
    }

    void RenderAll(Graphics& gfx) {
        for (auto& entity : m_entities) {
            if (entity->IsActive()) {
                entity->Render(gfx);
            }
        }
    }

    const std::vector<std::unique_ptr<GameObject>>& GetEntities() const {
        return m_entities;
    }

    void Clear() {
        m_entities.clear();
        m_pendingEntities.clear();
    }
};
