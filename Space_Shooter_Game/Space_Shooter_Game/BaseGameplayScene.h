#pragma once
#include "Graphics.h"
#include "InputManager.h"
#include "Player.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Button.h"
#include "GameConfig.h"
#include <memory>
#include <SpriteFont.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <d3d11.h>
#include "ProgressBar.h"
#include "EntityManager.h"
#include "BulletPool.h"

// Base class for all gameplay scenes - handles common player logic, rendering, etc.
class BaseGameplayScene : public Scene {
protected:
    Graphics& m_gfx;
    std::unique_ptr<Player> m_player;
    EntityManager m_entityManager;
    BulletPool m_bulletPool;

    std::unique_ptr<DirectX::SpriteFont> m_font;
    std::unique_ptr<ProgressBar> m_hpBar;
    int m_levelIndex;
    std::unique_ptr<Button> m_pauseButton;

    DirectX::XMFLOAT3 m_backgroundColor;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTexture;

public:
    BaseGameplayScene(Graphics& gfx, int levelIndex = 1);
    virtual ~BaseGameplayScene() = default;

    void Update(float dt, InputManager& input, SceneManager& manager) override;
    void Render(Graphics& gfx) override;

    void SetBackgroundColor(const DirectX::XMFLOAT3& color);
    void SetBackgroundTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

protected:
    // Initialize player with default skills - can be overridden in subclasses
    virtual void InitializePlayer();

    // Initialize level-specific entities - override this in subclasses to add enemies
    virtual void InitializeLevel();

    // Handle level-specific input - override this in subclasses
    virtual void HandleLevelInput(InputManager& input, SceneManager& manager);

    // Render level-specific elements - override this in subclasses
    virtual void RenderLevelElements(DirectX::SpriteBatch* spriteBatch);
};
