#include "SpaceLander.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

SpaceLander::SpaceLander() {
	// Initialize default values
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	rotation = glm::quat(1, 0, 0, 0);  // Identity quaternion
	angularVelocity = glm::vec3(0, 0, 0);
	angularAccel = glm::vec3(0, 0, 0);

	mass = 1000.0f;  // 1000 kg default
	fuel = 100.0f;
	maxFuel = 100.0f;
	dryMass = 900.0f;  // 900 kg dry mass

	thrustForce = glm::vec3(0, 0, 0);
	rcsTorque = glm::vec3(0, 0, 0);
	gravityForce = glm::vec3(0, 0, 0);
	turbulenceForce = glm::vec3(0, 0, 0);

	mainThrustPower = 10000.0f;  // 10000 N main engine (stronger for moon gravity)
	rcsThrustPower = 20.0f;      // 20 N⋅m RCS torque (much lower for controlled rotation)
	fuelConsumptionRate = 10.0f; // 10 kg/s at full thrust

	thrustInput = 0.0f;
	pitchInput = 0.0f;
	yawInput = 0.0f;
	rollInput = 0.0f;
	lateralInput = 0.0f;
	verticalInput = 0.0f;
	forwardInput = 0.0f;

	// Moon gravity (1.62 m/s^2) - much weaker than Earth
	gravity = glm::vec3(0, -1.62f, 0);
	turbulenceStrength = 50.0f;  // Turbulence force strength
	damping = 0.1f;              // Linear damping
	angularDamping = 2.0f;       // Angular damping (higher to prevent excessive spinning)
}

SpaceLander::~SpaceLander() {
}

void SpaceLander::setup(glm::vec3 startPos, float startMass, float startFuel) {
	position = startPos;
	mass = startMass;
	fuel = startFuel;
	maxFuel = startFuel;
	dryMass = startMass - startFuel;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	rotation = glm::quat(1, 0, 0, 0);  // Identity quaternion (no rotation)
	angularVelocity = glm::vec3(0, 0, 0);
	angularAccel = glm::vec3(0, 0, 0);
	forwardInput = 0.0f;
}

void SpaceLander::update(float deltaTime) {
	// Update mass based on fuel consumption
	mass = dryMass + fuel;

	// Apply all forces
	applyForces();

	// Integrate physics
	integratePhysics(deltaTime);

	// Consume fuel
	consumeFuel(deltaTime);
}

void SpaceLander::integratePhysics(float deltaTime) {
	// Update angular velocity from angular acceleration
	angularVelocity += angularAccel * deltaTime;
	
	// Apply angular damping (clamp to prevent negative values)
	float dampingFactor = glm::clamp(1.0f - angularDamping * deltaTime, 0.0f, 1.0f);
	angularVelocity *= dampingFactor;
	
	// Cap maximum angular velocity to prevent excessive spinning (max 2 rad/s ≈ 115 deg/s)
	float maxAngularVel = 2.0f;
	float angularVelMagnitude = glm::length(angularVelocity);
	if (angularVelMagnitude > maxAngularVel) {
		angularVelocity = (angularVelocity / angularVelMagnitude) * maxAngularVel;
	}

	// Update rotation quaternion from angular velocity
	// Angular velocity is in world space, convert to local space
	glm::vec3 localAngVel = glm::inverse(rotation) * angularVelocity;
	glm::quat rotationDelta = glm::quat(0, localAngVel.x, localAngVel.y, localAngVel.z) * rotation * 0.5f;
	rotation += rotationDelta * deltaTime;
	rotation = glm::normalize(rotation);

	// Update linear velocity from acceleration
	velocity += acceleration * deltaTime;

	// Apply linear damping
	velocity *= (1.0f - damping * deltaTime);

	// Update position from velocity
	position += velocity * deltaTime;

	// Reset acceleration for next frame
	acceleration = glm::vec3(0, 0, 0);
	angularAccel = glm::vec3(0, 0, 0);
}

void SpaceLander::applyForces() {
	// Reset forces
	thrustForce = glm::vec3(0, 0, 0);
	rcsTorque = glm::vec3(0, 0, 0);
	gravityForce = glm::vec3(0, 0, 0);
	turbulenceForce = glm::vec3(0, 0, 0);

	// Apply all force types
	applyGravity();
	applyThrust();
	applyRCS();
	applyTurbulence();

	// Calculate total acceleration from forces
	// F = ma, so a = F/m
	if (mass > 0.0f) {
		glm::vec3 totalForce = thrustForce + gravityForce + turbulenceForce;
		acceleration = totalForce / mass;
	}

	// Calculate angular acceleration from torque
	// τ = Iα, simplified: α = τ (assuming uniform mass distribution)
	angularAccel = rcsTorque;
}

void SpaceLander::applyThrust() {
	if (fuel <= 0.0f || thrustInput <= 0.0f) {
		return;
	}

	// Main thruster points upward (in local space, +Y) to push lander up
	// Since the lander model is flipped 180 degrees, thrust should be positive Y
	glm::vec3 localThrustDir = glm::vec3(0, 1, 0);
	
	// Apply lateral, vertical, and forward inputs to main thruster direction
	// Lateral: left/right (local X axis)
	localThrustDir.x += lateralInput * 0.3f;  // Allow some lateral component
	// Forward/backward: forward/back (local Z axis)
	localThrustDir.z += forwardInput * 0.3f;  // Allow forward/backward component
	// Vertical: up/down (local Y axis) - adjust thrust direction
	// Ensure Y component stays positive (upward) for main thruster
	localThrustDir.y = glm::max(0.1f, localThrustDir.y + verticalInput * 0.3f);
	
	// Normalize the thrust direction
	localThrustDir = glm::normalize(localThrustDir);

	// Convert to world space using rotation
	glm::vec3 worldThrustDir = rotation * localThrustDir;

	// Calculate thrust force magnitude
	float thrustMagnitude = mainThrustPower * thrustInput;
	thrustForce = worldThrustDir * thrustMagnitude;
}

void SpaceLander::applyRCS() {
	if (fuel <= 0.0f) {
		return;
	}

	// RCS provides rotational control
	// Pitch: rotation around local X axis (nose up/down)
	// Yaw: rotation around local Y axis (left/right turn)
	// Roll: rotation around local Z axis (banking)

	// Calculate RCS torque in local space
	glm::vec3 localTorque(
		pitchInput * rcsThrustPower,   // Pitch
		yawInput * rcsThrustPower,     // Yaw
		rollInput * rcsThrustPower     // Roll
	);

	// Convert to world space (torque is a vector, so we rotate it)
	rcsTorque = rotation * localTorque;
}

void SpaceLander::applyGravity() {
	// Gravity always points downward in world space
	gravityForce = gravity * mass;
}

void SpaceLander::applyTurbulence() {
	// Generate random turbulence force
	// This simulates atmospheric effects or random disturbances
	float time = ofGetElapsedTimef();
	
	// Use Perlin-like noise or simple sine waves for turbulence
	float turbX = sin(time * 0.5f + position.x * 0.1f) * turbulenceStrength;
	float turbY = sin(time * 0.7f + position.y * 0.1f) * turbulenceStrength * 0.5f;
	float turbZ = sin(time * 0.6f + position.z * 0.1f) * turbulenceStrength;
	
	turbulenceForce = glm::vec3(turbX, turbY, turbZ);
}

void SpaceLander::consumeFuel(float deltaTime) {
	if (fuel <= 0.0f) {
		fuel = 0.0f;
		return;
	}

	// Consume fuel based on thrust input
	float consumption = fuelConsumptionRate * thrustInput * deltaTime;
	
	// Also consume fuel for RCS usage
	float rcsConsumption = fuelConsumptionRate * 0.1f * 
		(abs(pitchInput) + abs(yawInput) + abs(rollInput) + 
		 abs(lateralInput) + abs(verticalInput)) * deltaTime;
	
	fuel -= (consumption + rcsConsumption);
	if (fuel < 0.0f) {
		fuel = 0.0f;
	}
}

glm::vec3 SpaceLander::getForward() const {
	// Forward is typically -Z in OpenGL, but depends on model orientation
	return rotation * glm::vec3(0, 0, -1);
}

glm::vec3 SpaceLander::getRight() const {
	return rotation * glm::vec3(1, 0, 0);
}

glm::vec3 SpaceLander::getUp() const {
	return rotation * glm::vec3(0, 1, 0);
}

void SpaceLander::setThrust(float value) {
	thrustInput = glm::clamp(value, 0.0f, 1.0f);
}

void SpaceLander::setPitch(float value) {
	pitchInput = glm::clamp(value, -1.0f, 1.0f);
}

void SpaceLander::setYaw(float value) {
	yawInput = glm::clamp(value, -1.0f, 1.0f);
}

void SpaceLander::setRoll(float value) {
	rollInput = glm::clamp(value, -1.0f, 1.0f);
}

void SpaceLander::setLateral(float value) {
	lateralInput = glm::clamp(value, -1.0f, 1.0f);
}

void SpaceLander::setVertical(float value) {
	verticalInput = glm::clamp(value, -1.0f, 1.0f);
}

void SpaceLander::setForward(float value) {
	forwardInput = glm::clamp(value, -1.0f, 1.0f);
}

void SpaceLander::reset(glm::vec3 newPos) {
	position = newPos;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	rotation = glm::quat(1, 0, 0, 0);
	angularVelocity = glm::vec3(0, 0, 0);
	angularAccel = glm::vec3(0, 0, 0);
	thrustInput = 0.0f;
	pitchInput = 0.0f;
	yawInput = 0.0f;
	rollInput = 0.0f;
	lateralInput = 0.0f;
	verticalInput = 0.0f;
}

glm::mat4 SpaceLander::getTransformMatrix() const {
	// Create transformation matrix from position and rotation
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMat = glm::mat4_cast(rotation);
	return translation * rotationMat;
}

