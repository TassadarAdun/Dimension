//      main_object.cpp
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
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "functions.hpp"
#include "terrain.hpp"
#include "main_objects.hpp"
#include "shaders.hpp"
#include "tree.hpp"

using namespace std;

game_object game;
camera cam1;
terrain * ter1;
shader shader1;


game_object::game_object()
{
width=1440;
height=900;
location=0;
//for(int i=0; i<6; i++)
//{
// draw_list[i]=0;
//}
}

game_object::~game_object()
{
end_game(0);
}

void game_object::initialize()//initialize dimension
{
  if (glfwInit() != GL_TRUE)//glfw started check
  {
    cout<<"GLFW failed to start. This could be a bug, please contact the creator.";
    end_game(1);
  }

  //create new window
  if (glfwOpenWindow(width, height, 5, 6, 5, 0, 0, 0, GLFW_FULLSCREEN) != GL_TRUE)
  {
    cout<<"GLFW failed to open a window. This could be a bug, please contact the creator.";
    end_game(1);
  }
  glfwSetWindowTitle("Dimension");
GLenum err = glewInit();
if (GLEW_OK != err)
{
  /* Problem: glewInit failed, something is seriously wrong. */
  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  shader1.init("vert.glsl","frag.glsl");
  //set camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)width/(double)height, 1.0, 1000.0);
  gluLookAt(0.0, 50.0, 0.0, 0.0, 2.0, 1.0, 0.0, 1.0, 0.0);
  glMatrixMode(GL_MODELVIEW);

  //initialize opengl stuff
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);

  //create terrain
  ter1=new terrain;
  draw_list[location]=ter1;
  location++;
}


void game_object::draw()//draws everything
{
  glLoadIdentity();

  
  GLfloat ambientColor[] = {0.5, 0.5, 0.5, 1.0}; //ambient light
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
  GLfloat lightColor0[] = {0.9, 0.9, 0.9, 1.0};//positioned light
  GLfloat lightPos0[] = {1.0, 0.0, 0.0, 0.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);


  //glColor3f(0.1, 0.9, 0.1);

  //draw everything in the draw list
  for(int i=0; i<location; i++)
  {
    //printf("%d\n",draw_list[i]);
    draw_list[i]->draw();
  }
  
}

void game_object::main_loop()
{
  // the time of the previous frame
  double old_time = glfwGetTime();


  // this just loops as long as the program runs
  while(1)
  {
    // calculate time elapsed, and the amount by which stuff rotates
    double current_time = glfwGetTime();	

    //calculate fps
    fps=1/(current_time-old_time);

    if(fps>10)
    fpsfac=60/fps;
    //cout<<fps<<":";
    //       delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
    old_time = current_time;
    // escape to quit
    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
      break;

    //camera
    //glMatrixMode(GL_PROJECTION);
    cam1.mouselook();
    //glMatrixMode(GL_MODELVIEW);

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
    draw();
    // swap back and front buffers
    glfwSwapBuffers();
  }
}


camera::camera()
  {
  x=128;
  y=100;
  z=128;
  xt=0;
  yt=0;
  zt=1;
  dir=0;
  ydir=0;
  lastx=400;
  lasty=300;
  }
void camera::mouselook()
  {
    int xm, ym;
    glfwGetMousePos(&xm, &ym);
    if(!(xm==lastx && ym==lasty))
    {
      float change_x;
      float change_y;
      change_x=(xm-lastx)/16;
      change_y=(ym-lasty)/16;
      lastx=xm;
      lasty=ym;
      dir-=change_x/60;
      ydir-=change_y/64;
      //glfwSetMousePos(400,300);
      xt=game.fpsfac*sin(dir);
      yt=game.fpsfac*ydir;
      zt=game.fpsfac*cos(dir);
    }
    if(glfwGetKey('W')==GLFW_PRESS)//W
    {
      x=x+game.fpsfac*sin(dir);
      z=z+game.fpsfac*cos(dir);
      y=game.getHeightExact(x,z)+1.6;//bodyheight in meters
    }
    if(glfwGetKey('S')==GLFW_PRESS)//S
    {
      x=x+game.fpsfac*sin(dir-M_PI);
      z=z+game.fpsfac*cos(dir-M_PI);
      y=game.getHeightExact(x,z)+1.6;
    }
    if(glfwGetKey('A')==GLFW_PRESS)//A
    {
      x=x+game.fpsfac*sin(dir+M_PI_2);
      z=z+game.fpsfac*cos(dir+M_PI_2);
      y=game.getHeightExact(x,z)+1.6;
    }
    if(glfwGetKey('D')==GLFW_PRESS)//D
    {
      x=x+game.fpsfac*sin(dir-M_PI_2);
      z=z+game.fpsfac*cos(dir-M_PI_2);
      y=game.getHeightExact(x,z)+1.6;
    }
    //y=200;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, game.width/game.height, 1.0, 1000.0);
  gluLookAt(x, y, z, x+xt, y+yt, z+zt, 0.0, 1.0, 0.0);
  glMatrixMode(GL_MODELVIEW);
  }

//game dependant functions
float game_object::getHeight(int x, int z)
{
  return ter1->getHeight(x, z);//ter1->heightmap.Data[z*256+x];
}

float game_object::getHeightExact(float x, float z)
{
  return ter1->getHeightExact(x, z);//ter1->heightmap.Data[z*256+x];
}

float camera::frustum(float ox, float oz)
{
  return abs(dir-atan2(x-ox,z-oz))>45;
}
