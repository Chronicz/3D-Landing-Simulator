#include "Lander.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Lander::Lander() {
    position = glm::vec3(0, 0, 0);
    velocity = glm::vec3(0, 0, 0);
    acceleration = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);  // Y-axis is up
    yaw = 0.0f;               // Start facing forward (along Z-axis)
    mass = 500.0f;           // 1000 kg (reasonable for a lander)
    
    // Optimized thrust force: mass * 1.1 * gravity for smooth hover/ascend
    // For mass=1000kg, Moon gravity=1.62 m/s²: F = 1000 * 1.1 * 1.62 = 1782 N
    // This allows hovering with slight upward capability
    thrustForce = mass * 1.8f * std::abs(MOON_GRAVITY);
    
    // Horizontal movement force (lateral thrust) - 60% of vertical thrust for responsive but smooth movement
    horizontalForce = thrustForce * 0.6f;
    
    rotationSpeed = 60.0f;    // 60 degrees per second for smooth rotation
    bExtremeForces = false;   // Set to true for testing (100x forces)
    bDisableCollisionClamping = false;  // Set to true to disable collision velocity clamping
    
    // Configure thrust particle emitter
    thrustEmitter.setEmitterType("cone");
    thrustEmitter.setDirection(glm::vec3(0, -1, 0));  // Downward
    thrustEmitter.setConeAngle(25.0f);
    thrustEmitter.setVelocityRange(3.0f, 6.0f);
    thrustEmitter.setLifespan(0.3f, 0.8f);
    thrustEmitter.setSize(0.15f);
    thrustEmitter.setColor(ofColor(255, 150, 50, 255));  // Orange/yellow exhaust
    thrustEmitter.setRate(100.0f);  // 100 particles per second
    thrustEmitter.setParticleRadius(0.3f);
    thrustEmitter.setGravity(glm::vec3(0, 0, 0));  // No gravity for thrust particles
}

void Lander::initialize(glm::vec3 startPosition) {
    position = startPosition;
    velocity = glm::vec3(0, 0, 0);
    acceleration = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    yaw = 0.0f;
}

void Lander::reset() {
    velocity = glm::vec3(0, 0, 0);
    acceleration = glm::vec3(0, 0, 0);
}

void Lander::update(float dt, bool moveForward, bool moveBack, bool moveLeft, bool moveRight, bool thrustUp, bool rotateCCW, bool rotateCW) {
    // Moon gravity (always downward)
    acceleration = glm::vec3(0, MOON_GRAVITY, 0);
    
    // Calculate local forward and right vectors based on yaw
    float yawRad = glm::radians(yaw);
    glm::vec3 localForward = glm::normalize(glm::vec3(std::sin(yawRad), 0, std::cos(yawRad)));
    glm::vec3 localRight = glm::normalize(glm::cross(localForward, up));
    
    // Calculate force magnitudes: F*dt/m gives velocity change
    float actualThrustForce = bExtremeForces ? (thrustForce * 100.0f) : thrustForce;
    float actualHorizontalForce = bExtremeForces ? (horizontalForce * 100.0f) : horizontalForce;
    
    float verticalForceMagnitude = actualThrustForce * dt / mass;
    float horizontalForceMagnitude = actualHorizontalForce * dt / mass;
    
    // Apply thrust forces based on input
    // W/S swapped to match expected forward/back behavior
    if (moveForward) {
        velocity -= localForward * horizontalForceMagnitude;  // W key - move forward (negative direction)
    }
    if (moveBack) {
        velocity += localForward * horizontalForceMagnitude;  // S key - move backward (positive direction)
    }
    if (moveLeft) {
        velocity += localRight * horizontalForceMagnitude;    // Swapped: was -=
    }
    if (moveRight) {
        velocity -= localRight * horizontalForceMagnitude;    // Swapped: was +=
    }
    if (thrustUp) {
        velocity += up * verticalForceMagnitude;
        
        // Start thrust particle emitter
        if (!thrustEmitter.isEmitting()) {
            thrustEmitter.start();
        }
    } else {
        // Stop thrust particle emitter when not thrusting
        if (thrustEmitter.isEmitting()) {
            thrustEmitter.stop();
        }
    }
    
    // Apply rotation
    if (rotateCCW) {
        yaw += rotationSpeed * dt;
    }
    if (rotateCW) {
        yaw -= rotationSpeed * dt;
    }
    
    // Normalize yaw to 0-360 range
    while (yaw < 0) yaw += 360.0f;
    while (yaw >= 360.0f) yaw -= 360.0f;
    
    // Integrate velocity (apply gravity and other accelerations)
    velocity += acceleration * dt;
    
    // Integrate position
    position += velocity * dt;
}

void Lander::updateParticles(float dt) {
    // Update emitter position to follow lander
    thrustEmitter.setPosition(position);
    
    // Update particle system
    thrustEmitter.update(dt);
}

void Lander::drawParticles() {
    thrustEmitter.draw();
}

void Lander::triggerExplosion() {
    // Stop thrust emitter if running
    thrustEmitter.stop();
    
    // Configure for explosion
    thrustEmitter.setEmitterType("radial");
    thrustEmitter.setVelocityRange(5.0f, 15.0f);
    thrustEmitter.setLifespan(0.5f, 1.5f);
    thrustEmitter.setSize(0.2f);
    thrustEmitter.setColor(ofColor(255, 100, 0, 255));  // Bright orange/red
    thrustEmitter.setGravity(glm::vec3(0, MOON_GRAVITY, 0));  // Apply gravity to explosion
    thrustEmitter.setPosition(position);
    
    // Emit burst of particles
    thrustEmitter.burst(200);  // 200 particles for explosion
    
    // Reconfigure back to thrust mode for next time
    thrustEmitter.setEmitterType("cone");
    thrustEmitter.setDirection(glm::vec3(0, -1, 0));
    thrustEmitter.setConeAngle(25.0f);
    thrustEmitter.setVelocityRange(3.0f, 6.0f);
    thrustEmitter.setLifespan(0.3f, 0.8f);
    thrustEmitter.setSize(0.15f);
    thrustEmitter.setColor(ofColor(255, 150, 50, 255));
    thrustEmitter.setGravity(glm::vec3(0, 0, 0));
}
