//      shaders.hpp
//      
//      Copyright 2010 Klaas Winter <klaaswinter@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef SHADERS
#define SHADERS

#include <GL/glfw.h>
//#include <string>


using namespace std;



class shader
{
public:
    shader();
    shader(const char* vsFile, const char* fsFile);
    ~shader();
    
    const char* returnshader(const char* fileName);
    void init(const char* vsFile, const char* fsFile);
    
    void check_compile(GLuint sha_ver, const char* file);
    void check_program(GLuint program);
    
    void begin();
    void end();
    
    int id();
    
private:
    int shader_id;
    int shader_vp;
    int shader_fp;
};

extern shader shader1;

#endif
