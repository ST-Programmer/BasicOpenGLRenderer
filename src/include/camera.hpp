#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderprogram.hpp"

class Camera
{
public:
    Camera(GLFWwindow* window, int width, int height, glm::vec3 pos)
        : m_Width(width), m_Height(height), m_Pos(pos)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void CreateMatrix(float fov, float nearPlane, float farPlane)
    {
        glm::mat4 view = glm::lookAt(m_Pos, m_Pos + m_Orientation, m_Up);

        glm::mat4 projection = glm::perspective(
            glm::radians(fov), static_cast<float>(m_Width) / static_cast<float>(m_Height),
            nearPlane, farPlane);

        m_CameraMatrix = projection * view;
    }

    void UploadMatrix(const ShaderProgram& shaderProgram, const char* uniformName) const
    {
        shaderProgram.SetMat4(uniformName, m_CameraMatrix);
    }

    void Movement(GLFWwindow* window, float deltaTime)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = m_MouseMovementEnabled ? (io.ConfigFlags | ImGuiConfigFlags_NoMouse)
                                                : (io.ConfigFlags & ~ImGuiConfigFlags_NoMouse);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            if (!m_TabPressedLastFrame)
            {
                m_MouseMovementEnabled = !m_MouseMovementEnabled;

                glfwSetInputMode(window, GLFW_CURSOR,
                                 m_MouseMovementEnabled ? GLFW_CURSOR_DISABLED
                                                        : GLFW_CURSOR_NORMAL);

                m_FirstMouseMovement = true;
                m_TabPressedLastFrame = true;
            }
        }
        else
        {
            m_TabPressedLastFrame = false;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Pos += m_Orientation * m_Speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Pos -= m_Orientation * m_Speed * deltaTime;

        glm::vec3 right = glm::normalize(glm::cross(m_Orientation, m_Up));

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Pos -= right * m_Speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Pos += right * m_Speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            m_Pos += m_Up * m_Speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            m_Pos -= m_Up * m_Speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS)
            m_Speed = m_SprintSpeed;
        else
            m_Speed = m_WalkSpeed;

        if (m_MouseMovementEnabled)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (m_FirstMouseMovement)
            {
                m_LastMouseX = mouseX;
                m_LastMouseY = mouseY;
                m_FirstMouseMovement = false;
                return;
            }

            float deltaX = static_cast<float>(mouseX - m_LastMouseX);
            float deltaY = static_cast<float>(mouseY - m_LastMouseY);

            m_LastMouseX = mouseX;
            m_LastMouseY = mouseY;

            float rotX = deltaY * m_Sensitivity * deltaTime;
            float rotY = deltaX * m_Sensitivity * deltaTime;

            glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX),
                                                   glm::normalize(glm::cross(m_Orientation, m_Up)));

            if (!(glm::angle(newOrientation, m_Up) <= glm::radians(5.0f) ||
                  glm::angle(newOrientation, -m_Up) <= glm::radians(5.0f)))
            {
                m_Orientation = newOrientation;
            }

            m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
        }
    }

    void printInfo()
    {
        std::cout << "Position: " << m_Pos.x << " " << m_Pos.y << " " << m_Pos.z << '\n';

        std::cout << "Orientation: " << m_Orientation.x << " " << m_Orientation.y << " "
                  << m_Orientation.z << '\n';
    }

    glm::vec3 getPos() const
    {
        return m_Pos;
    }

private:
    bool m_MouseMovementEnabled = true;
    bool m_FirstMouseMovement = true;
    bool m_TabPressedLastFrame = false;

    int m_Width;
    int m_Height;

    float m_WalkSpeed = 2.0f;
    float m_SprintSpeed = 4.0f;
    float m_Speed = m_WalkSpeed;

    float m_Sensitivity = 100.0f;

    double m_LastMouseX = 0.0;
    double m_LastMouseY = 0.0;

    glm::vec3 m_Pos;
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_CameraMatrix = glm::mat4(1.0f);
};
