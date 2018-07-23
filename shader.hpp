/* Copyright Joey de Vries 
   (original code : https://github.com/JoeyDeVries/LearnOpenGL)
   Modified by Tihran Katolikian 06.07.2018
   Updates:
   - code style changed;
   - member values incapsulated, getters and setters created;*/

#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // ------------------------
    // constructor generates the shader on the fly
    Shader(const char *vs_name, const char *fs_name, const char *gs_name = nullptr)
    {
        std::string vs_code;
        std::string fs_code;
        std::string gs_code;
        
        std::ifstream vs_source;
        std::ifstream fs_source;
        std::ifstream gs_source;
        
        // --------------------------
        // ensure ifstream objects can throw exceptions:
        vs_source.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fs_source.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gs_source.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // ---------------------
            // open files
            vs_source.open(std::string("shaders/") + vs_name);
            fs_source.open(std::string("shaders/") + fs_name);
            
            std::stringstream vs_stream, fs_stream;
            
            // --------------------------
            // read file's buffer contents into streams
            vs_stream << vs_source.rdbuf();
            fs_stream << fs_source.rdbuf();	

            // --------------------
            // close file handlers
            vs_source.close();
            fs_source.close();
            
            // ------------------- 
            // convert stream into string
            vs_code = vs_stream.str();
            fs_code = fs_stream.str();			
            
            // -------------------
            // if geometry shader path is present, also load a geometry shader
            if(gs_name != nullptr) {
                gs_source.open(std::string("shaders") + gs_name);
                std::stringstream gs_stream;
                gs_stream << gs_source.rdbuf();
                gs_source.close();
                gs_code = gs_stream.str();
            }
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n"
                      << e.what() << '\n';
        }

        // --------------------
        // compile shaders
        unsigned int vertex, fragment;

        // --------------
        // vertex shader
        const GLchar *vs_str = vs_code.c_str();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vs_str, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, VERTEX);
        
        // --------------
        // fragment Shader
        const GLchar *fs_str = fs_code.c_str();
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fs_str, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, FRAGMENT);
        
        // -----------------------
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        
        if (gs_name != nullptr) {
            const GLchar *gs_str = gs_code.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gs_str, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, GEOMETRY);
        }
        
        // ------------------
        // shader Program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        
        if (gs_name != nullptr)
            glAttachShader(id, geometry);
        
        glLinkProgram(id);
        checkCompileErrors(id, PROGRAM);
        
        // -----------------------
        // delete the shaders as they're linked into our program now
        // and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (gs_name != nullptr)
            glDeleteShader(geometry);
    }
    
    ~Shader() = default;
    
    // --------------------
    // activate the shader
    void use() const
    { 
        glUseProgram(id); 
    }
    
    // ---------------------------
    // utility uniform functions
    void setBool(const std::string &name, const bool value) const
    {         
        glUniform1i(glGetUniformLocation(id, name.c_str()),
                    static_cast <int>(value)); 
    }
    // --------------------------
    void setInt(const std::string &name, const int value) const
    { 
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    // --------------------------
    void setFloat(const std::string &name, const float value) const
    { 
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    
#ifdef OPENGL_SHADER_DOUBLE_PRESISION
    // --------------------------
    // WARNING! Double-presision on GPU only supported in versions 4 and higher!
    void setDouble(const std::string &name, const double &value)
    {
        //may not work in versions lesser then 4
        glUniform1d(glGetUniformLocation(id, name.c_str()), value);
    }
    
    void setVec2d(const std::string &name, const double &a, const double &b)
    {
        //may not work in versions lesser then 4
        glUniform2d(glGetUniformLocation(id, name.c_str()), a, b);
    }
#endif
    
    // -------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, const float x, const float y) const
    { 
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }
    // -------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, const float x, 
                 const float y, const float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }
    // -------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, const float x, const float y,
                 const float z, const float w) 
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w); 
    }
    // -------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1,
                           GL_FALSE, &mat[0][0]);
    }
    // -------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1,
                           GL_FALSE, &mat[0][0]);
    }
    // -------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1,
                           GL_FALSE, &mat[0][0]);
    }
    
    void setMVP(const glm::mat4 &m, const glm::mat4 &v, const glm::mat4 &p)
    {
        setMat4("model", m);
        setMat4("view", v);
        setMat4("projection", p);
    }
    
    // -----------------
    // getters and setters
    unsigned getid() const
    {
        return id;
    }

private:
    enum Type {PROGRAM, VERTEX, FRAGMENT, GEOMETRY};

    // -----------------------
    // shader program id
    unsigned int id;
    
    std::string getTypeName(const Type type) const noexcept(false)
    {
        switch(type) {
            case PROGRAM:
            return "PROGRAM";
            case FRAGMENT:
            return "FRAGMENT";
            case VERTEX:
            return "VERTEX";
            default:
            throw std::runtime_error("Unidentified type\n");
        }
    }
    
    // -----------------------
    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(const unsigned shader, const Type type)
    {
        int success;
        char info_log[1024];
        
        if(type != PROGRAM) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(shader, 1024, NULL, info_log);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " 
                          << getTypeName(type) << '\n' << info_log
                          << "\n-------------------------------------\n";
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shader, 1024, NULL, info_log);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
                          << getTypeName(type) << '\n' << info_log
                          << "\n-------------------------------------\n";
            }
        }
    }
};

#endif  //SHADER_HPP
