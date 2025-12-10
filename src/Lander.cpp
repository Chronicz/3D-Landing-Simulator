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
