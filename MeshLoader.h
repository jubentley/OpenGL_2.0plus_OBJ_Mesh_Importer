/*############################################################################
##																			##
##	MeshLoader																##
##																			##
##	Used in combination with MeshLoader.cpp									##
##																			##
##																			##
##	Author:		Justin Bentley										 ##  ## ##
##	Date:		04/2019												##  ##  ##
##	Version:	1.0												   ######   ##
##																# ##  ##    ##
#############################################################################*/

#ifndef MeshLoader_def 
#define MeshLoader_def 

#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define LINELEN 400 //LINE OF FILE MAX LEN
#define MATNAMELEN 50	//SIZE OF MATERIAL NAME
#define MAXMATS 100		//AMOUNT OF DIFFERENT MATERIALS
#define DIRLEN 50	//DIRECTORY NAME LENGTH
#define KEYLEN 80 //vt vx usemtl etc
#define MATCHANGES 1000 //AMOUNT OF TIMES MATERIALS CHANGE

//VLA ALTERNATIVE
#define VERTEX 1000000	//MUST BE DONE MANUALLY (SORRY) (BELIEVE IT OR NOT 2M x 4 BYTE = 8MB)
#define NORMALS 1000000	//MAKE SURE U PLUS 1 AS TXT ARRAY START AT 1
#define FACES 1000000	//FACELINES
#define TEXTURES 1000000

//#include <stdio.h>
//#include <string.h>

#include "GL\glew.h" 
//#include "glut.h"



class MeshLoader
{
public:
//	void MeshLoader::Render(GLfloat X, GLfloat Y, GLfloat Z, GLfloat YAW, GLfloat PITCH, GLfloat ROLL);	
	GLuint CreateBuffer(GLuint *vert_amt);
	MeshLoader(char* dir);
	~MeshLoader();

protected:
private:

	void LoadMeshData(char* dirobj);
	void LoadMattData(char* dirmtl);	
	void PrecalculateElements(FILE *infile, char *line, char *var);//isolated function to number a file of f, v, vt etc 

	char printf_output_filename[DIRLEN];

	int obj_line_f_point = 1;	//.OBJ ARRAYS START FROM ONE
	char MatIDObj[MAXMATS][MATNAMELEN];	//
	char MatIDMtl[MATCHANGES][MATNAMELEN];	//
	int MatChangeMarker[MATCHANGES];	//FACE NUM TO CHANGE MAT
	int MatChangeMarkerID[MATCHANGES];	//Kd Ka Ks TO USE
	int MatChangeMarkerPoint = 0;	//INCREMENTER
	int MatPointObj = 0;
	int MatPointMtl = -1;//BEING A PRE-INCREMENTER
//	GLfloat Kd[MAXMATS][4];	//DIFFUSE
	GLfloat Ka[MAXMATS][4];	//AMBIENT
//	GLfloat Ks[MAXMATS][4];	//SPECULAR
//	GLint Ns[MAXMATS];		//SPECULAR EXPONENT		//TYPE BLINN NOT PRESENT

	//VLA'S
	//GLfloat **vertexs;
	//GLfloat **normals;
	//int ***face;

	//	VLA ALTERNATIVE
	GLfloat vertexs[VERTEX][3];//literally just under 8mb * 3
	GLfloat normals[NORMALS][3];
	GLfloat texture[TEXTURES][2];
	int face[FACES][3][3];
};
#endif
