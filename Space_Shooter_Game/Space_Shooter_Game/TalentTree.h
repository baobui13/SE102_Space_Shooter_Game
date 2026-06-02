#pragma once
#include "TalentNode.h"
#include "Graphics.h"
#include <vector>
#include <memory>

class Player; // Forward declaration

class TalentTree {
public:
    TalentTree();
    ~TalentTree() = default;

    void Initialize(Graphics& gfx);
    void Update(float mouseX, float mouseY, bool isLeftClicked, float screenWidth, float screenHeight, Player& player);
    void Render(Graphics& gfx, float screenWidth, float screenHeight, DirectX::SpriteFont* font) const;

    const std::vector<std::unique_ptr<TalentNode>>& GetNodes() const { return m_nodes; }
    
    const TalentNode* GetHoveredNode() const;

    // So upgradePoints duoc luu trong save file (de Player.cpp doc lai sau khi load)
    int GetSavedUpgradePoints() const { return m_savedUpgradePoints; }
    bool HasSaveData() const { return m_hasSaveData; }

    // Ap dung lai buff cho tat ca node da duoc mo (dung sau khi load save)
    void ApplyAllUnlockedBuffs(Player& player);

    // Ghi toan bo trang thai talent hien tai ra file save
    void PersistSave(const Player& player) const;

private:
    bool CanUnlockNode(int nodeId, const Player& player) const;
    void ApplyBuff(TalentNode* node, Player& player);
    std::vector<std::unique_ptr<TalentNode>> m_nodes;
    
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lineTexture;

    // Du lieu tai duoc tu save file
    int m_savedUpgradePoints = 0;
    bool m_hasSaveData = false;
};
