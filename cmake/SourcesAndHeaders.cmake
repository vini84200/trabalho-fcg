set(sources
		src/entrePortaisEngine/Window.cpp
		src/entrePortaisEngine/Shader.cpp
		src/entrePortaisEngine/Logger.cpp
		src/entrePortaisEngine/VertexArrayBuffer.cpp
		src/entrePortaisEngine/IGameNode.cpp
		src/entrePortaisEngine/EasyMesh.cpp
		src/entrePortaisEngine/IObject.cpp
		src/entrePortaisEngine/Transform.cpp
		src/utils/matrices.cpp

		src/testGame/TriScene.cpp
		src/testGame/TriObject.cpp
		)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
)

set(test_sources
)
