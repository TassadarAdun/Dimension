//      functions.cpp
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
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GL/glfw.h>

#include "functions.hpp"

using namespace std;

void end_game(int return_type)
{
  glfwTerminate();
  exit(0);
}

int round(float x)
{
  int x2=ceil(x);
  if(x2-x<0.5)
  {return x2;}
  else
  {return floor(x);}
}

float deg(float x)
{
  return x*(180/M_PI);
}

float rad(float x)
{
  return (x/180)*M_PI; 
}

bool col_line_triangle(float x1,float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, float a, float b, float c, float k, float l, float m, float *cx, float *cy, float *cz)
{
  float lab, nx, ny, nz, d;
  k=k-a;
  l=l-b;
  m=m-c;
  nx=(y3-y1)*(z2-z1)-(y2-y1)*(z3-z1);
  ny=(z3-z1)*(x2-x1)-(z2-z1)*(x3-x1);
  nz=(x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
  d=nx*x1+ny*y1+nz*z1;
  lab=(d-a*nx-b*ny-c*nz)/(k*nx+l*ny+m*nz);
  *cx=a+lab*k;
  *cy=b+lab*l;
  *cz=c+lab*m;
  //check wether the coordinates are in the triangle
  if(x1<=x2 && x1<=x3)
  {
    if(*cx>=x1 && *cz>=z1 && (*cx-x1)+(*cz-z1)<=1.0)
    {return 1;}
  }
  else
  {
    if(*cx<=x1 && *cz<=z1 && (x1-*cx)+(z1-*cz)<=1.0)
    {return 1;}
  }
  return 0;
}
