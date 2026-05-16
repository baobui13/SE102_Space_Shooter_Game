#pragma once
#include "AttackMarkerDefinitions.h"

class AttackMarkerPool {
public:
    static const AttackMarkerDefinition& Get(AttackMarkerType type);
};
