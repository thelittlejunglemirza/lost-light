#pragma once

#include "common.hpp"
#include "entity.hpp"
#include <vector>

// class Turtle;
// class Fish;
class Projectile;

class Shield : public Entity {
    static Texture shield_texture;

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

    // Collision routines for turtles and fish
    // bool collides_with(const Turtle& turtle);
    // bool collides_with(const Fish& fish);

    // Returns the current salmon position
    vec2 get_position() const;

    void set_position(vec2 position);

    // Set salmon rotation in radians
    void set_rotation(float radians);

    vec2 getDirection();

    void show(float x, float y);

    void hide();

    void sizeMultiplier(float x, float y);

    vec2 get_bounding_box() const;

    bool collides_with(const Projectile& projectile);

    float triangleArea(vec2 p1, vec2 p2, vec2 p3);

    float trianglesArea(vec2 p1, vec2 p2, vec2 p3, vec2 p4, vec2 projectileP);

    float squareArea(vec2 p1, vec2 p2, vec2 p3);

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
