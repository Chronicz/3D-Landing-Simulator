#pragma once

#include "ofMain.h"
#include <glm/glm.hpp>

class Lander {
public:
    // Physics state
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 up;          // local UP axis
    float yaw;             // rotation around UP axis (in degrees)
    float mass;            // mass for physics calculations
    float thrustForce;     // magnitude of vertical thrust
    float horizontalForce; // magnitude of horizontal/lateral thrust
    float rotationSpeed;   // rotation speed in degrees per second
    
    // Debug/test flags
    bool bExtremeForces;   // Use 100x forces for testing
    bool bDisableCollisionClamping;  // Disable collision velocity clamping for testing
    
    // Moon physics constants
    static constexpr float MOON_GRAVITY = -1.62f;  // m/s² downward
    
    // Constructor
    Lander();
    
    // Initialize lander at a specific position
    void initialize(glm::vec3 startPosition);
    
    // Physics update function
    void update(float dt, bool moveForward, bool moveBack, bool moveLeft, bool moveRight, bool thrustUp, bool rotateCCW, bool rotateCW);
    
    // Reset physics state
    void reset();
};

