/*############################################################################
##																			##
##	MeshOperator															##
##																			##
##	MeshOperator.cpp and StaticMesh.cpp combined obj						##
##	state handler for multiple mesh's instanciated by StaticMesh			##
##																			##
##	Author:		Justin Bentley			              #####  ######  ###### ##
##	Date:		04/2019					            ##      ##  ##  ##  ##  ##
##	Version:	1.0						           ##      ######  ######   ##
##	Running on:	OpenGL 2-4+				       ##  #####  ##      ##        ##
#############################################################################*/



/*
//Mesh Set : MeshOperator.cpp(inc StaticMesh), MeshLoader.cpp, TexLoader.h, Shader.cpp

A Note of Materials; this will not import any materials from maya(it does import diffuse but currently its not used)
nor textures although it will group materials(/ textures) into their respective sets through the mesh tool therefore if
you find the right order in which the textures appear enter them in order in the instanciation(may change to shared
textures to be passed in render) of the "StaticMesh" and they will be drawn in their sets as they were in Maya.


//#include "ThirdParty\Bitmap.h"
//#include "TexLoader.h"
//#include "MeshLoader.h"
//#include "Shader.h"

*/
#include "MeshOperator.h"

Shader *shader;

void StaticMesh::Render(
	GLuint meshshader
	) {

	//****************TEXTURES*******************
	for (int i = 0; i < texnum; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texhdl_a[i]);
	}
	//****************SHADER*******************
	glUseProgram(meshshader);
	GLint uniformtexture0 = glGetUniformLocation(meshshader, "u_Texture0");		//add later, keep it easy
	GLint uniformtexture1 = glGetUniformLocation(meshshader, "u_Texture1");		//add later, keep it easy
	GLint uniformtexture2 = glGetUniformLocation(meshshader, "u_Texture2");		//add later, keep it easy
	GLint uniformtexture3 = glGetUniformLocation(meshshader, "u_Texture3");		//add later, keep it easy
	GLint uniformlightingflag = glGetUniformLocation(meshshader, "u_LightingFlag");		//add later, keep it easy

	GLint uniformprojection = glGetUniformLocation(meshshader, "u_MVP");
	GLint uniformnormal = glGetUniformLocation(meshshader, "u_normalmatrix");	

	//****************VERT_BUFFER_SETUP*******************
	GLuint element_size = sizeof(GLfloat) * 13;//size of one vertex and attributes, could use vao vbo w/e its called
	//(vert) x y z	(tex) s t		(normal)x y z		(color)r g b a		(texcolorkey) i		(All GLfloat's)(13 in total)
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glEnableVertexAttribArray(0); //verts
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, element_size, 0);
	glEnableVertexAttribArray(1); //tex's
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, element_size, (GLvoid *)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(2); //normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, element_size, (GLvoid *)(sizeof(GLfloat) * 5));
	glEnableVertexAttribArray(3); //color
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, element_size, (GLvoid *)(sizeof(GLfloat) * 8));
	glEnableVertexAttribArray(4); //mat id
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, element_size, (GLvoid *)(sizeof(GLfloat) * 12));
	//index(AtPoin num), components, data type, normalised, stride (dist tween [0][0] >> [1][0]), pointer (dist first ap origin >> this origin)

	//****************UNIFORM_SEND*******************
	glm::mat4 objectorigin = projectionmatrix * viewmatrix * modelviewmatrix;//not sure to multithread or gpu shade ?? //goto update
	glm::mat3 normalmatrix = /*glm::mat3(modelviewmatrix) **/ glm::transpose(glm::inverse(glm::mat3(/*viewmatrix * */modelviewmatrix)));
	glUniform1i(uniformtexture0, 0);//handle, gl texture slot
	glUniform1i(uniformtexture1, 1);//handle, gl texture slot
	glUniform1i(uniformtexture2, 2);//handle, gl texture slot
	glUniform1i(uniformtexture3, 3);//handle, gl texture slot
	glUniform1i(uniformlightingflag, lighting_on);//handle, gl texture slot
	glUniformMatrix4fv(uniformprojection, 1, GL_FALSE, &objectorigin[0][0]);//handle, num matx's, transpose, *projmatrix
	glUniformMatrix3fv(uniformnormal, 1, GL_FALSE, &normalmatrix[0][0]);

	//****************DRAW*******************
	glDrawArrays(GL_TRIANGLES, 0, vert_amt);//draw in triangles starting from 0 vert_amt of vertices from the currently bound buffer

	//glDrawElements(GL_TRIANGLES, index_size, GL_UNSIGNED_INT, nullptr);//(if) draw vert buffer using indexbuffer
}

//******************DEFINE_STATIC_MESH'S****************************
StaticMesh *bridge, *dojo_inner, *dojo_outer, *dojo_base, *polygons, *skybox, *planet0, *planet1, *planet2;
MeshOperator::MeshOperator()
{
	//may really wanna combine shared textures
	shader = new Shader();
	meshshader = shader->CreateMeshShader();

	GlobalRunOnceTextureHints();

	//****************************TEXTURES***********************************
	t[0] = MeshOperator::LoadTex("../../GameObjects/Textures/MetalFloor8192.bmp");
	t[1] = MeshOperator::LoadTex("../../GameObjects/Textures/Wood01px8192.bmp");
	t[2] = MeshOperator::LoadTex("../../GameObjects/Textures/Wood02px1024.bmp");
	t[3] = MeshOperator::LoadTex("../../GameObjects/Textures/FourPixelsOfGloriousWhite.bmp");
	t[4] = MeshOperator::LoadTex("../../GameObjects/Textures/Jade01_512.bmp");
	t[5] = MeshOperator::LoadTex("../../GameObjects/Textures/GrassPavement01px1024.bmp");
	t[6] = MeshOperator::LoadTex("../../GameObjects/Textures/pavingTexture.bmp");
	t[7] = MeshOperator::LoadTex("../../GameObjects/Textures/Sky/CGSky_SkyDay02.bmp");

//	t[8] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_earth_daymap.bmp");

	t[8] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_earth_daymap.bmp");
	t[9] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_jupiter.bmp");
	t[10] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_venus_surface.bmp");
	t[11] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/2k_neptune.bmp");
	t[12] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_moon.bmp");
	t[13] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_sun.bmp");
	t[14] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_saturn.bmp");
	t[15] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_mars.bmp");
	t[16] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_mercury.bmp");
	t[17] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_earth_clouds.bmp");
	t[18] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_earth_nightmap.bmp");
	t[19] = MeshOperator::LoadTex("../../GameObjects/Textures/Planetoids/8k_stars_milky_way.bmp");
	t[20] = MeshOperator::LoadTex("../../GameObjects/Textures/jbn.bmp");

	//***************************INSTANCIATE_MESH**************************************
	dojo_inner = new StaticMesh(3, t[3], t[3], t[1], 0, "../../GameObjects/Mesh/DojoInner", true);
	dojo_outer = new StaticMesh(3, t[1], t[4], t[20], 0, "../../GameObjects/Mesh/DojoOuter", true);
	dojo_base = new StaticMesh(4, t[4], t[5], t[1], t[4], "../../GameObjects/Mesh/DojoBase", true);
	skybox = new StaticMesh(1, t[7], 0, 0, 0, "../../GameObjects/Mesh/SkyBox", false);
	//bridge = new StaticMesh(1, t[0], 0, 0, 0, "../../GameObjects/Mesh/Bridge", true);
	planet0 = new StaticMesh(4, t[8], t[9], t[10], t[11], "../../GameObjects/Mesh/Planet0", true);
	planet1 = new StaticMesh(4, t[12], t[13], t[14], t[15], "../../GameObjects/Mesh/Planet1", true);
	planet2 = new StaticMesh(4, t[16], t[17], t[18], t[19], "../../GameObjects/Mesh/Planet2", true);

	//polygons = new StaticMesh(polygontex, polygonstexnum, "../../GameObjects/Mesh/Polygons");

	//***************************INITIAL_TRANSLATES**********************************
	dojo_inner->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	dojo_outer->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	dojo_base->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	skybox->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 170.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//bridge->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	planet0->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));//can use false
	planet1->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));//can use false
	planet2->SetModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));//can use false

	//polygons->SetModelViewMatrix(glm::vec3(0.0f, -4.0f, 50.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

}
void MeshOperator::Render() {
//	bridge->Render(meshshader);
	dojo_inner->Render(meshshader);
	dojo_outer->Render(meshshader);
	dojo_base->Render(meshshader);
//	polygons->Render(meshshader, true);	//<<something majorly wrong wiyh polygons
	skybox->Render(meshshader);
	planet0->Render(meshshader);
	planet1->Render(meshshader);
	planet2->Render(meshshader);

}
MeshOperator::~MeshOperator()
{
	delete bridge;
	delete dojo_inner;
	delete dojo_outer;
	delete dojo_base;
	delete polygons;
	delete skybox;
	delete planet0, planet1, planet2;
	delete shader;

	for (int i = 0; i < loaded_textures; i++) { glDeleteTextures(1, &t[i]); }//delete textures, maybe unloop use use gldelete amt

}

StaticMesh::StaticMesh(
	int texnum_p,
	int texhdl0_p,
	int texhdl1_p,
	int texhdl2_p,
	int texhdl3_p,
	char* meshname,
	bool lighting_p
	) {

	StaticMesh::lighting_on = lighting_p;
	StaticMesh::texnum = texnum_p;
	StaticMesh::texhdl_a[0] = texhdl0_p;
	StaticMesh::texhdl_a[1] = texhdl1_p;
	StaticMesh::texhdl_a[2] = texhdl2_p;
	StaticMesh::texhdl_a[3] = texhdl3_p;

	MeshLoader *meshloader = new MeshLoader(meshname);
	vertexbuffer = meshloader->CreateBuffer(&vert_amt);
	printf("%s Vert Amt = %d \n", meshname, vert_amt);
	delete meshloader;//creates vertexbuffer of mesh and vertex amt

}
GLuint MeshOperator::LoadTex(char* texname) {
	GLuint gltexhandle;
	CBitmap *cbitmaptex = new CBitmap(texname);				//for threadsafe reason idk
	/*TexLoader*/LoadTexture(cbitmaptex, 0, gltexhandle, 1);//second variable needs to go
	delete cbitmaptex;
	loaded_textures++;
	return gltexhandle;
}
void StaticMesh::SetModelViewMatrix(
	glm::vec3 translate,
	GLfloat rotatedegree,
	glm::vec3 rotateaxis) {
	modelviewmatrix = glm::translate(glm::mat4(1.0f), translate) * glm::rotate(glm::mat4(1.0f), glm::radians(rotatedegree), rotateaxis);
}
StaticMesh::~StaticMesh() {
}

/*







*/









