#include <iostream>
#include "opengl.h"

bool hasVBO = false; // Vertex Buffer Objects 
bool hasDL = false; // Display lists
bool hasVB = false; // Vertex Buffers
bool hasTS1_0 = false; // Texture Shaders 1.0
bool hasVP1_0 = false; // Vertex Programs 1.0

bool HasVBO(){ return false; }
bool HasDisplayLists(){ return hasDL; }
bool HasVertexBuffers(){ return hasVB; }
bool HasTS1_0(){ return hasTS1_0; }
bool HasVP1_0(){ return hasVP1_0; }


int OpenGL_Init() 
{
#ifdef OPENGL_VERBOSE
  std::cout<<"OpenGL Informations :\n";
  std::cout<<"   OpenGL Vendor: "<< glGetString( GL_VENDOR ) <<"\n";
  std::cout<<"   OpenGL Version: "<< glGetString( GL_VERSION ) <<"\n";
  std::cout<<"   OpenGL Renderer: "<< glGetString( GL_RENDERER ) <<"\n";
#endif //OPENGL_VERBOSE
  
  char *s = (char *)glGetString( GL_EXTENSIONS );
  char *exts = new char[ strlen(s)+1 ];
  strcpy( exts, s );
  char *nexttoken = strtok( exts, " ");

  do 
    {
      if(!strcmp(nexttoken, "GL_ARB_vertex_buffer_object"))
	hasVBO = true;
      if(!strcmp(nexttoken, "GL_NV_texture_shader")) // NVidia only
	hasTS1_0 = true; // ARB version is a good idea
      if(!strcmp(nexttoken, "GL_ARB_vertex_program"))
	hasVP1_0 = true; 
    } 
  while( (nexttoken = strtok(NULL, " ")) );
  
#ifdef OPENGL_VERBOSE
  std::cout<<"   Vertex Buffer Object: "<< HasVBO() <<"\n";
  std::cout<<"   Vertex Program 1.0: "<< HasVP1_0() <<"\n";
  std::cout<<"   Texture Shader 1.0 (NVidia only): "<< HasTS1_0() <<"\n";
#endif //OPENGL_VERBOSE


  delete [] exts;
  return 0;
}

