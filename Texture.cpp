//
// Created by brendan on 2/7/24.
//

#include <iostream>
#include <glad/glad.h>

#include "external/stb_image.h"

#include "Texture.h"

Texture::Texture(const char* path, GLint wrapS, GLint wrapT)
{
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    GLenum format = GL_RGB;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (nrChannels == 4)
    {
        format = GL_RGBA;
    }
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

