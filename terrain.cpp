//      terrain.cpp
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
#include <math.h>
#include <fstream>
#include <sstream>
#include "functions.hpp"
#include "terrain.hpp"
#include <stdio.h>
 
using namespace std;
 
terrain::terrain()
{
  
width=1024;
height=1024;
/*loading heightmap
if(glfwReadImage("heightmap.tga", &heightmap, 0)!=GL_TRUE)
{
  cout<<"Error reading heightmap!";
  end_game(1);
}*/
//
ifstream textfile("height.dat");
if(!textfile.is_open())
{
  cout<<"Error reading heightmap";
  end_game(1);
}
else
{
  //float heightdata[height][width];
  //string line;
  for(int z=0;z<1024;z++)
  {
    getline(textfile,line);
    if(line.empty())
    {
      break;
    }
    for(int x=0;x<1024;x++)
    {        //cout<<line;
      pos=line.find(' ');
      if(pos==string::npos)
      {
        break;
      }
      string line2(line,0,pos);
      stringstream strh(line2);
      strh >> heightdata[z][x];
      //printf("%d\n",heightdata[z][x]);
      //cout<<"reach!";
      line.erase(0,pos+1);
    }
  }
  textfile.close();
}

glGenTextures(1,&tex);
glBindTexture(GL_TEXTURE_2D, tex);
//GL_MODULATE for correct lighting and shit
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//when texture area is small, bilinear filter the closest mipmap
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//when texture area is large, bilinear filter the original
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
if(glfwLoadTexture2D("terrain1.tga", GLFW_BUILD_MIPMAPS_BIT)!=GL_TRUE)
{
  cout<<"Error reading terrain texture!";
  end_game(1);
}


//lod1=50;
/*for(int i=0; i<240; i++)
{
 printf("%d\n",heightmap.Data[i*256]);
}*/

//calculate normals
normals = new Vec3f*[height];
Vec3f** normals2 = new Vec3f*[height];
for(int i = 0; i < height; i++) {
  normals[i] = new Vec3f[width];
  normals2[i] = new Vec3f[width];
}

//rough normals
for(int z = 0; z < height; z++) {
  for(int x = 0; x < width; x++) {
    Vec3f sum(0.0, 0.0, 0.0);

    Vec3f down;
    if (z > 0) {
      down = Vec3f(0.0, getHeight(x, z - 1) - getHeight(x, z), -1.0);
    }
    Vec3f up;
    if (z < height - 1) {
      up = Vec3f(0.0, getHeight(x, z + 1) - getHeight(x, z), 1.0);
    }
    Vec3f left;
    if (x > 0) {
      left = Vec3f(-1.0, getHeight(x - 1, z) - getHeight(x, z), 0.0);
    }
    Vec3f right;
    if (x < width - 1) {
      right = Vec3f(1.0, getHeight(x + 1, z) - getHeight(x, z), 0.0);
    }

    if (x > 0 && z > 0) {
      sum += down.cross(left).normalize();
    }
    if (x > 0 && z < height - 1) {
      sum += left.cross(up).normalize();
    }
    if (x < width - 1 && z < height - 1) {
      sum += up.cross(right).normalize();
    }
    if (x < width - 1 && z > 0) {
      sum += right.cross(down).normalize();
    }

    normals2[x][z] = sum;
  }
}
//smooth normals
const float FALLOUT_RATIO = 0.5f;
for(int z = 0; z < height; z++) {
  for(int x = 0; x < width; x++) {
    Vec3f sum = normals2[x][z];

    if (x > 0) {
      sum += normals2[x - 1][z] * FALLOUT_RATIO;
    }
    if (x < width - 1) {
      sum += normals2[x + 1][z] * FALLOUT_RATIO;
    }
    if (z > 0) {
      sum += normals2[x][z - 1] * FALLOUT_RATIO;
    }
    if (z < height - 1) {
      sum += normals2[x][z + 1] * FALLOUT_RATIO;
    }

    if (sum.magnitude() == 0) {
      sum = Vec3f(0.0, 1.0, 0.0);
    }
    normals[x][z] = sum;
  }
}

//delete first array
for(int i = 0; i < height; i++) {
  delete[] normals2[i];
}
delete[] normals2;

}

void terrain::draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    int nlod=2;
    int lod[nlod];
    lod[2]=width/2;//only if width=height
    lod[1]=100;//can be divided by 4
    lod[0]=50;//can be divided by 2
    int exp=2;
    Vec3f normal;
    int zp, xp, zp2, xp2, zt, zf, xt, xf;
    float zgH, gH;
    float height;

for(int i=nlod; i>0; i--)
{
    int fc=pow(2,i);//never ever use ^!
    xp=fabs(round(cam1.x/fc)*fc);//round(cam1.x/2)*2 to avoid overlapping and shifting
    zp=fabs(round(cam1.z/fc)*fc);
    int lfc=pow(2,i+1);
    if(i==nlod)
    {
      zf=0;
      zt=lod[i]*2-fc;//fc in case that the final pixel cannot be divided by fc
      xf=0;
      xt=lod[i]*2-fc;
    }
    else
    {
      xp2=fabs(round(cam1.x/lfc)*lfc);
      zp2=fabs(round(cam1.z/lfc)*lfc);
      zf=zp2-lod[i];
      zt=zp2+lod[i]+lfc;//to avoid empty spaces
      xf=xp2-lod[i];
      xt=xp2+lod[i];
      if(zf<0)
      {zf=0;}
      if(zt>lod[nlod]*2-lfc)
      {zt=lod[nlod]*2-lfc;}
      if(xf<0)
      {xf=0;}
      if(xt>lod[nlod]*2-lfc)
      {xt=lod[nlod]*2-lfc;}
    }

    for(int z = zf; z < zt; z=z+fc) {

        if(fabs(zp-z)>lod[i-1])
        {
//glColor3f(0.1, 0.9, 0.1);
          glBegin(GL_TRIANGLE_STRIP);
          for(int x = xf; x <= xt; x=x+fc) {
              normal = getNormal(x, z);
              glNormal3f(normal[0], normal[1], normal[2]);
              glTexCoord2f((float)x/((float)lod[nlod]*2),(float)z/((float)lod[nlod]*2));
              //
              if(!(x % lfc==0) && (z==zf))
              {
                height=(getHeight(x-fc,z)+getHeight(x+fc,z))/2;
              }
              else
              if(!(z % lfc==0) && ((x==xf) || (x==xt)))
              {
                height=(getHeight(x,z-fc)+getHeight(x,z+fc))/2;
              }
              else
              {
                height=getHeight(x,z);
              }
              //
              glVertex3f(x, height, z);
              normal = getNormal(x, z+fc);
              glNormal3f(normal[0], normal[1], normal[2]);
              glTexCoord2f((float)x/((float)lod[nlod]*2),((float)z+fc)/((float)lod[nlod]*2));
              //
              if(!(x % lfc==0) && (z==zt-fc))
              {
                height=(getHeight(x-fc,z+fc)+getHeight(x+fc,z+fc))/2;
              }
              else
              if(!((z+fc) % lfc==0) && ((x==xf) || (x==xt)))
              {
                height=(getHeight(x,z)+getHeight(x,z+lfc))/2;
              }
              else
              {
                height=getHeight(x,z+fc);
              }
              //
              glVertex3f(x, height, z+fc);

          }
          glEnd();
        }
        else
        {
//glColor3f(0.9, 0.9, 0.9);
          if(xp>lod[i-1])//assumes you can't get of the map
          {
            glBegin(GL_TRIANGLE_STRIP);//right lod(low)
            for(int x = xf; x <= xp-lod[i-1]; x=x+fc) {
              normal = getNormal(x, z);
              glNormal3f(normal[0], normal[1], normal[2]);
              glTexCoord2f((float)x/((float)lod[nlod]*2),(float)z/((float)lod[nlod]*2));
              //
              if(!(z % lfc==0) && (x==xf))
              {
                height=(getHeight(x,z-fc)+getHeight(x,z+fc))/2;
              }
              else
              {
                height=getHeight(x,z);
              }
              //
              glVertex3f(x, height, z);
              normal = getNormal(x, z+fc);
              glNormal3f(normal[0], normal[1], normal[2]);
              glTexCoord2f((float)x/((float)lod[nlod]*2),((float)z+fc)/((float)lod[nlod]*2));
              //
              if(!((z+fc) % lfc==0) && (x==xf))
              {
                height=(getHeight(x,z)+getHeight(x,z+lfc))/2;
              }
              else
              {
                height=getHeight(x,z+fc);
              }
              //
              glVertex3f(x, height, z+fc);

            }
            glEnd();
          }
//glColor3f(0.1, 0.1, 0.9);
          if(xp+lod[i-1]<lod[nlod]*2)
          {
            glBegin(GL_TRIANGLE_STRIP);//middle-left lod(low)
            for(int x = xp+lod[i-1]; x <= xt; x=x+fc) {
              normal = getNormal(x, z);
              glNormal3f(normal[0], normal[1], normal[2]);
              glTexCoord2f((float)x/((float)lod[nlod]*2),(float)z/((float)lod[nlod]*2));
              //
              if(!(z % lfc==0) && (x==xt))
              {
                height=(getHeight(x,z-fc)+getHeight(x,z+fc))/2;
              }
              else
              {
                height=getHeight(x,z);
              }
              //
              glVertex3f(x, height, z);
              
              normal = getNormal(x, z+fc);
              glNormal3f(normal[0], normal[1], normal[2]);
              glTexCoord2f((float)x/((float)lod[nlod]*2),((float)z+fc)/((float)lod[nlod]*2));
              //
              if(!((z+fc) % lfc==0) && (x==xt))
              {
                height=(getHeight(x,z)+getHeight(x,z+lfc))/2;
              }
              else
              {
                height=getHeight(x,z+fc);
              }
              //
              glVertex3f(x, height, z+fc);

            }
            glEnd();
          }
        }
    }
}
//glColor3f(0.9, 0.1, 0.1);
int lfc=pow(2,1);
xp2=fabs(round(cam1.x/lfc)*lfc);
zp2=fabs(round(cam1.z/lfc)*lfc);
zf=zp2-lod[0];
zt=zp2+lod[0]+lfc;
xf=xp2-lod[0];
xt=xp2+lod[0];
if(zf<0)
{zf=0;}
if(zt>lod[nlod]*2-lfc)
{zt=lod[nlod]*2-lfc;}
if(xf<0)
{xf=0;}
if(xt>lod[nlod]*2-lfc)
{xt=lod[nlod]*2-lfc;}
for(int z = zf; z < zt; z++){
  glBegin(GL_TRIANGLE_STRIP);//middle-piece
  for(int x = xf; x <= xt; x++) {
    normal = getNormal(x, z);
    glNormal3f(normal[0], normal[1], normal[2]);
    //glTexCoord2f((float)x/((float)lod[nlod]*2),(float)z/((float)lod[nlod]*2));
    glTexCoord2f(x % lfc,1);
    //
    if(!(x % lfc==0) && (z==zf))
    {
      height=(getHeight(x-1,z)+getHeight(x+1,z))/2;
    }
    else
    if(!(z % lfc==0) && ((x==xf) || (x==xt)))
    {
      height=(getHeight(x,z-1)+getHeight(x,z+1))/2;
    }
    else
    {
      height=getHeight(x,z);
    }
    //
    glVertex3f(x, height, z);
    normal = getNormal(x, z+1);
    glNormal3f(normal[0], normal[1], normal[2]);
    //glTexCoord2f((float)x/((float)lod[nlod]*2),((float)z+1)/((float)lod[nlod]*2));
    glTexCoord2f(x % lfc,0);
    //
    if(!(x % lfc==0) && (z==zt-1))
    {
      height=(getHeight(x-1,z+1)+getHeight(x+1,z+1))/2;
    }
    else
    if(!((z+1) % lfc==0) && ((x==xf) || (x==xt)))
    {
      height=(getHeight(x,z)+getHeight(x,z+2))/2;
    }
    else
    {
      height=getHeight(x,z+1);
    }
    //
    glVertex3f(x, height, z+1);

  }
  
  glEnd();
}
glDisable(GL_TEXTURE_2D);
}

float terrain::getHeight(int x, int z)
{
  return heightdata[z][x];
}

float terrain::getHeightExact(float x, float z)
{
  float x1, x2, x3, y1, y2, y3, z1, z2, z3, xdif, zdif;
  float rx, rz;
  rx=floor(x);
  rz=floor(z);
  if(((x-rx)+(z-rz))<=1.0)
  {
    x1=0; y1=getHeight(rx, rz); z1=0;
    x2=1; y2=getHeight(rx+1, rz)-y1; z2=0;
    x3=0; y3=getHeight(rx, rz+1)-y1; z3=1;
    xdif=x-rx; zdif=z-rz;
  }
  else
  {
    x1=1; y1=getHeight(rx+1, rz+1); z1=1;
    x2=0; y2=getHeight(rx, rz+1)-y1; z2=1;
    x3=1; y3=getHeight(rx+1, rz)-y1; z3=0;
    xdif=(rx+1)-x; zdif=(rz+1)-z;
  }
  if(y2==0 && y3==0)
  {
    return(y1);
  }
  else
  {
    return((y2*xdif+y3*zdif+y1));
  }
}

Vec3f terrain::getNormal(int x, int z)
{
  return normals[x][z];
}
