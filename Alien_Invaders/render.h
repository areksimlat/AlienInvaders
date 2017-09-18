#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "models.h"


class Render;


class Render{
    public:
        void    init();
        void    reshape( int width, int height );
        void    draw();
};

#endif // RENDER_H_INCLUDED
