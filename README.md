# OpenGL_2.0plus_OBJ_Mesh_Importer

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


