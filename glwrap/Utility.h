#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

namespace glwrap{
    GLFWwindow* CreateWindow(const std::string& name, int width = 1280, int height = 768);

    void CompileShader(const std::string& code, GLuint shaderId);

    GLuint LoadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
}
