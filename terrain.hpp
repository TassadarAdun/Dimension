//      terrain.hpp
//      
//      Copyright 2009 Klaas Winter <klaaswinter@gmail.com>
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
#include <GL/glfw.h>
#include <string>
#include "main_objects.hpp"
#include "vec3f.hpp"

#ifndef ENVIRONMENT
#define ENVIRONMENT

using namespace std;

class terrain: public obj_draw
{
public:
terrain();
void draw();
float getHeight(int x, int z);
float getHeightExact(float x, float z);
Vec3f getNormal(int x, int z);
//GLFWimage heightmap;
private:
float heightdata[1024][1024];
int width;
int height;
//int lod1;
int drawamount;
GLuint tex;
Vec3f** normals;
string line;
string line2;
size_t pos;
};

//extern terrain * ter
#endif
