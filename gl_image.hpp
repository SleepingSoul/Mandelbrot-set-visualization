/* copyright Tihran Katolikian 2018*/

#ifndef GL_IMAGE_HPP
#define GL_IMAGE_HPP

#include <string>
#include <stdexcept>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include <glad/glad.h>
#include <GL/glfw3.h>

class GLTextureGenerator
{
public:
    GLTextureGenerator() = delete;
    static void setFlipVerticallyOnLoad(const bool f)
    {
        stbi_set_flip_vertically_on_load(f);
    }
    static unsigned generateTexture2D(const char *path)
    {
        // ------------------------------
        // creating, generating, binding ans setting-up the texture object
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // ------------------------------
        // loading image in memory using stbi_load function from stbi_image.h
        int width, height, chan_num;
        unsigned char *data = stbi_load(path, &width, &height,
                                        &chan_num, 0);
        
        auto format = checkFormat(chan_num);
        if (!format)
            throw std::runtime_error("Undefined image format\n");
        
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, *format, width, height, 0,
                         *format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load texture\n";
    
        // ------------------------------
        // deallocating image memory, because now it is already stored
        // in GPU memory
        stbi_image_free(data);

        return texture;
    }
    static unsigned generateTexture2D(const std::string &path)
    {
        // ------------------------------
        // creating, generating, binding ans setting-up the texture object
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // ------------------------------
        // loading image in memory using stbi_load function from stbi_image.h
        int width, height, chan_num;
        unsigned char *data = stbi_load(path.c_str(), &width, &height,
                                        &chan_num, 0);
        
        auto format = checkFormat(chan_num);
        if (!format)
            throw std::runtime_error("Undefined image format\n");
        
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, *format, width, height, 0,
                         *format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load texture\n";
    
        // ------------------------------
        // deallocating image memory, because now it is already stored
        // in GPU memory
        stbi_image_free(data);

        return texture;
    }
    
    static unsigned generateTexture1D(const char *path)
    {
        // ------------------------------
        // creating, generating, binding ans setting-up the texture object
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_1D, texture);

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // ------------------------------
        // loading image in memory using stbi_load function from stbi_image.h
        int width, height, chan_num;
        unsigned char *data = stbi_load(path, &width, &height,
                                        &chan_num, 0);
        
        auto format = checkFormat(chan_num);
        if (!format)
            throw std::runtime_error("Undefined image format\n");
        
        if (data) {
            glTexImage1D(GL_TEXTURE_1D, 0, *format, width, 0,
                         *format, GL_UNSIGNED_BYTE, data);
        }
        else
            std::cout << "Failed to load texture\n";
    
        // ------------------------------
        // deallocating image memory, because now it is already stored
        // in GPU memory
        stbi_image_free(data);

        return texture;
    }
    
private:
    static std::optional <GLenum> checkFormat(const int chan_num)
    {
        switch (chan_num) {
        case 1:
            return GL_RED;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return std::nullopt;
        }
    }
};

#endif // GL_IMAGE_HPP
