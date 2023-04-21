set(sources
		src/entrePortaisEngine/Window.cpp
		src/entrePortaisEngine/render/Shader.cpp
		src/entrePortaisEngine/Logger.cpp
		src/entrePortaisEngine/render/VertexArrayBuffer.cpp
		src/entrePortaisEngine/IGameNode.cpp
		src/entrePortaisEngine/meshes/EasyMesh.cpp
		src/entrePortaisEngine/IObject.cpp
		src/entrePortaisEngine/Transform.cpp
        src/entrePortaisEngine/plugins/IPlugin.cpp
        src/entrePortaisEngine/plugins/ImGuiPlugin.cpp
        src/entrePortaisEngine/Bezier.cpp
        src/entrePortaisEngine/IScene.cpp
        src/entrePortaisEngine/render/Camera.cpp
        src/entrePortaisEngine/tasks/TaskManager.cpp
        src/utils/matrices.cpp
        src/entrePortaisEngine/render/ShadersManager.cpp
        src/entrePortaisEngine/render/IRenderable.cpp
        src/entrePortaisEngine/render/Renderer.cpp
        src/entrePortaisEngine/render/Texture.cpp
		src/entrePortaisEngine/render/ImageTexture.cpp
        src/entrePortaisEngine/render/TextureManager.cpp
        src/entrePortaisEngine/render/FrameBuffer.cpp
        src/entrePortaisEngine/render/PointLight.cpp

        src/entrePortaisEngine/gui/GuiObject.cpp
        src/entrePortaisEngine/gui/GuiRectangle.cpp


        src/entrePortaisEngine/meshes/IMesh.cpp
        src/entrePortaisEngine/meshes/CubeMesh.cpp
        src/entrePortaisEngine/meshes/CylinderMesh.cpp
        src/entrePortaisEngine/meshes/PlaneMesh.cpp
        src/entrePortaisEngine/meshes/MeshFromObj.cpp
        src/entrePortaisEngine/meshes/ComputeNormals.cpp
        src/entrePortaisEngine/Characters/FirstPersonCharacter.cpp
        src/entrePortaisEngine/Objects/ObjFromFile.cpp
        src/entrePortaisEngine/Objects/AmbientFromTexture.cpp
		src/entrePortaisEngine/physics/PhysXAdapter.cpp
		src/entrePortaisEngine/physics/PhysicsScene.cpp
		src/entrePortaisEngine/physics/PhysicsActor.cpp


		src/testGame/TriScene.cpp
		src/testGame/TriObject.cpp
		src/testGame/WaitAndFinishTask.cpp
		src/testGame/DependencyTask.cpp
		src/testGame/CubeObject.cpp
		src/testGame/CylinderObject.cpp
		src/testGame/Ground.cpp
		src/testGame/Player.cpp
		src/testGame/TeapotObject.cpp
		src/testGame/Cuelho.cpp
		src/testGame/EmptyObject.cpp
		src/testGame/Ball.cpp

		src/utils/utils.cpp
		)

set(exe_sources
		src/main.cpp
		src/labirinto/LoadingScene.cpp
		src/labirinto/MenuScene.cpp
		src/labirinto/HelpScene.cpp
		src/labirinto/CreditsScene.cpp
		src/labirinto/WinScene.cpp
		src/labirinto/LoseScene.cpp
		src/labirinto/GameScene.cpp
		src/labirinto/Player.cpp
		src/labirinto/LabirintoMap.cpp
		src/labirinto/InvisibleBox.cpp
		src/labirinto/Pillar.cpp
		src/labirinto/Key.cpp
		${sources}
		)

set(headers
)

set(test_sources
		#		src/rotations.cpp
		#		src/TransformsTest.cpp
		)
