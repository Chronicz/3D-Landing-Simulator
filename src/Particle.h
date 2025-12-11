#pragma once

#include "ofMain.h"

// Simple particle structure
class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    ofColor color;
    float age;           // Current age in seconds
    float lifespan;      // Total lifespan in seconds
    float size;          // Particle size
    bool alive;
    
    Particle() {
        position = glm::vec3(0, 0, 0);
        velocity = glm::vec3(0, 0, 0);
        acceleration = glm::vec3(0, 0, 0);
        color = ofColor(255, 255, 255, 255);
        age = 0.0f;
        lifespan = 1.0f;
        size = 1.0f;
        alive = false;
    }
    
    void update(float dt) {
        if (!alive) return;
        
        // Update physics
        velocity += acceleration * dt;
        position += velocity * dt;
        age += dt;
        
        // Check if particle should die
        if (age >= lifespan) {
            alive = false;
        }
    }
    
    void draw() {
        if (!alive) return;
        
        // Calculate alpha based on age (fade out)
        float lifeFraction = age / lifespan;
        float alpha = ofMap(lifeFraction, 0.0f, 1.0f, 255.0f, 0.0f, true);
        
        ofSetColor(color.r, color.g, color.b, (int)alpha);
        ofDrawSphere(position, size);
    }
    
    float getAge() const { return age; }
    float getLifespan() const { return lifespan; }
    bool isAlive() const { return alive; }
};

