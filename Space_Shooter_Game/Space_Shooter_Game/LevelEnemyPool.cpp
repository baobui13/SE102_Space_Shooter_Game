#include "LevelEnemyPool.h"
#include "GameConfig.h"
#include "SimpleJson.h"

#include <filesystem>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>

namespace {
using StatsMap = std::map<EnemyType, EnemyStatsDefinition>;
using MovementMap = std::map<EnemyType, EnemyMovementSequenceDefinition>;

std::string ResolveConfigPath(const std::string& filePath) {
    if (std::filesystem::exists(filePath)) {
        return filePath;
    }

    std::filesystem::path current = std::filesystem::current_path();
    const std::filesystem::path relativePath(filePath);
    for (int i = 0; i < 6 && !current.empty(); ++i) {
        const std::filesystem::path candidate = current / relativePath;
        if (std::filesystem::exists(candidate)) {
            return candidate.string();
        }
        current = current.parent_path();
    }

    const std::filesystem::path projectPath =
        std::filesystem::path("Space_Shooter_Game") / "Space_Shooter_Game" / filePath;
    if (std::filesystem::exists(projectPath)) {
        return projectPath.string();
    }

    return filePath;
}

EnemyType EnemyTypeFromString(const std::string& value) {
    if (value == "Melee_Fast") return EnemyType::Melee_Fast;
    if (value == "Melee_Spawner") return EnemyType::Melee_Spawner;
    if (value == "Ranged_Basic") return EnemyType::Ranged_Basic;
    if (value == "Ranged_Burst") return EnemyType::Ranged_Burst;
    if (value == "Boss_Stage1") return EnemyType::Boss_Stage1;
    return EnemyType::Melee_Basic;
}

float ReadFloat(const JsonValue& object, const std::string& key, float fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<float>(value->AsNumber(fallback)) : fallback;
}

int ReadInt(const JsonValue& object, const std::string& key, int fallback) {
    const JsonValue* value = object.Find(key);
    return value ? static_cast<int>(value->AsNumber(fallback)) : fallback;
}

float ReadCoordinate(const JsonValue& value, float fallback, bool isX) {
    if (value.IsNumber()) {
        return static_cast<float>(value.AsNumber(fallback));
    }

    const std::string token = value.AsStringOr("");
    if (token == "center") {
        return isX ? VIRTUAL_WIDTH * 0.5f : VIRTUAL_HEIGHT * 0.5f;
    }
    if (token == "screenWidth") {
        return VIRTUAL_WIDTH;
    }
    if (token == "screenHeight") {
        return VIRTUAL_HEIGHT;
    }

    return fallback;
}

EnemyStatsDefinition ParseStats(const JsonValue& value, EnemyStatsDefinition fallback) {
    fallback.health = ReadFloat(value, "health", fallback.health);
    fallback.moveSpeed = ReadFloat(value, "moveSpeed", fallback.moveSpeed);
    fallback.attackPower = ReadFloat(value, "attackPower", fallback.attackPower);
    fallback.attackSpeed = ReadFloat(value, "attackSpeed", fallback.attackSpeed);
    fallback.attackRange = ReadFloat(value, "attackRange", fallback.attackRange);
    fallback.expReward = ReadInt(value, "expReward", fallback.expReward);
    return fallback;
}

EnemyStatsDefinition DefaultStats() {
    return { 1.0f, 80.0f, 1.0f, 1.0f, 30.0f, 0 };
}

EnemyStatsDefinition GetStats(const StatsMap& stats, EnemyType type) {
    auto it = stats.find(type);
    return it == stats.end() ? DefaultStats() : it->second;
}

EnemyMovementDefinition ParseMovementDefinition(const JsonValue& value) {
    const std::string kind = value.At("kind").AsStringOr("Chase");

    if (kind == "Linear") {
        return EnemyMovementDefinition::Linear(
            ReadFloat(value, "speedX", 0.0f),
            ReadFloat(value, "speedY", 0.0f));
    }
    if (kind == "SineWave") {
        return EnemyMovementDefinition::SineWave(
            ReadFloat(value, "speedY", 60.0f),
            ReadFloat(value, "amplitude", 120.0f),
            ReadFloat(value, "frequency", 1.0f));
    }
    if (kind == "Circular") {
        return EnemyMovementDefinition::Circular(
            ReadCoordinate(value.At("centerX"), VIRTUAL_WIDTH * 0.5f, true),
            ReadCoordinate(value.At("centerY"), VIRTUAL_HEIGHT * 0.5f, false),
            ReadFloat(value, "radius", 240.0f),
            ReadFloat(value, "angularSpeed", 0.5f));
    }
    if (kind == "FigureEight") {
        return EnemyMovementDefinition::FigureEight(
            ReadCoordinate(value.At("centerX"), VIRTUAL_WIDTH * 0.5f, true),
            ReadCoordinate(value.At("centerY"), VIRTUAL_HEIGHT * 0.5f, false),
            ReadFloat(value, "radiusX", VIRTUAL_WIDTH * 0.25f),
            ReadFloat(value, "radiusY", VIRTUAL_HEIGHT * 0.2f));
    }

    return EnemyMovementDefinition::Chase(ReadFloat(value, "speed", 80.0f));
}

EnemyMovementStepDefinition ParseMovementStep(const JsonValue& value) {
    const JsonValue& movementValue = value.Contains("movement") ? value.At("movement") : value;
    return EnemyMovementStepDefinition::Step(
        ReadFloat(value, "duration", 0.0f),
        ParseMovementDefinition(movementValue));
}

EnemyMovementSequenceDefinition ParseMovementSequence(const JsonValue& value) {
    const std::string preset = value.At("preset").AsStringOr("");
    if (preset == "BossPatrol") {
        return EnemyMovementSequenceDefinition::BossPatrol(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    }

    std::vector<EnemyMovementStepDefinition> steps;
    for (const auto& step : value.At("steps").AsArray()) {
        steps.push_back(ParseMovementStep(step));
    }
    if (steps.empty()) {
        steps.push_back(EnemyMovementStepDefinition::Step(
            0.0f,
            EnemyMovementDefinition::Chase(ReadFloat(value, "speed", 80.0f))));
    }

    EnemyMovementSequenceDefinition sequence =
        value.At("mode").AsStringOr("Linear") == "Loop"
            ? EnemyMovementSequenceDefinition::Loop(std::move(steps))
            : EnemyMovementSequenceDefinition::Linear(std::move(steps));

    const JsonValue& flee = value.At("fleeFromPlayer");
    if (flee.IsObject()) {
        sequence.fleeTriggerDistance = ReadFloat(flee, "triggerDistance", 0.0f);
        sequence.fleeSpeed = ReadFloat(flee, "fleeSpeed", 0.0f);
    }
    return sequence;
}

EnemyMovementSequenceDefinition GetMovement(const MovementMap& movements, EnemyType type) {
    auto it = movements.find(type);
    if (it != movements.end()) {
        return it->second;
    }
    return EnemyMovementSequenceDefinition::Single(EnemyMovementDefinition::Chase(80.0f));
}

StatsMap LoadStats() {
    StatsMap stats;
    const JsonValue root = SimpleJson::ParseFile(
        ResolveConfigPath("config/enemies/enemy_stats.json"));
    for (const auto& [typeName, value] : root.AsObject()) {
        stats[EnemyTypeFromString(typeName)] = ParseStats(value, DefaultStats());
    }
    return stats;
}

MovementMap LoadMovements() {
    MovementMap movements;
    const JsonValue root = SimpleJson::ParseFile(
        ResolveConfigPath("config/enemies/enemy_movement.json"));
    for (const auto& [typeName, value] : root.AsObject()) {
        movements[EnemyTypeFromString(typeName)] = ParseMovementSequence(value);
    }
    return movements;
}

LevelEnemySpawnDefinition ParseSpawn(
    const JsonValue& value,
    const StatsMap& stats,
    const MovementMap& movements)
{
    const EnemyType type = EnemyTypeFromString(value.At("type").AsStringOr("Melee_Basic"));
    const float x = ReadCoordinate(value.At("x"), type == EnemyType::Boss_Stage1 ? VIRTUAL_WIDTH * 0.5f : 0.0f, true);
    const float y = ReadCoordinate(value.At("y"), 0.0f, false);

    LevelEnemySpawnDefinition spawn{
        type,
        x,
        y,
        GetStats(stats, type),
        GetMovement(movements, type)
    };

    if (value.At("stats").IsObject()) {
        spawn.stats = ParseStats(value.At("stats"), spawn.stats);
    }
    if (value.At("movement").IsObject()) {
        spawn.movementSequence = ParseMovementSequence(value.At("movement"));
    }

    for (const auto& entry : value.At("spawnEntries").AsArray()) {
        spawn.spawnEntries.push_back(ParseSpawn(entry, stats, movements));
    }

    spawn.periodicSpawnInterval = ReadFloat(value, "periodicSpawnInterval", spawn.periodicSpawnInterval);
    spawn.deathSpawnCount = ReadInt(value, "deathSpawnCount", spawn.deathSpawnCount);
    return spawn;
}

std::string DefaultLevelPath(int levelIndex) {
    std::ostringstream path;
    path << "config/levels/level_" << std::setw(2) << std::setfill('0') << levelIndex << ".json";
    return path.str();
}

std::string LevelPathFromManifest(int levelIndex) {
    try {
        const JsonValue manifest = SimpleJson::ParseFile(
            ResolveConfigPath("config/level_manifest.json"));
        for (const auto& level : manifest.At("levels").AsArray()) {
            if (ReadInt(level, "id", 0) == levelIndex) {
                return level.At("config").AsStringOr(DefaultLevelPath(levelIndex));
            }
        }
    }
    catch (...) {
    }

    return DefaultLevelPath(levelIndex);
}
}

std::vector<EnemyPhaseDefinition> LevelEnemyPool::Create(int levelIndex) {
    std::vector<EnemyPhaseDefinition> phases;

    try {
        const StatsMap stats = LoadStats();
        const MovementMap movements = LoadMovements();
        const JsonValue level = SimpleJson::ParseFile(
            ResolveConfigPath(LevelPathFromManifest(levelIndex)));

        for (const auto& wave : level.At("waves").AsArray()) {
            std::vector<LevelEnemySpawnDefinition> enemies;
            for (const auto& enemy : wave.At("enemies").AsArray()) {
                enemies.push_back(ParseSpawn(enemy, stats, movements));
            }
            phases.emplace_back(ReadFloat(wave, "time", 0.0f), std::move(enemies));
        }
    }
    catch (...) {
    }

    return phases;
}
