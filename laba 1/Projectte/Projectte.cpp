#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

//Функция для обработки ошибок GLFW
void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

//Функция для чтения файла шейдера
std::string readShaderFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

//Функция для компиляции шейдера
GLuint compileShader(const std::string& shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* shaderSource = shaderCode.c_str();
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "Error compiling shader: " << infoLog << std::endl;
    }

    return shader;
}

//Функция для создания программы шейдеров
GLuint createShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {
    GLuint vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
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

    //Создание VAO и VBO
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    //Координаты вершин прямоугольника
    GLfloat points[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    //Индексы для отрисовки прямоугольника
    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Чтение и компиляция шейдеров
    std::string vertexShaderCode = readShaderFile("vertex_shader.glsl");
    std::string fragmentShaderCode = readShaderFile("fragment_shader.glsl");

    GLuint shaderProgram = createShaderProgram(vertexShaderCode, fragmentShaderCode);

    
    
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(vao);

        //Изменение цвета в зависимости от времени
        float timeValue = glfwGetTime();
        float red = (sin(timeValue) + 1.0f) / 2.0f;
        float green = (cos(timeValue) + 1.0f) / 2.0f;
        float blue = sin(timeValue * 2.0f) * 0.5f + 0.5f;

        GLint colorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(colorLocation, red, green, blue, 1.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
