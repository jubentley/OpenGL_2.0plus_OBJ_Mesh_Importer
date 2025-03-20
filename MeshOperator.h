#ifndef Bridge_def 
#define Bridge_def

#include <stdio.h>
#include <string.h>

#include "GL/glew.h"	//opengl handler
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "ThirdParty\Bitmap.h"
#include "TexLoader.h"
#include "MeshLoader.h"
#include "Shader.h"

extern glm::mat4 projectionmatrix;	//why not use local projections?
extern glm::mat4 viewmatrix;

class MeshOperator
{
public:

	void Render();

	MeshOperator();
	~MeshOperator();

private:
	GLuint LoadTex(char * texname);
	int loaded_textures = 0;
	GLuint meshshader;
	GLuint t[30];//texture handle 
};

class StaticMesh 
{
public:

	StaticMesh(int texnum_p, int texhdl0_p, int texhdl1_p, int texhdl2_p, int texhdl3_p, char * meshname, bool lighting_p);
	void SetModelViewMatrix(glm::vec3 translate, GLfloat rotatedegree, glm::vec3 rotateaxis);
	void Render(GLuint meshshader);//try address

	~StaticMesh();

private:

	GLuint vertexbuffer;
	GLuint vert_amt;
	glm::mat4 modelviewmatrix;
	int texnum;
	int texhdl_a[4];
	bool lighting_on;
};
#endif
