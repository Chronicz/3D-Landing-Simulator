#pragma once

#include "ofMain.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class SpaceLander {
public:
	SpaceLander();
	~SpaceLander();

	// Physics state
	glm::vec3 position;          // Position in world space
	glm::vec3 velocity;          // Linear velocity
	glm::vec3 acceleration;      // Linear acceleration
	glm::quat rotation;          // Rotation as quaternion
	glm::vec3 angularVelocity;   // Angular velocity (rad/s)
	glm::vec3 angularAccel;      // Angular acceleration

	// Physical properties
	float mass;                  // Mass of the lander (kg)
	float fuel;                  // Current fuel amount
	float maxFuel;               // Maximum fuel capacity
	float dryMass;               // Mass without fuel

	// Forces and torques
	glm::vec3 thrustForce;       // Current thrust force vector
	glm::vec3 rcsTorque;         // Reaction Control System torque
	glm::vec3 gravityForce;      // Gravity force
	glm::vec3 turbulenceForce;   // Turbulence/wind force

	// Thrust parameters
	float mainThrustPower;       // Main engine thrust power
	float rcsThrustPower;        // RCS thruster power
	float fuelConsumptionRate;   // Fuel consumption per second at full thrust

	// Control inputs (normalized -1 to 1)
	float thrustInput;           // Main thruster input (0 to 1)
	float pitchInput;            // Pitch control (-1 to 1)
	float yawInput;              // Yaw control (-1 to 1)
	float rollInput;             // Roll control (-1 to 1)
	float lateralInput;          // Lateral (left/right) input (-1 to 1)
	float verticalInput;         // Vertical (up/down) input (-1 to 1)
	float forwardInput;          // Forward/backward input (-1 to 1)

	// Physics constants
	glm::vec3 gravity;           // Gravity vector (typically (0, -9.81, 0) or moon gravity)
	float turbulenceStrength;    // Strength of turbulence forces
	float damping;               // Linear damping coefficient
	float angularDamping;        // Angular damping coefficient

	// Methods
	void setup(glm::vec3 startPos, float startMass, float startFuel);
	void update(float deltaTime);
	void integratePhysics(float deltaTime);
	void applyForces();
	void applyThrust();
	void applyRCS();
	void applyGravity();
	void applyTurbulence();
	void consumeFuel(float deltaTime);
	
	// Getters for orientation vectors
	glm::vec3 getForward() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;

	// Control methods
	void setThrust(float value);      // 0 to 1
	void setPitch(float value);       // -1 to 1
	void setYaw(float value);         // -1 to 1
	void setRoll(float value);        // -1 to 1
	void setLateral(float value);     // -1 to 1
	void setVertical(float value);    // -1 to 1
	void setForward(float value);     // -1 to 1

	// Reset
	void reset(glm::vec3 newPos);
	
	// Get transform matrix for rendering
	glm::mat4 getTransformMatrix() const;
};

