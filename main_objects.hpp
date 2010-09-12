//      main_objects.hpp
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
#ifndef MAIN_OBJECTS
#define MAIN_OBJECTS

using namespace std;

class obj_draw
{
  public:
  virtual void draw()=0;
};

class game_object
{
private:
int location;
obj_draw * draw_list[5];//list containing all the things to draw
void draw();//draws everything
public:
//obj_draw * ter1;
int width, height;
//keep these doubles!!!
double fpsfac, fps;
game_object();
~game_object();

void initialize();//initialize dimension

void main_loop();//loops the game


//game dependant functions
float getHeight(int x, int z);
float getHeightExact(float x, float z);
};

class camera
{
  public:
  float x, y, z;
  float xt, yt, zt;
  float dir;
  float ydir;
  float lastx, lasty;
  bool mode_edit;
  float cx, cy, cz;
  camera();
  void mouselook();
  void editmode();
  float frustum(float ox, float oy);
  private:
  bool e_key;
  float fov_y, fov_x, factor_screen, cam_height;
};



extern game_object game;
extern camera cam1;


#endif
