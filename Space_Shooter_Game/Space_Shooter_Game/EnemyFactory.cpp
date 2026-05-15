#include "EnemyFactory.h"
#include "MeleeEnemy1.h"
#include "RangeEnemy1.h"
#include "MovementStrategies.h"
#include "AssetManager.h"

std::unique_ptr<BaseEnemy> EnemyFactory::Create(
    EnemyType type, Graphics& gfx, float x, float y)
{
    std::unique_ptr<BaseEnemy> enemy;

    switch (type) {
    case EnemyType::Melee_Basic:
        // HP=100, Speed=80, ATK=10, Range=30
        enemy = std::make_unique<MeleeEnemy1>(x, y, 48.0f, 48.0f, 100.0f, 80.0f, 10.0f, 30.0f);
        enemy->SetMovementStrategy(std::make_unique<ChaseMovement>(80.0f));
        enemy->SetExpReward(10);
        break;

    case EnemyType::Melee_Fast:
        // Nhanh hơn, ít HP hơn, di chuyển zigzag
        enemy = std::make_unique<MeleeEnemy1>(x, y, 36.0f, 36.0f, 50.0f, 150.0f, 5.0f, 20.0f);
        enemy->SetMovementStrategy(std::make_unique<SineWaveMovement>(120.0f, 60.0f, 3.0f));
        enemy->SetExpReward(15);
        break;

    case EnemyType::Ranged_Basic:
        // HP=80, Speed=100, ATK=15
        enemy = std::make_unique<RangeEnemy1>(x, y, 48.0f, 48.0f, 80.0f, 100.0f, 15.0f);
		enemy->SetMovementStrategy(std::make_unique<LinearMovement>(0.0f, 50.0f));
        enemy->SetExpReward(20);
        break;

    // TODO: Thêm các loại enemy mới ở đây
    // case EnemyType::Ranged_Basic:
    //     enemy = std::make_unique<RangedEnemy>(x, y, ...);
    //     enemy->SetMovementStrategy(std::make_unique<LinearMovement>(0.0f, 50.0f));
    //     enemy->SetExpReward(20);
    //     break;

    // case EnemyType::Boss_Stage1:
    //     enemy = std::make_unique<BossEnemy>(x, y, ...);
    //     enemy->SetMovementStrategy(std::make_unique<CircularMovement>(...));
    //     enemy->SetExpReward(100);
    //     break;

    default:
        enemy = std::make_unique<MeleeEnemy1>(x, y, 48.0f, 48.0f, 100.0f, 80.0f, 10.0f, 30.0f);
        enemy->SetMovementStrategy(std::make_unique<ChaseMovement>(80.0f));
        enemy->SetExpReward(10);
        break;
    }

    // Gán sprite/animation cho enemy
    if (type == EnemyType::Melee_Basic || type == EnemyType::Melee_Fast) {
        auto tex = AssetManager::GetInstance().GetTexture(gfx, L"Assets/Enemy/Enemy1_lv1.png");
        enemy->GetAnim().Initialize(tex);
        enemy->GetAnim().AddClip("idle", 0, 0, 128, 128, 1, 1, 1.0f, true);
        enemy->GetAnim().Play("idle");
    }
    else if (type == EnemyType::Ranged_Basic) {
        auto tex = AssetManager::GetInstance().GetTexture(gfx, L"Assets/Enemy/Enemy2_lv1.png");
        enemy->GetAnim().Initialize(tex);
        enemy->GetAnim().AddClip("idle", 0, 0, 128, 128, 1, 1, 1.0f, true);
        enemy->GetAnim().Play("idle");
	}

    return enemy;
}
