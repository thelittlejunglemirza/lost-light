#pragma once

// internal
#include "character.hpp"
#include "common.hpp"
#include "enemy.hpp"
#include "shield.hpp"

// #include "turtle.hpp"
// #include "fish.hpp"
#include "./system/InputSystem.hpp"
#include "./system/PhysicsSystem.hpp"
#include "./system/ShieldSystem.hpp"
#include "./system/enemyAISystem.hpp"
#include "background.hpp"
#include "factory.hpp"
#include "potion.hpp"
#include "projectile.hpp"

// entt
#include <entt/entity/registry.hpp>

// stlib
#include <random>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class World {
public:
    World();
    ~World();

    // Creates a window, sets up events and begins the game
    bool init(vec2 screen);

    // Releases all associated resources
    void destroy();

    // Steps the game ahead by ms milliseconds
    bool update(float ms);

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over() const;

private:
    // Generates a new turtle
    bool spawn_projectile();

    // !!! INPUT CALLBACK FUNCTIONS
    void on_key(GLFWwindow*, int key, int, int action, int mod);
    void on_mouse_move(GLFWwindow* window, double xpos, double ypos);
    // Calculates the length of a vec2 vector
    float lengthVec2(vec2 v);

private:
    // Window handle
    GLFWwindow* m_window;
    float m_screen_scale; // Screen to pixel coordinates scale factor

    // Screen texture
    // The draw loop first renders to this texture, then it is used for the background shader
    GLuint m_frame_buffer;
    Texture m_screen_tex;

    // Background effect
    Background m_background;

    // Number of fish eaten by the salmon, displayed in the window title
    unsigned int m_points;

    // Game entities
    Character m_character;
    Shield m_shield;
    Enemy m_enemy;
    Potion m_potion;
    std::vector<Projectile> m_projectiles;

    float m_current_speed;
    float m_next_projectile_spawn;

    Mix_Music* m_background_music;
    Mix_Chunk* m_salmon_dead_sound;
    Mix_Chunk* m_salmon_eat_sound;

    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1

    InputSystem inputSystem;
    PhysicsSystem physicsSystem;
    ShieldSystem shieldSystem;
    EnemyAISystem enemyAI;
    entt::registry registry;
};
