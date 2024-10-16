//
// Created by brendan on 10/16/24.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include <memory>
class FrameBuffer {
public:
    static std::unique_ptr<FrameBuffer> create();

    void bind() const;
    static void unbind();
    void rescale(int width, int height) const;
    [[nodiscard]] unsigned int textureColorBuffer() const { return m_textureColorBuffer; }
private:
    unsigned int m_id;
    unsigned int m_textureColorBuffer;
    unsigned int m_rbo;
};


#endif //FRAMEBUFFER_H
