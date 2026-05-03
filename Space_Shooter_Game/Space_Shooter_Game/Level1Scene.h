#pragma once
#include "BaseGameplayScene.h"

class Level1Scene : public BaseGameplayScene {
public:
    Level1Scene(Graphics& gfx);
    ~Level1Scene() = default;

protected:
    void InitializeLevel() override;
};