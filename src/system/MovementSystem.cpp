#include "MovementSystem.hpp"
#include <iostream>

void MovementSystem::sync(entt::registry &registry, float ms)
{
    // Character Movement Update
    auto view = registry.view<characterComponent, motionComponent, inputKeyboard, inputMouse>();
    for (auto entity : view)
    {
        auto &[position, direction, radians, speed] = view.get<motionComponent>(entity);

        auto &[is_alive] = view.get<characterComponent>(entity);

        auto &[upKeyPressed, downKeyPressed, leftKeyPressed, rightKeyPressed, resetKeyPressed] = view.get<inputKeyboard>(entity);

        auto &[xpos, ypos] = view.get<inputMouse>(entity);

        float step = speed * (ms / 1000);
        vec2 offset = mul(normalize(direction), step);

        if (resetKeyPressed)
        {
            resetCharacter(registry);
        }
        // Set movement for character
        if (is_alive)
        {
            move(position, offset);
            // TODO: Let character rotate
            vec2 characterVector = {0.f, 1.f};
            vec2 mouseVec = {
                (float)xpos - position.x,
                (float)ypos - position.y};

            int factor = 1;
            if (mouseVec.x > 0.f)
                factor = -1;

            float angle = acos(dot(mouseVec, characterVector) / (lengthVec2(mouseVec) * lengthVec2(characterVector)));
            radians = factor * angle;
        }
    }

    // Shield Position Update
    auto viewCharacter = registry.view<characterComponent, motionComponent>();
    auto viewShield = registry.view<shieldComponent, motionComponent, inputMouse>();
    for (auto character : viewCharacter)
    {
        auto &position = viewCharacter.get<motionComponent>(character).position;
        auto &is_alive = viewCharacter.get<characterComponent>(character).is_alive;
        if (is_alive)
        {
            for (auto shield : viewShield)
            {
                auto &radians = viewShield.get<motionComponent>(shield).radians;
                auto &[xpos, ypos] = viewShield.get<inputMouse>(shield);

                vec2 shieldVec = {0.f, 1.f};
                vec2 mouseVec = {
                    (float)xpos - position.x,
                    (float)ypos - position.y};

                int factor = 1;
                if (mouseVec.x > 0.f)
                    factor = -1;

                float angle = acos(dot(mouseVec, shieldVec) / (lengthVec2(mouseVec) * lengthVec2(shieldVec)));
                radians = factor * angle;
            }
        }
    }
}

void MovementSystem::update(entt::registry &registry, Character &m_character, Shield &m_shield)
{
    // Character Movement Update
    auto character = registry.view<characterComponent, motionComponent>();
    for (auto entity : character)
    {
        auto &position = character.get<motionComponent>(entity).position;
        auto &radians = character.get<motionComponent>(entity).radians;

        m_character.set_position(position);
        m_character.set_rotation(radians);
    }

    auto shield = registry.view<shieldComponent, motionComponent>();

    for (auto entity : shield)
    {
        auto &radians = shield.get<motionComponent>(entity).radians;
        m_shield.set_position(m_character.get_position());
        m_shield.set_rotation(radians);
    }
}

void MovementSystem::move(vec2 &pos, vec2 off)
{
    float C_FRAME_X_MAX = 1150;
    float C_FRAME_X_MIN = 50;
    float C_FRAME_Y_MAX = 720;
    float C_FRAME_Y_MIN = 50;

    pos.x += off.x;
    pos.y += off.y;

    if (pos.x > C_FRAME_X_MAX)
    {
        pos.x = C_FRAME_X_MAX;
    }
    else if (pos.x < C_FRAME_X_MIN)
    {
        pos.x = C_FRAME_X_MIN;
    }
    if (pos.y > C_FRAME_Y_MAX)
    {
        pos.y = C_FRAME_Y_MAX;
    }
    else if (pos.y < C_FRAME_Y_MIN)
    {
        pos.y = C_FRAME_Y_MIN;
    }
}

void MovementSystem::setCharacterDead(entt::registry &registry)
{
    auto view = registry.view<characterComponent>();
    for (auto entity : view)
    {
        auto &is_alive = view.get(entity).is_alive;
        is_alive = false;
    }
}

void MovementSystem::resetCharacter(entt::registry &registry)
{
    auto view = registry.view<characterComponent, motionComponent>();
    for (auto entity : view)
    {
        auto &is_alive = view.get<characterComponent>(entity).is_alive;
        is_alive = true;

        auto &position = view.get<motionComponent>(entity).position;
        position = {250.f, 300.f};
    }
}

void MovementSystem::rotate(float &radians, float newRadians)
{
    radians = newRadians;
}

// Calculates the length of a vec2 vector
float MovementSystem::lengthVec2(vec2 v)
{
    return sqrt(pow(v.x, 2.f) + pow(v.y, 2.f));
}
