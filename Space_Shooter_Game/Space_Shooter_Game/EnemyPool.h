#pragma once
#include "EnemyDefinitions.h"

class EnemyPool {
public:
    static const EnemyVisualDefinition& GetVisual(EnemyType type);
};
