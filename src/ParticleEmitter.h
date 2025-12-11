#pragma once

#include "ofMain.h"
#include "Particle.h"
#include <vector>

class ParticleEmitter {
public:
    // Constructor
    ParticleEmitter();
    
    // Core methods
    void update(float dt);
    void draw();
    
    // Emission control
    void start();                    // Start continuous emission
    void stop();                     // Stop continuous emission
    void burst(int count);           // Emit a burst of particles
    
    // Configuration
    void setPosition(glm::vec3 pos);
    void setVelocity(glm::vec3 vel);
    void setEmitterType(std::string type);  // "cone", "radial"
    void setLifespan(float min, float max);
    void setVelocityRange(float min, float max);
    void setSize(float size);
    void setColor(ofColor col);
    void setParticleRadius(float radius);
    void setRate(float particlesPerSecond);
    void setOneShot(bool oneshot);
    void setRandomLifespan(bool randomize);
    void setGravity(glm::vec3 grav);
    void setDirection(glm::vec3 dir);        // Direction for cone emitter
    void setConeAngle(float angle);          // Spread angle for cone emitter
    
    // Status
    bool isEmitting() const { return emitting; }
    int getParticleCount() const;
    
private:
    std::vector<Particle> particles;
    
    // Emitter state
    bool emitting;
    bool oneShot;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 gravity;
    glm::vec3 direction;      // Emission direction for cone
    float coneAngle;          // Spread angle in degrees
    
    // Particle properties
    float lifespanMin;
    float lifespanMax;
    float velocityMin;
    float velocityMax;
    float particleSize;
    float particleRadius;     // Spawn radius
    ofColor particleColor;
    bool randomLifespan;
    
    // Emission rate
    float rate;               // Particles per second
    float accumulator;        // Time accumulator for emission
    
    // Emitter type
    std::string emitterType;  // "cone" or "radial"
    
    // Helper methods
    void spawnParticle();
    glm::vec3 randomDirectionInCone();
    glm::vec3 randomRadialDirection();
};

