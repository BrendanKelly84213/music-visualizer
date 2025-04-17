//
// Created by brendan on 10/16/24.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include <memory>
class FrameBuffer {
public:
    static std::unique_ptr<FrameBuffer> create();
    explicit FrameBuffer(int width, int height)
        : m_width(width), m_height(height), m_id(0), m_textureColorBuffer(0), m_rbo(0) { }
    void bind() const;
    static void unbind();
    void rescale(int width, int height);
    [[nodiscard]] unsigned int textureColorBuffer() const { return m_textureColorBuffer; }
    [[nodiscard]] unsigned int id() const { return m_id; }
    [[nodiscard]] int width() const { return m_width; }
    [[nodiscard]] int height() const { return m_height; }
private:


    int m_width;
    int m_height;
    unsigned int m_id;
    unsigned int m_textureColorBuffer;
    unsigned int m_rbo;
};


#endif //FRAMEBUFFER_H
