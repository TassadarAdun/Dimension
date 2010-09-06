// vert.glsl
//
// Copyright 2010 Klaas Winter <klaaswinter@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

//Please mark all new code to make debugging easier

void main()
{
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec3 light_dir = normalize(gl_LightSource[0].position.xyz);
  float dot_product = max(dot(normal, light_dir), 0.0);
  vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
  vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
  ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
  
  
  
  gl_FrontColor = (dot_product * diffuse) + ambient;
  //
  gl_TexCoord[0] = gl_MultiTexCoord0;
  //
  
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
