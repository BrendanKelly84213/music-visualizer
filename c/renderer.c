#include "renderer.h"
#include <malloc.h>

renderer_t* renderer_create(void)
{
    renderer_t* renderer = malloc(sizeof(renderer_t));
    if (renderer == NULL) {
        return NULL;
    }

    // Initialize OpenGL context and other rendering setup here
    return renderer;
}
