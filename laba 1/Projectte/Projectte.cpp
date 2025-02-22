#include <glew.h>
#include <glfw3.h>
#include <iostream>

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_TRIANGLE_STRIP);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(x + width, y);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(x, y + height);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(x + width, y + height);

    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Rectangle Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawRectangle(-0.5f, -0.5f, 1.0f, 0.5f); // Прямоугольник по центру

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
