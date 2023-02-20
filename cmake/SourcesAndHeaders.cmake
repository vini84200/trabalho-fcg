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
		src/entrePortaisEngine/Event.cpp
		src/entrePortaisEngine/IScene.cpp
		src/entrePortaisEngine/render/Camera.cpp
		src/entrePortaisEngine/tasks/TaskManager.cpp
		src/utils/matrices.cpp
		src/entrePortaisEngine/render/ShadersManager.cpp
		src/entrePortaisEngine/render/IRenderable.cpp
		src/entrePortaisEngine/render/Renderer.cpp
		src/entrePortaisEngine/meshes/IMesh.cpp
		src/entrePortaisEngine/meshes/CubeMesh.cpp
		src/entrePortaisEngine/meshes/CylinderMesh.cpp
		src/entrePortaisEngine/meshes/PlaneMesh.cpp
		src/entrePortaisEngine/Characters/FirstPersonCharacter.cpp

		src/testGame/TriScene.cpp
		src/testGame/TriObject.cpp
		src/testGame/WaitAndFinishTask.cpp
		src/testGame/DependencyTask.cpp
		src/testGame/CubeObject.cpp
		src/testGame/CylinderObject.cpp
		src/testGame/Ground.cpp
		src/testGame/Player.cpp
		)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
)

set(test_sources
)
