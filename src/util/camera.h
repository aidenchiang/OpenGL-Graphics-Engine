#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
public:
	// Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera Options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor using vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // Constructor using individual inputs
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Returns the view matrix calculated from the LookAt matrix along with our Euler angles.
    glm::mat4 GetViewMatrix();

    // Process WASD (or similar) movements
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // Processes mouse movements for controlling yaw + patch
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // Processes scroll wheel input to allow for zooming in and out
    void ProcessMouseScroll(float yoffset);

    // Calculates and updates the location, and angle of the Camera
    void updateCameraVectors();
};

#endif