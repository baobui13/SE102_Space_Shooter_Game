#pragma once
#include "BaseGameplayScene.h"

class Level2Scene : public BaseGameplayScene {
public:
    Level2Scene(Graphics& gfx);
    ~Level2Scene() = default;

protected:
    void InitializeLevel() override;
};