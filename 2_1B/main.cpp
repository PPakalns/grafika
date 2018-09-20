#include <iostream>
#include <random>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/Utility.h>
#include <glwrap/Utility.h>

const int MAX_PARTICLES = 50000;
const size_t PARTICLE_SIZE = 3;

GLfloat particlePos[PARTICLE_SIZE * MAX_PARTICLES];
GLfloat particleSpeed[PARTICLE_SIZE * MAX_PARTICLES];

struct Particle {
    static glm::vec3 fromArray(const GLfloat* arr)
    {
        return glm::vec3(arr[0], arr[1], arr[2]);
    }

    static void toArray(GLfloat* arr, const glm::vec3& val)
    {
        arr[0] = val.x;
        arr[1] = val.y;
        arr[2] = val.z;
    }

    static void update(GLfloat* pos, GLfloat* speed, float dt, std::mt19937& gen) {
        glm::vec3 p = fromArray(pos);
        glm::vec3 s = fromArray(speed);
        s += glm::vec3(0, -10, 0) * dt;
        p += s * dt;
        if (p.y < -1.0f) {
            p = glm::vec3(0.0f, -0.5f, 0.0f);
            s = glm::vec3(std::uniform_real_distribution(2.0f, 4.0f)(gen),
                          std::uniform_real_distribution(10.0f, 20.0f)(gen),
                          std::uniform_real_distribution(2.0f, 4.0f)(gen));
        }
        toArray(speed, s);
        toArray(pos, p);
    }
};

int safe_main(int, char**)
{
    std::random_device device;
    std::mt19937 generator(device());
    GLFWwindow* window = glwrap::CreateWindow("2.1B");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    const std::string vertexShader(R"(
    #version 330 core

    layout(location = 0) in vec3 vPosModelspace;
    layout(location = 1) in vec3 vColor;
    layout(location = 2) in vec3 vPos;
    uniform mat4 MV;
    uniform mat4 SCALE;
    out vec3 fragmentColor;

    void main() {
        gl_Position = MV * (vec4(vPos, 0) + (SCALE * vec4(vPosModelspace , 1)));
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

    const GLfloat gPlane[] = {
        -1, 0, 1,
        1, 0, -1,
        -1, 0, -1,
        1, 0, -1,
        -1, 0, 1,
        1, 0, 1,
    };

    const GLfloat gPlaneColor[] = {
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
    };

    const GLfloat gVertexBufferData[] = {
        1, -1, -1,
        -1, -1, -1,
        0, 1, 0,
        -1, -1, -1,
        1, -1, -1,
        0, -1, 1,
        -1, -1, -1,
        0, -1, 1,
        0, 1, 0,
        1, -1, -1,
        0, 1, 0,
        0, -1, 1,
    };

    const GLfloat zeroPos[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
    };

    GLfloat gColorBufferData[sizeof(gVertexBufferData) / sizeof(GLfloat)];
    {
        size_t elems = sizeof(gColorBufferData) / sizeof(GLfloat);
        for (size_t i = 0; i < elems; i += 3)
        {
            gColorBufferData[i] = std::uniform_real_distribution(0.3f, 0.6f)(generator);
            gColorBufferData[i + 1] = std::uniform_real_distribution(0.3f, 0.6f)(generator);
            gColorBufferData[i + 2] = std::uniform_real_distribution(0.8f, 1.0f)(generator);
        }
    }


    GLuint particlePosBuffer;
    glGenBuffers(1, &particlePosBuffer);

    GLuint planeVertexBuffer;
    glGenBuffers(1, &planeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, planeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gPlane), gPlane, GL_STATIC_DRAW);


    GLuint planeColorBuffer;
    glGenBuffers(1, &planeColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, planeColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gPlaneColor), gPlaneColor, GL_STATIC_DRAW);


    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gColorBufferData), gColorBufferData, GL_STATIC_DRAW);

    GLuint planePosBuffer;
    glGenBuffers(1, &planePosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, planePosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zeroPos), zeroPos, GL_STATIC_DRAW);


    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4 / (float)3, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(30, 20, -20), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
    // glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    GLint mvId = glGetUniformLocation(programId, "MV");
    GLint scaleId = glGetUniformLocation(programId, "SCALE");

    auto timePoint = std::chrono::high_resolution_clock::now();

    do {
        auto tmpTimePoint = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedDuration = tmpTimePoint - timePoint;
        float elapsed = elapsedDuration.count();
        timePoint = tmpTimePoint;

        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            Particle::update(&particlePos[3 * i], &particleSpeed[3 * i], elapsed, generator);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programId);

        glBindBuffer(GL_ARRAY_BUFFER, particlePosBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particlePos), particlePos, GL_STREAM_DRAW);

        glm::mat4 mv = projection * view;
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        glUniformMatrix4fv(mvId, 1, GL_FALSE, &mv[0][0]);
        glUniformMatrix4fv(scaleId, 1, GL_FALSE, &scale[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particlePosBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 0);
        glVertexAttribDivisor(2, 1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(gVertexBufferData) / (3 * sizeof(GLfloat)), MAX_PARTICLES);

        // DRAW BLUE PLANE
        mv = projection * view;
        scale = glm::scale(glm::mat4(1.0f), glm::vec3(50, 50, 50));
        glUniformMatrix4fv(mvId, 1, GL_FALSE, &mv[0][0]);
        glUniformMatrix4fv(scaleId, 1, GL_FALSE, &scale[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER, planeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, planeColorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, planePosBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

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
