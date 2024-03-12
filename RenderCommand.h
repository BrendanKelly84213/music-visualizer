//
// Created by brendan on 3/11/24.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "RendererAPI.h"

class RenderCommand {
public:
    static inline void setClearColor(const glm::vec4& color)
    {
        s_renderAPI->setClearColor(color);
    }

    static inline void clear()
    {
        s_renderAPI->clear();
    }

    static inline void drawIndexed(int count, const void* indices = nullptr)
    {
        s_renderAPI->drawIndexed(count, indices);
    }

private:
    static std::unique_ptr<RendererAPI> s_renderAPI;
};


#endif //RENDERCOMMAND_H
