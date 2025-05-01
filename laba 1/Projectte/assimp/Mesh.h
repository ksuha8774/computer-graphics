#ifndef MESH_H
#define MESH_H

#include <GL\GL.h>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <vector>
#include "Shader.h"
using namespace std;

struct Vertex {
    glm::vec3 Position;  // Позиция вершины
    glm::vec3 Normal;    // Нормаль вершины
};

class Mesh {
public:
    vector <Vertex>  vertices;
    vector <unsigned int> indices;
    unsigned int VAO;  // Идентификатор Vertex Array Object

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
        setupMesh();
    }

    void Draw(Shader& shader) {
        if (VAO == 0 || indices.empty()) {
            std::cerr << "Mesh not properly initialized!" << std::endl;
            return;
        }
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VBO, EBO;  // Идентификаторы буферов

    void setupMesh() {
        // Генерация буферов
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Заполнение буфера вершин
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Заполнение буфера индексов
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Указание атрибутов вершин (позиция)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Указание атрибутов вершин (нормаль)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glBindVertexArray(0);
    }
};
#endif