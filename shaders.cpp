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

static char* textFileRead(const char *fileName) {
    char* text;
    
    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = ' ';
            }
            fclose(file);
        }
    }
    return text;
}

Shader::Shader() {
    
}

Shader::Shader(const char *vsFile, const char *fsFile) {
    init(vsFile, fsFile);
}

void Shader::init(const char *vsFile, const char *fsFile) {
    shader_vp = glCreateShader(GL_VERTEX_SHADER);
    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* vsText = textFileRead(vsFile);
    const char* fsText = textFileRead(fsFile);    
    
    if (vsText == NULL || fsText == NULL) {
        cerr << "Either vertex shader or fragment shader file not found." << endl;
        return;
    }
    
    glShaderSource(shader_vp, 1, &vsText, 0);
    glShaderSource(shader_fp, 1, &fsText, 0);
    
    glCompileShader(shader_vp);
    glCompileShader(shader_fp);
    
    shader_id = glCreateProgram();
    glAttachShader(shader_id, shader_fp);
    glAttachShader(shader_id, shader_vp);
    glLinkProgram(shader_id);
}

Shader::~Shader() {
    glDetachShader(shader_id, shader_fp);
    glDetachShader(shader_id, shader_vp);
    
    glDeleteShader(shader_fp);
    glDeleteShader(shader_vp);
    glDeleteProgram(shader_id);
}

unsigned int Shader::id() {
    return shader_id;
}

void Shader::begin() {
    glUseProgram(shader_id);
}

void Shader::end() {
    glUseProgram(0);
}
