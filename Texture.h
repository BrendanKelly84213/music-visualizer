//
// Created by brendan on 2/7/24.
//

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    explicit Texture(const char* path, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT);
    unsigned int ID() const { return m_id; }
private:
    unsigned int m_id {0};
};

#endif //TEXTURE_H
