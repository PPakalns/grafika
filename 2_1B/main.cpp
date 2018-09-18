#include <iostream>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/Utility.h>
#include <glwrap/Utility.h>

int safe_main(int, char**)
{
    std::default_random_engine generator;
    GLFWwindow* window = glwrap::CreateWindow("2.1B");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    const std::string vertexShader(R"(
    #version 330 core

    layout(location = 0) in vec3 vPosModelspace;
    layout(location = 1) in vec3 vColor;
    uniform mat4 MVP;
    out vec3 fragmentColor;

    void main() {
        gl_Position = MVP * vec4(vPosModelspace , 1);
        fragmentColor = vColor;
    }
    )");

    const std::string fragmentShader(R"(
    #version 330 core
    out vec3 color;
    in vec3 fragmentColor;

    void main() {
        color = fragmentColor;
    }
    )");

    GLuint programId = glwrap::LoadShaders(vertexShader, fragmentShader);

    const GLfloat gVertexBufferData[] = {
        -1.0f , -1.0f , -1.0f ,
        -1.0f , -1.0f , 1.0f  ,
        -1.0f , 1.0f  , 1.0f  ,
        1.0f  , 1.0f  , -1.0f ,
        -1.0f , -1.0f , -1.0f ,
        -1.0f , 1.0f  , -1.0f ,
        1.0f  , -1.0f , 1.0f  ,
        -1.0f , -1.0f , -1.0f ,
        1.0f  , -1.0f , -1.0f ,
        1.0f  , 1.0f  , -1.0f ,
        1.0f  , -1.0f , -1.0f ,
        -1.0f , -1.0f , -1.0f ,
        -1.0f , -1.0f , -1.0f ,
        -1.0f , 1.0f  , 1.0f  ,
        -1.0f , 1.0f  , -1.0f ,
        1.0f  , -1.0f , 1.0f  ,
        -1.0f , -1.0f , 1.0f  ,
        -1.0f , -1.0f , -1.0f ,
        -1.0f , 1.0f  , 1.0f  ,
        -1.0f , -1.0f , 1.0f  ,
        1.0f  , -1.0f , 1.0f  ,
        1.0f  , 1.0f  , 1.0f  ,
        1.0f  , -1.0f , -1.0f ,
        1.0f  , 1.0f  , -1.0f ,
        1.0f  , -1.0f , -1.0f ,
        1.0f  , 1.0f  , 1.0f  ,
        1.0f  , -1.0f , 1.0f  ,
        1.0f  , 1.0f  , 1.0f  ,
        1.0f  , 1.0f  , -1.0f ,
        -1.0f , 1.0f  , -1.0f ,
        1.0f  , 1.0f  , 1.0f  ,
        -1.0f , 1.0f  , -1.0f ,
        -1.0f , 1.0f  , 1.0f  ,
        1.0f  , 1.0f  , 1.0f  ,
        -1.0f , 1.0f  , 1.0f  ,
        1.0f  , -1.0f , 1.0f
    };

    GLfloat gColorBufferData[sizeof(gVertexBufferData) / sizeof(GLfloat)];
    {
        std::uniform_real_distribution dist(0.0f, 1.1f);
        for (GLfloat& f : gColorBufferData)
        {
            f = dist(generator);;
        }
    }

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gColorBufferData), gColorBufferData, GL_STATIC_DRAW);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4 / (float)3, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
    GLint matrixId = glGetUniformLocation(programId, "MVP");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programId);

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(gVertexBufferData) / (3 * sizeof(GLfloat)));

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == false);

    glDeleteBuffers(1, &colorBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayId);
    glDeleteProgram(programId);

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
