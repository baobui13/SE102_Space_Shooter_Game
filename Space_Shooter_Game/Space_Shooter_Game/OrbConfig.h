#pragma once

#include <string>

struct OrbDefinition {
    std::string animationId = "exp_orb_idle";
    std::string colliderName = "exp_orb";
    std::string collectColliderName = "exp_orb_collect";
    float width = 16.0f;
    float height = 16.0f;
    float magnetSpeed = 450.0f;
    float fallSpeed = 50.0f;
};

class OrbConfig {
public:
    static const OrbDefinition& GetExpOrb();
    static const OrbDefinition& GetTalentOrb();

private:
    static const OrbDefinition& GetDefinition(const std::string& id);
};
