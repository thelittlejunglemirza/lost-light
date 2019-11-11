#include "levelSystem.hpp"

bool LevelSystem::init_level(entt::registry &registry, int m_lvl_num)
{
    reset_enemy(registry);

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed] = level.get(entity);
        switch (m_lvl_num)
        {
        case 1:
            // minion info
            minion_num = 3;
            minion_count = 0;
            minion_killed = 0;
            minion_max_on_screen = 3;
            minion_init_pos.push_back(init_pos_array[0]);
            minion_init_pos.push_back(init_pos_array[1]);
            minion_init_pos.push_back(init_pos_array[2]);
            minion_is_movable.push_back(false);
            minion_is_movable.push_back(false);
            minion_is_movable.push_back(false);

            // boss info
            boss_num = 1;
            boss_count = 0;
            boss_killed = 0;
            boss_max_on_screen = 1;
            boss_init_pos.push_back(c_boss_init_pos);
            boss_is_movable.push_back(true);

            // global info
            lvl_num = 1;
            enemy_killed = 0;
            enemy_total = minion_num + boss_num;
            enemy_spawn_delay = 0.f;
            next_enemy_spawn_counter = 0.f;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        }
    }

    return true;
}

void LevelSystem::update(entt::registry &registry, float elapsed_ms, vector<Enemy> *m_enemies, vector<Projectile> *m_projectiles)
{
    next_enemy_spawn_counter -= elapsed_ms;

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed] = level.get(entity);
        enemy_killed = minion_killed + boss_killed;

        if (enemy_killed >= enemy_total)
        {
            menuSystem.sync(registry, STATE_WIN);
        }
    }

    auto view = registry.view<inputKeyboard, inputMouse>();
    for (auto entity : view)
    {
        auto &resetKeyPressed = view.get<inputKeyboard>(entity).resetKeyPressed;
        if (resetKeyPressed)
        {
            m_enemies->clear();
            m_projectiles->clear();
            init_level(registry, 1);
        }
    }
}

// Minion functions
vec2 LevelSystem::get_next_minion_pos()
{
    vec2 result = minion_init_pos[0];
    auto init_pos = minion_init_pos.begin();
    minion_init_pos.erase(init_pos);
    return result;
}

bool LevelSystem::get_next_minion_is_movable()
{
    bool result = minion_is_movable[0];
    auto init_pos = minion_is_movable.begin();
    minion_is_movable.erase(init_pos);
    return result;
}

bool LevelSystem::should_spawn_minion(int enemy_size)
{
    if (enemy_size < minion_max_on_screen && next_enemy_spawn_counter < 0.f && minion_count < minion_num)
    {
        minion_count++;
        next_enemy_spawn_counter = enemy_spawn_delay;
        return true;
    }

    return false;
}

// Boss functions
vec2 LevelSystem::get_next_boss_pos()
{
    vec2 result = boss_init_pos[0];
    auto init_pos = boss_init_pos.begin();
    boss_init_pos.erase(init_pos);
    return result;
}

bool LevelSystem::get_next_boss_is_movable()
{
    bool result = boss_is_movable[0];
    auto init_pos = boss_is_movable.begin();
    boss_is_movable.erase(init_pos);
    return result;
}

void LevelSystem::reset_enemy(entt::registry &registry)
{
    minion_init_pos.clear();
    minion_is_movable.clear();
    boss_init_pos.clear();
    boss_is_movable.clear();

    auto viewEnemy = registry.view<enemyComponent, physicsScaleComponent, motionComponent>();

    for (auto entity : viewEnemy)
    {
        auto &is_alive = viewEnemy.get<enemyComponent>(entity).is_alive;
        is_alive = false;
    }

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed] = level.get(entity);
        minion_killed = 0;
        boss_killed = 0;
    }

    auto menu = registry.view<menuComponent>();
    for (auto entity : menu)
    {
        auto &state = menu.get(entity).state;
        state = STATE_PLAYING;
    }
}

bool LevelSystem::should_spawn_boss(entt::registry &registry)
{
    if (enemy_killed == minion_num && boss_count < boss_num)
    {
        auto view = registry.view<potionComponent, physicsScaleComponent>();
        for (auto entity : view)
        {
            auto &id = view.get<potionComponent>(entity).id;
            auto &is_consumed = view.get<potionComponent>(entity).is_consumed;
            auto &scale = view.get<physicsScaleComponent>(entity).scale;
            scale = {0.07f, 0.07f};
            is_consumed = false;
        }
        boss_count++;
        return true;
    }

    return false;
}

// Enemy shared functions
int LevelSystem::get_next_enemy_id()
{
    return enemy_id++;
}

void LevelSystem::increment_enemy_killed(entt::registry &registry)
{
    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed] = level.get(entity);
        minion_killed++;
    }
}
