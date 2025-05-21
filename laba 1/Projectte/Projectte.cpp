#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include "Shader.h"
#include "Model.h"


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
float fov = 45.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

float OX1Angle = 0.0f;
glm::mat4 OX1Pos = glm::mat4(1.0f);
float OX2Angle = 0.0f;
glm::mat4 OX2Pos = glm::mat4(1.0f);
float OX3Angle = 0.0f;
glm::mat4 OX3Pos = glm::mat4(1.0f);



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

void processInput(GLFWwindow* window) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        OX1Angle -= deltaTime * 50.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        OX1Angle += deltaTime * 50.0f;
    }
    OX1Pos = glm::rotate(glm::mat4(1.0f), glm::radians(OX1Angle), glm::vec3(0, 1, 0));

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        OX2Angle += deltaTime * 0.5f;
        OX2Angle = glm::clamp(OX2Angle, 0.0f, 0.4f); 
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        OX2Angle -= deltaTime * 0.5f;
        OX2Angle = glm::clamp(OX2Angle, 0.0f, 0.4f); 
    }
    OX2Pos = glm::translate(glm::mat4(1.0f), glm::vec3(0, OX2Angle, 0));


    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        OX3Angle += deltaTime * 0.5f;
        OX3Angle = glm::clamp(OX3Angle, -0.05f, 0.3f);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        OX3Angle -= deltaTime * 0.5f;
        OX3Angle = glm::clamp(OX3Angle, -0.05f, 0.3f);
    }
    OX3Pos = glm::translate(glm::mat4(1.0f), glm::vec3(OX3Angle, 0, 0));

}

int main() {
 
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Model Viewer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("vertex_shader.glsl", "fragment_shader.glsl");

    Model ourModel("Lab_3_VAR_10.obj");
    if (ourModel.meshes.empty()) {
        std::cerr << "Failed to load model or model is empty!" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        ourShader.setVec3("material.ambient", glm::vec3(0.2125f, 0.1275f, 0.054f));
        ourShader.setVec3("material.diffuse", glm::vec3(0.714f, 0.4284f, 0.18144f));
        ourShader.setVec3("material.specular", glm::vec3(0.393548f, 0.271906f, 0.166721f));
        ourShader.setFloat("material.shininess", 25.6f);

        ourShader.setVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
        ourShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        ourShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // Позиция камеры
        ourShader.setVec3("viewPos", cameraPos);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Цвет модели
        ourShader.setVec4("ourColor", glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

        // Рисуем модель с передачей матриц преобразований
        ourModel.Draw(ourShader, OX1Pos, OX2Pos, OX3Pos);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
