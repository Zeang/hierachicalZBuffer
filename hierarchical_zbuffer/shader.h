#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

class Shader {
public:
    Shader(const std::string& vsFilepath, const std::string& fsFilepath) {
        std::string vsCode = readFile(vsFilepath);
        std::string fsCode = readFile(fsFilepath);

        createShaderProgram(vsCode, fsCode);
    }

    Shader(const char* vsCode, const char* fsCode) {
        createShaderProgram(vsCode, fsCode);
    }

    ~Shader() {
        if (_id > 0) {
            glDeleteProgram(_id);
        }
    }

    void use() {
        glUseProgram(_id);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
    }

private:
    GLuint _id = 0;

    std::string readFile(const std::string& filePath) {
        std::ifstream is(filePath, std::ios::in | std::ios::ate);
        try {
            is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            std::stringstream ss;
            ss << is.rdbuf();
            return ss.str();
        }
        catch (std::ifstream::failure& e) {
            throw std::runtime_error(std::string("read ") + filePath + "error: " + e.what());
        }
    }

    GLuint createShader(const std::string& code, GLenum shaderType) {
        GLuint shader = glCreateShader(shaderType);
        if (shader == 0) {
            throw std::runtime_error("create shader failure");
        }

        const char* codeBuf = code.c_str();
        glShaderSource(shader, 1, &codeBuf, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char buffer[1024];
            glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
            throw std::runtime_error("compile error: \n" + std::string(buffer));
        }

        return shader;
    }

    void createShaderProgram(const std::string& vsCode, const std::string fsCode) {
        GLuint vs = 0, fs = 0;
        try {
            vs = createShader(vsCode, GL_VERTEX_SHADER);
            if (!vs) {
                throw std::runtime_error("create vertex shader failure");
            }

            fs = createShader(fsCode, GL_FRAGMENT_SHADER);
            if (!fs) {
                throw std::runtime_error("create fragment shader failure");
            }

            _id = glCreateProgram();
            if (_id == 0) {
                throw std::runtime_error("create shader program failure");
            }

            glAttachShader(_id, vs);
            glAttachShader(_id, fs);

            glLinkProgram(_id);

            GLint success;
            glGetProgramiv(_id, GL_LINK_STATUS, &success);
            if (!success) {
                char buffer[1024];
                glGetProgramInfoLog(_id, sizeof(buffer), NULL, buffer);
                throw std::runtime_error("link program error: " + std::string(buffer));
            }

            glDeleteShader(vs);
            glDeleteShader(fs);
        }
        catch (const std::exception& e) {
            if (vs) glDeleteShader(vs);
            if (fs) glDeleteShader(fs);
            if (_id) glDeleteProgram(_id);
            throw e;
        }
    }
};