//
// Created by brendan on 3/11/24.
//

#include "RenderCommand.h"

std::unique_ptr<RendererAPI> RenderCommand::s_renderAPI = RendererAPI::create();