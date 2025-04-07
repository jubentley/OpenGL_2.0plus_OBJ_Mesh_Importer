# OpenGL 2.0+ .OBJ Mesh Importer

.obj Mesh Importer for OpenGL 2.0+

The .obj mesh importer is situated in MeshLoader.cpp/MeshLoader.h 

In this configuration MeshOperator is instanciated once:

```C++
  // Main.cpp
  #include "MeshOperator.h"  // brings in MeshOperator.cpp

  MeshOperator *meshoperator;
  meshoperator = new MeshOperator();

  ///

  delete meshoperator;
```

And during gameplay various Renders are called every frame:

```C++
  // Main.cpp
		if (state == 1) {
			meshoperator->Render(Right, EyePos);
			meshoperator->RenderArms(Right, EyePos, RightEye);
			if (Replay) {
				meshoperator->RenderGhost(Right, EyePos, RightEye);
				timer++;
				if ((timer % ReplaySpd) == 0) { RendBufferCt++; }//Replay Slowdown
				//was in meshoperator->RenderGhost
				if (RendBufferCt >= RBufferSize) { Replay = false; }
			}
		}
```
Which are accepted by MeshOperator:

```C++
//Batch Render for all static mesh's
void MeshOperator::Render(
	PROJECTION_VIEW_MATRIXES eyeside,
	VIEW_TRANSLATES EyePos) {

	///

	}

```

But first, during the instanciation of MeshOperator:

```C++
  //Main.cpp
  MeshOperator *meshoperator;
  meshoperator = new MeshOperator();
```

MeshOperator calls StaticMesh (included within MeshOperator.cpp) for every Mesh found within the game, though what is of prime importance is the textures that it will use are loaded beforehand and passed into it. StaticMesh in turn calls MeshLoader and deletes upon use, this is because at this stage the Vertex Buffer data has been loaded in VRAM and its GL handle is stored in <b>vertexbuffer</b>.
```C++
StaticMesh *dojo_inner;

MeshOperator::MeshOperator()
{
	MeshOperator::LoadTex(t[0], "../../GameObjects/Textures/MetalFloor8192.bmp");//impacts load time (mainly resolution)
	MeshOperator::LoadTex(t[1], "../../GameObjects/Textures/Wood01px8192.bmp");
	MeshOperator::LoadTex(t[2], "../../GameObjects/Textures/Wood02px1024.bmp");

	dojo_inner = new StaticMesh(3, t[3], t[2], t[1], 0, "../../GameObjects/Mesh/DojoInner", true);
}	

//instanciate mesh with tex ids, file name/directory etc
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
```

Which, for interest is later referenced in the Render section:

```C++
//renders a static mesh using its own buffers
void StaticMesh::Render(
	GLuint meshshader,
	glm::mat4 projection_matrix,
	glm::mat4 view_matrix,
	glm::mat4 modelviewmatrix_additional,
	glm::mat4 normal_matrix_additional
	) {

	//****************TEXTURES*******************
	for (int i = 0; i < texnum; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texhdl_a[i]);
	}
	//****************SHADER*******************
	glUseProgram(meshshader);
	GLint uniformtexture0 = glGetUniformLocation(meshshader, "u_Texture0");
	GLint uniformtexture1 = glGetUniformLocation(meshshader, "u_Texture1");
	GLint uniformtexture2 = glGetUniformLocation(meshshader, "u_Texture2");
	GLint uniformtexture3 = glGetUniformLocation(meshshader, "u_Texture3");
	GLint uniformlightingflag = glGetUniformLocation(meshshader, "u_LightingFlag");

	GLint uniformprojection = glGetUniformLocation(meshshader, "u_MVP");
	GLint uniformnormal = glGetUniformLocation(meshshader, "u_normalmatrix");

	//****************VERT_BUFFER_SETUP*******************
	GLuint element_size = sizeof(GLfloat) * 13;//size of one vertex and attributes, could use vao vbo w/e its called
	//(vert) x y z (tex) s t (normal) x y z (color)r g b a (texcolorkey) i (All GLfloat's)(13 in total)
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
	glm::mat4 objectorigin = projection_matrix *
		view_matrix * (modelviewmatrix_additional * modelviewmatrix);

	glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(modelviewmatrix*normal_matrix_additional)));//will work with identity if no rotate
	glUniform1i(uniformtexture0, 0);//handle, gl texture slot
	glUniform1i(uniformtexture1, 1);//handle, gl texture slot
	glUniform1i(uniformtexture2, 2);//handle, gl texture slot
	glUniform1i(uniformtexture3, 3);//handle, gl texture slot
	glUniform1i(uniformlightingflag, lighting_on);//handle, gl texture slot
	glUniformMatrix4fv(uniformprojection, 1, GL_FALSE, &objectorigin[0][0]);//handle, num matx's, transpose, *projmatrix
	glUniformMatrix3fv(uniformnormal, 1, GL_FALSE, &normalmatrix[0][0]);

	//****************DRAW*******************
	glDrawArrays(GL_TRIANGLES, 0, vert_amt);//draw in triangles starting from 0 vert_amt of vertices from the currently bound buffer
}
```
