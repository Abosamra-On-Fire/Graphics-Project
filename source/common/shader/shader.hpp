#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            program = glCreateProgram();
            if (!program)
                std::cerr << "ERROR: failed to create a shader program" << std::endl;
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());

        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2fv(getUniformLocation(uniform),1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            glUniform3fv(getUniformLocation(uniform),1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4fv(getUniformLocation(uniform),1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        //Question: Why do we delete the copy constructor and assignment operator?
        // bec they share the same handle and if one of them got deleted them the handle will be deleted leading to errors
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
    };

}

#endif