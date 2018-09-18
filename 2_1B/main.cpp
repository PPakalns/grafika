#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/Utility.h>

static void CompileShader(const std::string& code, GLuint shaderId)
{
    std::cout << "Compiling shader " << std::endl;
    const char * sourcePointer = code.c_str();
    glShaderSource(shaderId, 1, &sourcePointer, nullptr);
    glCompileShader(shaderId);

    GLint result = GL_FALSE;
    int infoLength;
    // Check Vertex Shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
    if (infoLength > 0){
        std::vector<char> infoMessage(static_cast<size_t>(infoLength+1));
        glGetShaderInfoLog(shaderId, infoLength, nullptr, infoMessage.data());
        std::cerr << infoMessage.data() << std::endl;
    }
}

static GLuint LoadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode){
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    CompileShader(vertexShaderCode, vertexShaderId);
    CompileShader(fragmentShaderCode, fragmentShaderId);

    std::cout << "Linking program" << std::endl;
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    GLint result;
    int infoLength;

    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLength);
    if (infoLength > 0){
        std::vector<char> infoMessage(static_cast<size_t>(infoLength + 1));
        glGetProgramInfoLog(programId, infoLength, NULL, infoMessage.data());
        std::cerr << infoMessage.data() << std::endl;
    }

    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return programId;
}

int safe_main(int, char**)
{
    glewExperimental = true;
    if (!glfwInit())
    {
        throw GrafikaException("Failed to initialize glfw");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "2_1B", NULL, NULL);
    if (window == nullptr)
    {
        glfwTerminate();
        throw GrafikaException("Failed to create GLFW window, opengl 3.3 ??");
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw GrafikaException("Failed to initialize glew");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    const std::string vertexShader(R"(
    #version 330 core

    layout(location = 0) in vec3 vertexPosition_modelspace;
    uniform mat4 MVP;

    void main() {
        gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    }
    )");

    const std::string fragmentShader(R"(
    #version 330 core
    out vec3 color;
    void main() {
        color = vec3(1, 0, 0);
    }
    )");

    GLuint programId = LoadShaders(vertexShader, fragmentShader);

    static const GLfloat gVertexBufferData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4 / (float)3, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
    GLint matrixId = glGetUniformLocation(programId, "MVP");

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programId);

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == false);

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayId);
    glDeleteProgram(programId);

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
