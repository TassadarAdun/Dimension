//      shaders.cpp
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

#include <GL/glew.h>
#include <GL/glfw.h>
#include "shaders.hpp"
//#include "main_objects.hpp"
#include "functions.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

string returnshader(string sFile)
{
  ifstream file(sFile.c_str());
  if (!file.is_open())
  {
    cout<<"Could not open"<< sFile <<"for reading";
    end_game(1);
  }
  stringstream ss;
  ss << file.rdbuf();
  file.close();
  return ss.str();
}

shader::shader()
{
}

shader::shader(string vsFile, string fsFile)
{
  init(vsFile,fsFile);
}

void shader::init(string vsFile, string fsFile)
{
  shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
	const char * vsText = returnshader(vsFile).c_str();
	const char * fsText = returnshader(fsFile).c_str();	
  
	glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_fp, 1, &fsText, 0);
    
	glCompileShader(shader_vp);
	glCompileShader(shader_fp);
    
	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
}

shader::~shader() 
{
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);
    
	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

int shader::id() 
{
	return shader_id;
}

void shader::begin() 
{
	glUseProgram(shader_id);
}

void shader::end() 
{
	glUseProgram(0);
}
