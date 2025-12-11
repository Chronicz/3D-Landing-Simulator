#include "ParticleEmitter.h"
#include <cmath>

ParticleEmitter::ParticleEmitter() {
    emitting = false;
    oneShot = false;
    position = glm::vec3(0, 0, 0);
    velocity = glm::vec3(0, 0, 0);
    gravity = glm::vec3(0, -1.62f, 0);  // Moon gravity
    direction = glm::vec3(0, -1, 0);    // Default: downward
    coneAngle = 15.0f;                  // 15 degree cone
    
    lifespanMin = 0.5f;
    lifespanMax = 1.5f;
    velocityMin = 2.0f;
    velocityMax = 5.0f;
    particleSize = 0.1f;
    particleRadius = 0.2f;
    particleColor = ofColor(255, 150, 0, 255);  // Orange
    randomLifespan = true;
    
    rate = 50.0f;  // 50 particles per second
    accumulator = 0.0f;
    
    emitterType = "cone";
    
    // Pre-allocate particle pool
    particles.reserve(1000);
}

void ParticleEmitter::update(float dt) {
    // Update existing particles
    for (auto& p : particles) {
        if (p.isAlive()) {
            p.update(dt);
        }
    }
    
    // Emit new particles if emitting
    if (emitting) {
        accumulator += dt;
        float emitInterval = 1.0f / rate;
        
        while (accumulator >= emitInterval) {
            spawnParticle();
            accumulator -= emitInterval;
            
            // If one-shot, stop after first emission cycle
            if (oneShot) {
                emitting = false;
                break;
            }
        }
    }
}

void ParticleEmitter::draw() {
    ofPushStyle();
    ofEnableAlphaBlending();
    ofDisableLighting();
    
    for (auto& p : particles) {
        if (p.isAlive()) {
            p.draw();
        }
    }
    
    ofEnableLighting();
    ofDisableAlphaBlending();
    ofPopStyle();
}

void ParticleEmitter::start() {
    emitting = true;
    oneShot = false;
    accumulator = 0.0f;
}

void ParticleEmitter::stop() {
    emitting = false;
}

void ParticleEmitter::burst(int count) {
    for (int i = 0; i < count; i++) {
        spawnParticle();
    }
}

void ParticleEmitter::setPosition(glm::vec3 pos) {
    position = pos;
}

void ParticleEmitter::setVelocity(glm::vec3 vel) {
    velocity = vel;
}

void ParticleEmitter::setEmitterType(std::string type) {
    emitterType = type;
}

void ParticleEmitter::setLifespan(float min, float max) {
    lifespanMin = min;
    lifespanMax = max;
}

void ParticleEmitter::setVelocityRange(float min, float max) {
    velocityMin = min;
    velocityMax = max;
}

void ParticleEmitter::setSize(float size) {
    particleSize = size;
}

void ParticleEmitter::setColor(ofColor col) {
    particleColor = col;
}

void ParticleEmitter::setParticleRadius(float radius) {
    particleRadius = radius;
}

void ParticleEmitter::setRate(float particlesPerSecond) {
    rate = particlesPerSecond;
}

void ParticleEmitter::setOneShot(bool oneshot) {
    oneShot = oneshot;
}

void ParticleEmitter::setRandomLifespan(bool randomize) {
    randomLifespan = randomize;
}

void ParticleEmitter::setGravity(glm::vec3 grav) {
    gravity = grav;
}

void ParticleEmitter::setDirection(glm::vec3 dir) {
    direction = glm::normalize(dir);
}

void ParticleEmitter::setConeAngle(float angle) {
    coneAngle = angle;
}

int ParticleEmitter::getParticleCount() const {
    int count = 0;
    for (const auto& p : particles) {
        if (p.isAlive()) count++;
    }
    return count;
}

void ParticleEmitter::spawnParticle() {
    // Find dead particle or create new one
    Particle* p = nullptr;
    for (auto& particle : particles) {
        if (!particle.isAlive()) {
            p = &particle;
            break;
        }
    }
    
    if (p == nullptr) {
        particles.push_back(Particle());
        p = &particles.back();
    }
    
    // Initialize particle
    p->alive = true;
    p->age = 0.0f;
    
    // Random position within spawn radius
    glm::vec3 randomOffset = glm::vec3(
        ofRandom(-particleRadius, particleRadius),
        ofRandom(-particleRadius, particleRadius),
        ofRandom(-particleRadius, particleRadius)
    );
    p->position = position + randomOffset;
    
    // Determine velocity based on emitter type
    glm::vec3 particleDirection;
    if (emitterType == "radial") {
        particleDirection = randomRadialDirection();
    } else {
        particleDirection = randomDirectionInCone();
    }
    
    float speed = ofRandom(velocityMin, velocityMax);
    p->velocity = velocity + particleDirection * speed;
    
    // Set acceleration (gravity)
    p->acceleration = gravity;
    
    // Set lifespan
    if (randomLifespan) {
        p->lifespan = ofRandom(lifespanMin, lifespanMax);
    } else {
        p->lifespan = lifespanMin;
    }
    
    // Set appearance
    p->size = particleSize;
    p->color = particleColor;
}

glm::vec3 ParticleEmitter::randomDirectionInCone() {
    // Generate random direction within cone
    float angleRad = glm::radians(coneAngle);
    
    // Random angle around the cone axis
    float theta = ofRandom(0, 2.0f * PI);
    
    // Random angle from cone axis (weighted toward center)
    float phi = ofRandom(0, angleRad);
    
    // Create direction vector in local space (assuming direction is the axis)
    // We need to rotate around the direction vector
    
    // Simple approach: perturb the direction vector
    glm::vec3 perpendicular;
    if (std::abs(direction.y) < 0.9f) {
        perpendicular = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
    } else {
        perpendicular = glm::normalize(glm::cross(direction, glm::vec3(1, 0, 0)));
    }
    glm::vec3 perpendicular2 = glm::normalize(glm::cross(direction, perpendicular));
    
    // Combine to get random direction in cone
    float x = std::sin(phi) * std::cos(theta);
    float y = std::sin(phi) * std::sin(theta);
    float z = std::cos(phi);
    
    glm::vec3 result = direction * z + perpendicular * x + perpendicular2 * y;
    return glm::normalize(result);
}

glm::vec3 ParticleEmitter::randomRadialDirection() {
    // Generate random direction in all directions (sphere)
    float theta = ofRandom(0, 2.0f * PI);
    float phi = ofRandom(0, PI);
    
    float x = std::sin(phi) * std::cos(theta);
    float y = std::sin(phi) * std::sin(theta);
    float z = std::cos(phi);
    
    return glm::vec3(x, y, z);
}

