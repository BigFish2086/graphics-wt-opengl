#include "shader.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

void our::ShaderProgram::create() {
    // Create Shader Program
    program = glCreateProgram();
}

void our::ShaderProgram::destroy() {
    // Delete Shader Program
    if (program != 0)
        glDeleteProgram(program);
    program = 0;
}

// Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename,
                                GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code
    // of our shader
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR: Couldn't open shader file: " << filename
                  << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file),
                                           std::istreambuf_iterator<char>());
    const char *sourceCStr = sourceString.c_str();
    file.close();

    GLuint shaderID = glCreateShader(type);

    // TODO: send the source code to the shader and compile it
    // void glShaderSource(GLuint shader, GLsizei count, const GLchar **string,
    //                     const GLint *length);
    // shader
    //     Specifies the handle of the shader object whose source code is to
    //     be replaced.
    // count
    //     Specifies the number of elements in the string and length arrays.
    // string
    //     Specifies an array of pointers to strings containing the source
    //     code to be loaded into the shader.
    // length
    //     Specifies an array of string lengths. If it's nullptr, each string is
    //     assumed to be null-terminated
    //
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID);

    // Here we check for compilation errors
    // TODO: Uncomment this if block
    if (std::string error = checkForShaderCompilationErrors(shaderID);
        error.size() != 0) {
        std::cerr << "ERROR IN " << filename << std::endl;
        std::cerr << error << std::endl;
        // OpenGL copies the shader source code strings when glShaderSource is
        // called, so an application may free its copy of the source code
        // strings immediately after the function returns.
        glDeleteShader(shaderID);
        return false;
    }

    // TODO: attach the shader to the program then delete the shader
    glAttachShader(this->program, shaderID);
    glDeleteShader(shaderID);

    // We return true since the compilation succeeded
    return true;
}

bool our::ShaderProgram::link() const {
    // TODO: call opengl to link the program identified by this->program
    glLinkProgram(this->program);

    // Here we check for linking errors
    // TODO: Uncomment this if block
    if (auto error = checkForLinkingErrors(program); error.size() != 0) {
        std::cerr << "LINKING ERROR" << std::endl;
        std::cerr << error << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader) {
    // Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program) {
    // Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}
