/*############################################################################
##																			##
##	TexLoader																##
##																			##
##	OpenGL 4 Texture Loader	& Setup	Texture Shader Slots					##
##	Basically my attempt to make a threadsafe inline file :)				##
##	Comes from a glut engine (upgraded vic)									##
##	Author:		Justin Bentley										 ##  ## ##
##	Date:		12/2018	- 03/2019									##  ##  ##
##	Version:	2.0												   ######   ##
##																# ##  ##    ##
#############################################################################*/
/*


Imports Texture Files in .bmp only

use 1 to 1 aspect ratio eg 512x512 or 8x8 and power of two amounts 2,4,8,16,32
due to logaritmic calculation (height) of mipmap amount minimum texture size is 2x2

//Sky Textures was courtesy of: https://www.hdri-skies-360.com/skies.html until he asked for my credit card for a free texture
//Sky Textures courtesy of https://www.cgskies.com/skies.php
//Other Textures courtesy of https://www.textures.com  also deviantart and a few other place but mainly all in all everyone but shutterstok
//And ofcourse https://www.solarsystemscope.com/textures/ for the A-grade planets


*/

#ifndef TexLoader_def
#define TexLoader_def

#pragma warning(disable : 4996) // Reduce unsafe warning messages

#include <stdio.h>
#include <string.h>
//#include "GLFW\glfw3.h"


class TexLoader
{
public:

	void GlobalRunOnceTextureHints();

	void LoadTexture(char * texname, GLuint &texhandle, int ClampTile);

	TexLoader();

	~TexLoader();

};
#endif
//Now Redundant was Inline but was too troublesome so I classed it

/**INCLUDE IN "PROTECTED" CALLING .CPP OBJECT

void LoadTextures(){

const int textures = 3;			
static GLuint tex[3];	

GlobalRunOnceTextureHints(); /Only Call Once

bmp[0] = new CBitmap("../Textures/Mario01.bmp");
bmp[1] = new CBitmap("../Textures/Smile.bmp");
bmp[2] = new CBitmap("../Textures/mossyTexture.bmp");

LoadTexture(bmp[0], 0, tex[0], 0);
LoadTexture(bmp[1], 1, tex[1], 0);
LoadTexture(bmp[2], 2, tex[2], 0);

for (int i = 0; i < textures; i++) { delete bmp[i]; }

}

void Render(){
//		UNCONFIRMED
for (int i = 0; i < textures; i++) {
	glActiveTexture(GL_TEXTURE0 + i);		//set active texture slot in gpu, must be called before setting tex
	glBindTexture(GL_TEXTURE_2D, tex[i]);
	}
}

void ~YourObjectName(){
	for (int i = 0; i < textures; i++) { glDeleteTextures(1, &tex[i]); }

}


*/











/*
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, Smile);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//BI-LINIEAR
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//linear interp //bi-linear //average neigbour texels
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

*/