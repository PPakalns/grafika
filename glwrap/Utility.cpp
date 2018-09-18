#include "Utility.h"

#include <iostream>
#include <vector>

#include "core/Utility.h"

GLFWwindow* glwrap::CreateWindow(const std::string& name, int width, int height)
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
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
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

    return window;
}

void glwrap::CompileShader(const std::string& code, GLuint shaderId)
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

GLuint glwrap::LoadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
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

