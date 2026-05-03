#pragma once
#include "BaseGameplayScene.h"

class Level3Scene : public BaseGameplayScene {
public:
    Level3Scene(Graphics& gfx);
    ~Level3Scene() = default;

protected:
    void InitializeLevel() override;
};