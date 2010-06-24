//      dimension.cpp
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

//#include <iostream>
//#include <stdlib.h>
//#include <math.h>
//#include <GL/glfw.h>
#include "functions.hpp"

#include "main_objects.hpp"
#include "terrain.hpp"
#include "tree.hpp"
#include "shaders.hpp"

using namespace std;

//game_object game;

int main()
{
  game.initialize();
  game.main_loop();
  end_game(0);
}
