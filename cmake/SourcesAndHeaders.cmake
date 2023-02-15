set(sources
		src/entrePortaisEngine/Window.cpp
		src/entrePortaisEngine/Shader.cpp
		src/entrePortaisEngine/Logger.cpp
		src/entrePortaisEngine/VertexArrayBuffer.cpp
		src/entrePortaisEngine/IGameNode.cpp
		src/entrePortaisEngine/EasyMesh.cpp
		src/entrePortaisEngine/IObject.cpp
		src/entrePortaisEngine/Transform.cpp
		src/entrePortaisEngine/IPlugin.cpp
		src/entrePortaisEngine/ImGuiPlugin.cpp
		src/entrePortaisEngine/Event.cpp
		src/entrePortaisEngine/IScene.cpp
		src/entrePortaisEngine/Camera.cpp
		src/entrePortaisEngine/tasks/TaskManager.cpp
		src/utils/matrices.cpp
		src/entrePortaisEngine/ShadersManager.cpp
		src/entrePortaisEngine/render/IRenderable.cpp
		src/entrePortaisEngine/render/Renderer.cpp
		src/entrePortaisEngine/IMesh.cpp


		src/testGame/TriScene.cpp
		src/testGame/TriObject.cpp
		src/testGame/WaitAndFinishTask.cpp
		src/testGame/DependencyTask.cpp
		)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
)

set(test_sources
)
