/*############################################################################
##																			##
##	TexLoader																##
##																			##
##	OpenGL 4 Texture Loader													##
##	Uses Bitmap.h & only loads .bmp											##
##	Was Glut & was inline, has gone through a lot of revisions				##
##	Author:		Justin Bentley			              #####  ######  ###### ##
##	Date:		12/2018	- 03/2019		            ##      ##  ##  ##  ##  ##
##	Version:	3.0						           ##      ######  ######   ##
##	Running on:	OpenGL 2-4+				       ##  #####  ##      ##        ##
#############################################################################*/

#include <stdio.h>
#include <string>
#include "GL/glew.h"	//opengl handler
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "ThirdParty\Bitmap.h"
//#include "GLFW\glfw3.h"
//#include "GLM/glm.hpp"
//#include "GLM/gtc/matrix_transform.hpp"

#include <math.h> //used to log down a number for mipmaps

#include "TexLoader.h"


//Call these once in your program for Texture Quality Hints
//Unsure as to whether this makes a difference and some are not supported in OpenGL 2+
void TexLoader::GlobalRunOnceTextureHints() {

	GLuint tx_glerror;
	//debateable if these need to be called or are usefull

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);			//dont need to be called repeatadly
	tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL CALL ERROR, GL_LINE_SMOOTH_HINT: %i\n", tx_glerror); }
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);		//
	tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL CALL ERROR, GL_POINT_SMOOTH_HINT: %i\n", tx_glerror); }
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);		//
	tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL CALL ERROR, GL_POLYGON_SMOOTH_HINT: %i\n", tx_glerror); }
	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);	//
	tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL CALL ERROR, GL_TEXTURE_COMPRESSION_HINT: %i\n", tx_glerror); }
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);			//error in OpenGL 4
	tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL CALL ERROR, GL_GENERATE_MIPMAP_HINT: %i\n", tx_glerror); }
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//error in OpenGL 4
	tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL CALL ERROR, GL_PERSPECTIVE_CORRECTION_HINT: %i\n", tx_glerror); }

}


//Load Individual Texture; 
//filename ..\Textures\****.bmp, TexHandle num, GL Tex Slot num, Clamp = 0 / Tile = 1
//will only take bitmap
void TexLoader::LoadTexture(
	char* texname,
	GLuint &texhandle,
	int ClampTile) {

	//GLuint t2;
	CBitmap *bmp = new CBitmap(texname);

	glActiveTexture(GL_TEXTURE0);//set active texture slot in gpu, must be called before setting tex

	glGenTextures(1, &texhandle);
	glBindTexture(GL_TEXTURE_2D, texhandle);

	glTexStorage2D(GL_TEXTURE_2D,
		//9,
		log2(bmp->GetHeight()),	//reverse exponent image dimensions for mipmaps, minimum px is 2x2
		GL_RGBA8,		//check
		bmp->GetWidth(),
		bmp->GetHeight()
	);
	//printf("log2 of 2 is %lf texture loaded\n", (log2(2)));	//:)
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0,
		0,
		bmp->GetWidth(),
		bmp->GetHeight(),
		GL_RGBA,
		GL_UNSIGNED_BYTE,				// format for GL to store it
		bmp->GetBits()
	);

	glGenerateMipmap(GL_TEXTURE_2D);
	//	glGenerateTextureMipmap();	//alternative, requires opengl 4.5 nvidia 10XX

	if (ClampTile == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}
	if (ClampTile == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//causes creeping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);//sharpest
																					 //	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);//worth a try
	delete bmp;

	GLuint tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL ERROR TEXTURE (PROBABLY) 404 %i\n", tx_glerror); __debugbreak(); }//internal error, will literally tell u how much u overran the loop by
	//return t2;
}
TexLoader::TexLoader() {

}
TexLoader::~TexLoader() {

}