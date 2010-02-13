//      objtoheight.cpp
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


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "math.h"
using namespace std;

int sround(float x)
{
  int x2=ceil(x);
  if(x2-x<0.5)
  {return x2;}
  else
  {return int(floor(x));}
}

int main(int argc, char** argv)
{
	if (argc!=3)
		{
			cout<<"usage: objtoheight [objfile] [heightfile]\n";
			return 0;
		}
	ifstream textfile(argv[1]);
  float values[1025][1025];
	if (!textfile.is_open()) 
	{
		cout<<"the file could not be opened for reading\n";
    return 0;
	}
	else 
	{
		cout<<"file "<<argv[1]<<" opened\n";
		string line0;
		size_t loc, loc2;
		string xt, yt, value;
		float xf, yf;
		//int amount=0;
		float z;
		while (!textfile.eof())
    {
			//amount=amount+1;
      getline(textfile,line0);
      if(line0.empty())
      {
        break;
      }
			string line1(line0,2);//*
			//
			loc=line1.find(' ');
			string xt(line1,0,loc);
			stringstream strx(xt);
			strx >> xf;
			int x=sround(xf);
			//cout<<x<<' ';
			string line2(line1,loc+1);
			//
			loc=line2.find(' ');
      loc2=line2.find('.');
			string value(line2,0,loc2+3);
			string yt(line2,loc+1);
			stringstream stry(yt);
			stry >> yf;
			int y=sround(yf);
			
			//
			stringstream strz(value);
      strz >> z;
			//cout<<z<<' ';
			values[x+512][y+512]=z;
      
    }
		textfile.close();
	}
  ofstream writefile(argv[2]);
	if (!writefile.is_open()) 
	{
		cout<<"the file could not be opened for writing\n";
	}
	else 
	{
    cout<<"file "<<argv[2]<<" opened\n";
		for(int y=0; y<1025; y++)
    {
      for(int x=0; x<1025; x++)
      {
        writefile<<values[y][x]<<' ';
      }
      writefile<<'\n';
    }
  writefile.close();
  }
  return 0;
}
