#ifndef SHADER_HPP
#define SHADER_HPP

#include <new>
#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

class ShaderProgram {

private:
    // Shader Program Handle
    GLuint program;

public:
    void create();
    void destroy();

    ShaderProgram() { program = 0; }
    ~ShaderProgram() { destroy(); }

    bool attach(const std::string &filename, GLenum type) const;

    bool link() const;

    void use() {
        // TODO: call opengl to use the program identified by this->program
        glUseProgram(program);
    }

    GLuint getUniformLocation(const std::string &name) {
        // TODO: call opengl to get the uniform location for the uniform defined
        // by name from this->program
        return glGetUniformLocation(this->program, name.c_str());
    }

    void set(const std::string &uniform, GLfloat value) {
        // TODO: call opengl to set the value to the uniform defined by name
        glUniform1f(this->getUniformLocation(uniform), value);
    }

    void set(const std::string &uniform, glm::vec2 value) {
        // TODO: call opengl to set the value to the uniform defined by name
        glUniform2fv(this->getUniformLocation(uniform), 1, &value[0]);
    }

    void set(const std::string &uniform, glm::vec3 value) {
        // TODO: call opengl to set the value to the uniform defined by name
        glUniform3fv(this->getUniformLocation(uniform), 1, &value[0]);
    }

    void set(const std::string &uniform, glm::vec4 value) {
        // TODO: call opengl to set the value to the uniform defined by name
        glUniform4fv(this->getUniformLocation(uniform), 1, &value[0]);
    }

    // TODO: Delete the copy constructor and assignment operator
    // Question: Why do we do this? Hint: Look at the deconstructor
    ShaderProgram(const ShaderProgram &other) = delete;
    ShaderProgram operator==(const ShaderProgram &other) = delete;
};

} // namespace our

#endif
