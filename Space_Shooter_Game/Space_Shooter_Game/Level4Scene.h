#pragma once
#include "BaseGameplayScene.h"

class Level4Scene : public BaseGameplayScene {
public:
    Level4Scene(Graphics& gfx);
    ~Level4Scene() = default;

protected:
    void InitializeLevel() override;
};