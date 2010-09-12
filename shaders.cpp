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
//#include <GL/glfw.h>
#include "shaders.hpp"
//#include "main_objects.hpp"
#include "functions.hpp"
#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

const char* shader::returnshader(const char* filename)
{
  
char* text;
  //have they given us a filename?
  if (filename == NULL) 
  {
    cerr<<"Could not open"<< filename <<"for reading"<<endl;
    end_game(1);
  }
  else
  {
    FILE *file = fopen(filename, "rt");
    //is the file empty?
    if (file == NULL) 
    {
      cerr<<filename <<"is empty"<<endl;
      end_game(1);
    }
    else
    {
      //count the amount of characters in the file
      fseek(file, 0, SEEK_END);
      int count = ftell(file);
      rewind(file);
            
      if (count > 0) {
        //free as much memory as needed
        text = (char*)malloc(sizeof(char) * (count + 1));
        //put the data into text
        count = fread(text, sizeof(char), count, file);
        //end it all with termination
        text[count] = '\0';
      }
      fclose(file);
    }
  }
  return text;
}

void shader::check_compile(GLuint sha_ver, const char* sha_file = 0)
{
  int buffer_size = 512;
  char buffer[buffer_size];
  memset(buffer, 0, buffer_size);
  GLsizei length = 0;
  
  glGetShaderInfoLog(sha_ver, buffer_size, &length, buffer);
  if (length > 0) 
  {
    cerr << "Shader " << sha_ver << " (file:" << sha_file << ") compile error: " << buffer << endl;
  }
}

void shader::check_program(GLuint program)
{
  int buffer_size = 512;
  char buffer[buffer_size];
  memset(buffer, 0, buffer_size);
  GLsizei length = 0;
  
  memset(buffer, 0, buffer_size);
  glGetProgramInfoLog(program, buffer_size, &length, buffer);
  if (length > 0)
  {
    cerr << "Program " << program << " link error: " << buffer << endl;
  }
  
  glValidateProgram(program);
  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE)
  {
    cerr << "Error validating shader " << program << endl;
  }
}

shader::shader()
{
}

shader::shader(const char* vsFile, const char* fsFile)
{
  init(vsFile,fsFile);
}

void shader::init(const char* vsFile, const char* fsFile)
{

  shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

  //the ordering of below commands are important and thanks to returnshader will return an error if you change the order!
  const char * fsText = returnshader(fsFile);	
	glShaderSource(shader_fp, 1, &fsText, 0);
	glCompileShader(shader_fp);
  check_compile(shader_fp,fsFile);

	const char * vsText = returnshader(vsFile);
  glShaderSource(shader_vp, 1, &vsText, 0);
	glCompileShader(shader_vp);
  check_compile(shader_vp,vsFile);

    
	shader_id = glCreateProgram();
  glAttachShader(shader_id, shader_fp);
  glAttachShader(shader_id, shader_vp);

	glLinkProgram(shader_id);
  check_program(shader_id);
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
