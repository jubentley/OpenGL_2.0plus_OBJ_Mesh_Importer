/*############################################################################
##																			##
##	MeshLoader																##
##																			##
##	Imports and creates buffers for .obj and .mtl Mesh Files				##
##	Converted from my Glut .obj importer									##
##	Author:		Justin Bentley												##
##	Date:		2018 >> 04/2019			              ###### ######  ###### ##
##	Version:	2.0 (converted from glut)           ###     ##  ##  ##  ##  ##
##	Running on:	OpenGL 2-4+					       ##      ######  ######   ##
##										        #   ##### ##      ##        ##
##############################################################################

The idea is to instanciate this with the mesh and mtl filename and directory,
Create and retrieve the buffer, then kill this object (or not)

Due to OpenGL only being able to index a vertex and all other attributes of it at the same time (one line of
buffer element) the elements of the created buffer are written sequentially, while vertexs are repeated most majorly
the tex co-ordinates differ and therefore the buffers cannot be written with indexes and in OpenGL 4 creating
an indexbuffer would be pointless.


//In Maya;
//Prefferable to Combine
//Groups, Point Groups and Smoothing off
//Materials, Normals on
//Will only read in Ambient (Ka)
//Only Triangles, so Triangulate
//RGB Color 0-1

//Favours Heavy VRAM
*/


#include <stdio.h>
#include <string.h>




#include "MeshLoader.h"


///Glut Renderer
//void MeshLoader::Render(GLfloat X,GLfloat Y,GLfloat Z,GLfloat YAW, GLfloat PITCH, GLfloat ROLL) {
//	glFrontFace(GL_CCW);	//PREFER NOT TO BE HERE
////	glPolygonMode(GL_FRONT, GL_FILL);
////	glPolygonMode(GL_BACK, GL_LINE);		
////	glEnable(GL_LIGHTING);
//
//	glPushMatrix();
//	glTranslatef(X,Y,Z);
//	glRotatef(YAW, 0, 1, 0);
//	if (PITCH != 0) { glRotatef(PITCH, 0, 0, 1); }
//	if (ROLL != 0) { glRotatef(ROLL, 1, 0, 0); }
//	glBegin(GL_TRIANGLES);
//	//REF 4295/6019/8570 4297/5987/8486 4296/5984/8480 VT/TX/NR
//	MatChangeMarkerPoint = 0;
//	for (int i = 1; i < obj_line_f_point; i++) {
//		if (MatChangeMarker[MatChangeMarkerPoint] == i) {
//			glEnd();
//			
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, 
//				&Ka[MatChangeMarkerID[MatChangeMarkerPoint]][0]);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, 
//				&Ks[MatChangeMarkerID[MatChangeMarkerPoint]][0]);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, 
//				&Kd[MatChangeMarkerID[MatChangeMarkerPoint]][0]);
//			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, ((int)128 * 0.5));
//
//			glBegin(GL_TRIANGLES);
//			MatChangeMarkerPoint++;
//		}
//		glNormal3f(
//			normals[face[i][0][2]] [0],
//			normals[face[i][0][2]] [1],
//			normals[face[i][0][2]] [2]	);
//		glVertex3f(
//			vertexs[face[i][0][0]] [0],
//			vertexs[face[i][0][0]] [1],
//			vertexs[face[i][0][0]] [2]);
//		glNormal3f(
//			normals[face[i][1][2]] [0],
//			normals[face[i][1][2]] [1],
//			normals[face[i][1][2]] [2]);
//		glVertex3f(
//			vertexs[face[i][1][0]] [0],
//			vertexs[face[i][1][0]] [1],
//			vertexs[face[i][1][0]] [2]);
//		glNormal3f(
//			normals[face[i][2][2]] [0],
//			normals[face[i][2][2]] [1],
//			normals[face[i][2][2]] [2]);
//		glVertex3f(
//			vertexs[face[i][2][0]] [0],
//			vertexs[face[i][2][0]] [1],
//			vertexs[face[i][2][0]] [2]);
//	}	
//	glEnd();
//	glPopMatrix();
//}

//redundant function, not used but useful
//will scan a .obj file for number of vertices/faces/normals
//was initially used to get values to set buffer maximums
void MeshLoader::PrecalculateElements(FILE *infile, char *line, char *key) {

	int verticesct = 2, normalsct = 2, facesct = 2;
	while (fgets(line, LINELEN, infile) > 0) {
		sscanf(line, "%s", key);
		if (strcmp(key, "v")) { verticesct++; }
		if (strcmp(key, "vn")) { normalsct++; }
		if (strcmp(key, "f")) { facesct++; } //(key[0] == 'f')
	}
	printf("Vertices = %d, Normals = %d, Faces = %d\n", verticesct, normalsct, facesct);
	fseek(infile, 0, SEEK_SET);	//BREAK HERE FOR VERTS READOUT

}
//Will create a OpenGL 2+ Vertex Buffer from the (previously loaded) internal buffer of this class
GLuint MeshLoader::CreateBuffer(GLuint *vert_amt) {

//  (vert) x y z	(tex) s t		(normal)x y z		(color)r g b a		(texcolorkey) i		(All GLfloat's)(13 in total)
	
	const GLuint buffer_element_size = sizeof(GLfloat) * 13;	//(see above)

	const GLuint obj_line_size = buffer_element_size * 3;	//.obj has 3 per line

	*vert_amt = (((obj_line_f_point - 1) * 3)); //((lines - offsets) * verts per line) * element_size, offsets = start from 1 and post increment at end (maybe i get onto it)

	GLuint VertexBufferID;
	//int indexcounter = 0;


	glGenBuffers(1, &VertexBufferID);//1 buffer called
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, obj_line_size * (obj_line_f_point - 1), NULL, GL_STATIC_DRAW);//obj_line -1, first value written to 1, last value increments +1
	GLfloat activemats[4]; /*GLfloat Alpha = 1.0f;*/ MatChangeMarkerPoint = 0;  int MatId = 0;
	for (int i = 1; i < obj_line_f_point; i++)
	{
		if (MatChangeMarker[MatChangeMarkerPoint] == i) {
			activemats[0] = Ka[MatChangeMarkerID[MatChangeMarkerPoint]] [0];
			activemats[1] = Ka[MatChangeMarkerID[MatChangeMarkerPoint]] [1];
			activemats[2] = Ka[MatChangeMarkerID[MatChangeMarkerPoint]] [2];
			activemats[3] = Ka[MatChangeMarkerID[MatChangeMarkerPoint]] [3];
			activemats[4] = MatChangeMarkerID[MatChangeMarkerPoint];//this will give an ID for mat that refers to order in .mtl
			MatChangeMarkerPoint++;
		}
		GLfloat temp_element[obj_line_size/*<<should this work*/] = {
			vertexs[face[i][0][0]] [0], vertexs[face[i][0][0]] [1],  vertexs[face[i][0][0]] [2], 
			texture[face[i][0][1]] [0],	texture[face[i][0][1]] [1],																		
			normals[face[i][0][2]] [0], normals[face[i][0][2]] [1],  normals[face[i][0][2]] [2],
			activemats[0], activemats[1], activemats[2], activemats[3], activemats[4],	//rgb mats and a consistant mat id
			vertexs[face[i][1][0]] [0], vertexs[face[i][1][0]] [1],  vertexs[face[i][1][0]] [2],
			texture[face[i][1][1]] [0],	texture[face[i][1][1]] [1],
			normals[face[i][1][2]] [0], normals[face[i][1][2]] [1],  normals[face[i][1][2]] [2],
			activemats[0], activemats[1], activemats[2], activemats[3], activemats[4],
			vertexs[face[i][2][0]] [0], vertexs[face[i][2][0]] [1],  vertexs[face[i][2][0]] [2],
			texture[face[i][2][1]] [0],	texture[face[i][2][1]] [1],
			normals[face[i][2][2]] [0], normals[face[i][2][2]] [1],  normals[face[i][2][2]] [2],
			activemats[0], activemats[1], activemats[2], activemats[3], activemats[4]//A-OK
		};
		glBufferSubData(GL_ARRAY_BUFFER, obj_line_size * (i - 1), obj_line_size, &temp_element);			//consider glMapBuffer, memcpy instead
		GLuint tx_glerror = glGetError(); if (tx_glerror > 0) { printf("GL ERROR BUFFER FILL %s %i\n", &printf_output_filename, tx_glerror); __debugbreak(); }//internal error, will literally tell u how much u overran the loop by
		//indexcounter++;
	}
	//printf("indexcounter = %d, indexcounter x 3 = %d\n", indexcounter, (indexcounter * 3));
	return VertexBufferID;
}
//Loads material data from a .obj included .mtl file
//Currently loads Ambient (Ka) & Transparency (Tf) and will sort eventual buffer into 2-4
//mat groups
void MeshLoader::LoadMattData(char* dirmtl) {
	FILE *infile;
	char line[LINELEN], key[KEYLEN], inputmatname[MATNAMELEN];

	infile = fopen(dirmtl, "rb");//READ/BINARY

	while (fgets(line, LINELEN, infile) > 0) {
		sscanf(line, "%s", key);
		if (strcmp(key, "newmtl") == 0) {
			MatPointMtl++;
			sscanf(line, "%s %s", key, inputmatname);
			strcpy(MatIDMtl[MatPointMtl], inputmatname);//STACKS MATERIAL NAMES IN .MTL
		}
		//if (strcmp(key, "Kd") == 0) {
		//	sscanf(line, "%s %f %f %f", key,
		//		&Kd[MatPointMtl][0],
		//		&Kd[MatPointMtl][1],
		//		&Kd[MatPointMtl][2]);
		//	Kd[MatPointMtl][3] = 1.0;
		//}
		if (strcmp(key, "Ka") == 0) {
			sscanf(line, "%s %f %f %f", key,
				&Ka[MatPointMtl][0],
				&Ka[MatPointMtl][1],
				&Ka[MatPointMtl][2]);
			Ka[MatPointMtl][3] = 1.0;//incase Tf not present
		}
		if (strcmp(key, "Tf") == 0) {
			GLfloat alpha_g, alpha_b;//get this working then remove
			sscanf(line, "%s %f %f %f", key,
				&Ka[MatPointMtl][3],//alpha red, implemented as alpha rgb
				&alpha_g,
				&alpha_b);
		}
		//if (strcmp(key, "Ks") == 0) {
		//	sscanf(line, "%s %f %f %f", key,
		//		&Ks[MatPointMtl][0],
		//		&Ks[MatPointMtl][1],
		//		&Ks[MatPointMtl][2]);
		//	Ks[MatPointMtl][3] = 1.0;
		//}
	}
	fclose(infile);
}

//Loads Mesh data from .obj to internal buffer 
void MeshLoader::LoadMeshData(char* dirobj) {

	FILE *infile;
	char line[LINELEN], key[KEYLEN], inputmatname[MATNAMELEN];
	int vertexpoint = 0, normalpoint = 0, texturepoint = 0;

	infile = fopen(dirobj, "rb");//READ/BINARY

//	MeshLoader::PrecalculateElements(infile, line, key);

	while (fgets(line, LINELEN, infile) > 0) {
		sscanf(line, "%s", key);
		if (strcmp(key, "usemtl") == 0) {
			sscanf(line, "%s %s", key, inputmatname);
//			strcpy(MatIDObj[MatPointObj], inputmatname);//?
			MatChangeMarker[MatChangeMarkerPoint] = obj_line_f_point;
			for (int i = 0; i <= MatPointMtl; i++) {
				if (strcmp(inputmatname, MatIDMtl[i]) == 0) {
					MatChangeMarkerID[MatChangeMarkerPoint] = i;
					break;
				}				
			}
			MatPointObj++;
			MatChangeMarkerPoint++;
		}
		if (strcmp(key, "vt") == 0) {
			texturepoint++;
			sscanf(line, "%s %f %f",
				key, &texture[texturepoint][0], &texture[texturepoint][1]);
		}
		if (strcmp(key, "v") == 0) {
			vertexpoint++;
			sscanf(line, "%s %f %f %f",
				key, &vertexs[vertexpoint][0],
				&vertexs[vertexpoint][1], &vertexs[vertexpoint][2]);
		}
		if (strcmp(key, "vn") == 0) {
			normalpoint++;
			sscanf(line, "%s %f %f %f",
				key, &normals[normalpoint][0],
				&normals[normalpoint][1], &normals[normalpoint][2]);
		}
		if (strcmp(key, "f") == 0) {
			for (int i = 0; i < strlen(line); i++) {	//hehe
				if (line[i] == '/') { line[i] = ' '; }
			}
			sscanf(line, "%s %d %d %d %d %d %d %d %d %d",	//takes a whole line, refers to 3 vec3 vertexs's
				key,
				&face[obj_line_f_point][0][0], &face[obj_line_f_point][0][1], &face[obj_line_f_point][0][2], 
				&face[obj_line_f_point][1][0], &face[obj_line_f_point][1][1], &face[obj_line_f_point][1][2],
				&face[obj_line_f_point][2][0], &face[obj_line_f_point][2][1], &face[obj_line_f_point][2][2]);
			obj_line_f_point++;	//should change to a pre incrementer
		}
	}
	fclose(infile);
}

//takes a filename/directory and will load Triangle Data into buffers
//.mtl Material and .obj Vertex Data
MeshLoader::MeshLoader(char* dir)
{
	strcpy(printf_output_filename, dir);

	char dirobj[DIRLEN], dirmtl[DIRLEN];
	strcpy(dirobj, dir); strcat(dirobj, ".obj");
	strcpy(dirmtl, dir); strcat(dirmtl, ".mtl");

	MeshLoader::LoadMattData(dirmtl);
	MeshLoader::LoadMeshData(dirobj);

}
MeshLoader::~MeshLoader()
{
	// DELETE VLA
//	delete[] vertexs, normals, face;		//probably wrong delete type call
//	use free dumbass
}

//int **arr = new int*[sizeX];
//for (int i = 0; i < sizeX; i++)
//	arr[i] = new int[sizeY];