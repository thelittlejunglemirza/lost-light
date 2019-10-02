#pragma once

#include "common.hpp"
#include "missile.hpp"
#include <vector>

// class Turtle;
// class Fish;
//class Projectile;

class Enemy : public Entity {
    static Texture enemy_texture;
    const int MAX_Missile = 5;

public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all associated resources
    void destroy();

    // Update salmon position based on direction
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the salmon
    void draw(const mat3& projection) override;

    // Returns the current salmon position
    vec2 get_position() const;

    // Moves the salmon's position by the specified offset
    void move(vec2 off);

    // Set salmon rotation in radians
    void set_rotation(float radians);

    // True if the salmon is alive
    bool is_alive() const;

    // Kills the salmon, changing its alive state and triggering on death events
    void kill();

    vec2 get_bounding_box() const;

    void shootMissile();

    std::vector<Missile> m_missiles;

private:
    float m_light_up_countdown_ms; // Used to keep track for how long the salmon should be lit up
    bool m_is_alive; // True if the salmon is alive=

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
