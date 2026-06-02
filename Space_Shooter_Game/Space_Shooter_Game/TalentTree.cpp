#include "TalentTree.h"
#include "Player.h"
#include "Graphics.h"
#include "SimpleJson.h"
#include "ConfigUtils.h"
#include "AssetManager.h"
#include "SaveManager.h"
#include <iostream>

TalentType ParseTalentType(const std::string& typeStr) {
    if (typeStr == "ATTACK_DAMAGE") return TalentType::ATTACK_DAMAGE;
    if (typeStr == "ATTACK_SPEED") return TalentType::ATTACK_SPEED;
    if (typeStr == "ATTACK_RANGE") return TalentType::ATTACK_RANGE;
    if (typeStr == "COOLDOWN_REDUCTION") return TalentType::COOLDOWN_REDUCTION;
    if (typeStr == "SKILL_SIZE") return TalentType::SKILL_SIZE;
    if (typeStr == "MAX_HP") return TalentType::MAX_HP;
    if (typeStr == "MOVEMENT_SPEED") return TalentType::MOVEMENT_SPEED;
    return TalentType::MAX_HP;
}

TalentTree::TalentTree() {
}

void TalentTree::Initialize(Graphics& gfx) {
    m_lineTexture = AssetManager::GetInstance().GetTexture(gfx, L"Assets/WhitePoint.png");

    if (m_nodes.empty()) {
        // 1. Load cấu hình node từ JSON
        try {
            const JsonValue root = SimpleJson::ParseFile(ResolveConfigPath("config/player/talent_tree.json"));
            const JsonValue::Array& nodesArray = root.At("nodes").AsArray();
            
            for (const auto& nodeJson : nodesArray) {
                int id = static_cast<int>(nodeJson.At("id").AsNumber());
                int parentId = static_cast<int>(nodeJson.At("parentId").AsNumber());
                std::string typeStr = nodeJson.At("type").AsString();
                float buffAmount = static_cast<float>(nodeJson.At("buffAmount").AsNumber());
                float x = static_cast<float>(nodeJson.At("x").AsNumber());
                float y = static_cast<float>(nodeJson.At("y").AsNumber());
                std::string iconPath = nodeJson.At("icon").AsString();
                
                auto node = std::make_unique<TalentNode>(id, parentId, ParseTalentType(typeStr), buffAmount, x, y);
                std::wstring wIconPath(iconPath.begin(), iconPath.end());
                node->SetIcon(AssetManager::GetInstance().GetTexture(gfx, wIconPath));
                m_nodes.push_back(std::move(node));
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to load talent tree config: " << e.what() << std::endl;
        }

        // 2. Phuc hoi trang thai da mo tu file save
        SaveManager::TalentSaveData saveData;
        if (SaveManager::GetInstance().LoadTalentData(saveData)) {
            m_hasSaveData = true;
            m_savedUpgradePoints = saveData.upgradePoints;
            for (int id : saveData.unlockedNodeIds) {
                for (auto& node : m_nodes) {
                    if (node->GetId() == id) {
                        node->Unlock();
                        break;
                    }
                }
            }
        }
    }
}

void TalentTree::Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth, float screenHeight, Player& player) {
    for (auto& node : m_nodes) {
        node->Update(mouseX, mouseY, isLeftClicked, screenWidth, screenHeight);
        
        if (node->IsClicked()) {
            if (CanUnlockNode(node->GetId(), player)) {
                node->Unlock();
                player.UseUpgradePoint();
                ApplyBuff(node.get(), player);
                std::cout << "Unlocked node " << node->GetId() << std::endl;

                // Lưu trạng thái ngay sau khi mở node
                PersistSave(player);
            } else {
                std::cout << "Cannot unlock node " << node->GetId() << std::endl;
            }
            node->ResetClicked();
        }
    }
}

void TalentTree::Render(Graphics& gfx, float screenWidth, float screenHeight, DirectX::SpriteFont* font) const {
    // 1. Draw lines between nodes
    if (m_lineTexture) {
        for (const auto& node : m_nodes) {
            if (node->GetParentId() != -1) {
                // Find parent
                const TalentNode* parentNode = nullptr;
                for (const auto& p : m_nodes) {
                    if (p->GetId() == node->GetParentId()) {
                        parentNode = p.get();
                        break;
                    }
                }
                
                if (parentNode) {
                    DirectX::XMVECTOR color = node->IsUnlocked() ? DirectX::Colors::Yellow : DirectX::Colors::Gray;
                    gfx.DrawLine(m_lineTexture.Get(), parentNode->GetX(), parentNode->GetY(), node->GetX(), node->GetY(), 4.0f, color);
                }
            }
        }
    }

    // 2. Draw nodes (SpriteBatch needs to be begun and ended by the scene, assuming it is active here)
    // Wait, DrawLine might mess with SpriteBatch state if not careful, but we assume it's fine.
    // We should draw nodes after lines.
    DirectX::SpriteBatch* sb = gfx.GetSpriteBatch();
    for (const auto& node : m_nodes) {
        node->Render(sb, screenWidth, screenHeight, font);
    }
}

bool TalentTree::CanUnlockNode(int nodeId, const Player& player) const {
    if (player.GetUpgradePoints() <= 0) return false;
    
    const TalentNode* targetNode = nullptr;
    for (const auto& node : m_nodes) {
        if (node->GetId() == nodeId) {
            targetNode = node.get();
            break;
        }
    }
    
    if (!targetNode || targetNode->IsUnlocked()) return false;
    
    if (targetNode->GetParentId() == -1) return true; // Should not happen, root is already unlocked
    
    // Check if parent is unlocked
    for (const auto& node : m_nodes) {
        if (node->GetId() == targetNode->GetParentId()) {
            return node->IsUnlocked();
        }
    }
    
    return false;
}

void TalentTree::ApplyBuff(TalentNode* node, Player& player) {
    if (!node || node->GetId() == 0) return; // Node 0 has no buff

    float buff = node->GetBuffAmount();
    switch (node->GetType()) {
        case TalentType::ATTACK_DAMAGE:
            player.SetAttackDamage(player.GetAttackDamage() + static_cast<int>(buff));
            break;
        case TalentType::ATTACK_SPEED:
            // Assuming smaller attack speed is faster (delay between shots)
            player.SetAttackSpeed(player.GetAttackSpeed() - buff); 
            break;
        case TalentType::ATTACK_RANGE:
            player.SetAttackRange(player.GetAttackRange() + buff);
            break;
        case TalentType::COOLDOWN_REDUCTION:
            // Assuming cooldown multiplier gets smaller
            player.SetCooldownMultiplier(player.GetCooldownMultiplier() - buff);
            break;
        case TalentType::SKILL_SIZE:
            player.SetSkillSizeMultiplier(player.GetSkillSizeMultiplier() + buff);
            break;
        case TalentType::MAX_HP:
            player.SetMaxHp(player.GetMaxHp() + static_cast<int>(buff));
            player.Heal(static_cast<int>(buff)); // Heal the player for the new max HP
            break;
        case TalentType::MOVEMENT_SPEED:
            player.SetSpeed(player.GetSpeed() + buff);
            break;
    }
}

const TalentNode* TalentTree::GetHoveredNode() const {
    for (const auto& node : m_nodes) {
        if (node->IsHovered()) {
            return node.get();
        }
    }
    return nullptr;
}

void TalentTree::PersistSave(const Player& player) const {
    SaveManager::TalentSaveData data;
    data.upgradePoints = player.GetUpgradePoints();
    for (const auto& node : m_nodes) {
        if (node->IsUnlocked()) {
            data.unlockedNodeIds.push_back(node->GetId());
        }
    }
    SaveManager::GetInstance().SaveTalentData(data);
}

void TalentTree::ApplyAllUnlockedBuffs(Player& player) {
    // Ap dung lai buff cua tung node da mo (bo qua root node id=0 vi no khong co buff)
    for (const auto& node : m_nodes) {
        if (node->IsUnlocked() && node->GetId() != 0) {
            ApplyBuff(node.get(), player);
        }
    }
}
