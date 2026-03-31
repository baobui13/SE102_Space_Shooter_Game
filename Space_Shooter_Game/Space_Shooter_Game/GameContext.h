#pragma once
#include "Graphics.h"
#include "InputManager.h"
#include "AssetManager.h"

class BulletPool; // Forward declaration
class Player;    // Forward declaration 
class EntityManager; // Forward declaration 

struct GameContext {
    Graphics& gfx;
    InputManager& input;
    AssetManager& assets;
    BulletPool& bulletPool;
    EntityManager& entityManager;
    Player& player;      // Truy cập nhanh tới Player từ mọi nơi
    
    float screenWidth;
    float screenHeight;

    GameContext(Graphics& g, InputManager& i, AssetManager& a, BulletPool& b, EntityManager& e, Player& p, float sw, float sh)
        : gfx(g), input(i), assets(a), bulletPool(b), entityManager(e), player(p), screenWidth(sw), screenHeight(sh) {}
};
