#ifndef OPENGL_INIT_H
#define OPENGL_INIT_H

#include "gl-headers/mygl.h"
#include "gl-headers/myglu.h"

// private Functions
bool HasVBO();
bool HasDisplayLists();
bool HasVertexBuffers();
bool HasTS1_0(); 
bool HasVP1_0();

// public Functions
int OpenglInit();

#endif
