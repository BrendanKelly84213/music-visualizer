//
// Created by brendan on 10/16/24.
//

#include "FrameBuffer.h"
#include "glad/glad.h"
std::unique_ptr<FrameBuffer> FrameBuffer::create()
{
    auto frameBuffer = std::make_unique<FrameBuffer>();
    glGenFramebuffers(1, &frameBuffer->m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->m_id);

    // Generate texture
    // TODO: Should really refactor to seperate texture class
    glGenTextures(1, &frameBuffer->m_textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, frameBuffer->m_textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->m_textureColorBuffer, 0);

    glGenRenderbuffers(1, &frameBuffer->m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return nullptr;
    }

    frameBuffer->unbind();
    return frameBuffer;
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::rescale(int width, int height) const
{
    glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBuffer, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    glViewport(0, 0, width, height);
}

