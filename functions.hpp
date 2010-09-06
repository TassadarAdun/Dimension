//      functions.hpp
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
#ifndef FUNCTIONS
#define FUNCTIONS

using namespace std;

void end_game(int return_type);

int round(float x);

float rad(float x);
float deg(float x);

bool col_line_triangle(float x1,float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3, float a, float b, float c, float k, float l, float m, float *cx, float *cy, float *cz);

#endif
