#pragma once
#include "Graphics.h"
#include "InputManager.h"
#include "AssetManager.h"

class BulletPool; // Forward declaration
class Player;    // Forward declaration 

struct GameContext {
    Graphics& gfx;
    InputManager& input;
    AssetManager& assets;
    BulletPool& bulletPool;
    Player& player;      // Truy cập nhanh tới Player từ mọi nơi

    GameContext(Graphics& g, InputManager& i, AssetManager& a, BulletPool& b, Player& p)
        : gfx(g), input(i), assets(a), bulletPool(b), player(p) {}
};
