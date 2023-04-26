#ifndef ENTREPORTAIS_LABIRINTOMAP_HPP
#define ENTREPORTAIS_LABIRINTOMAP_HPP

#include "entrePortaisEngine/IObject.hpp"
#include "labirinto/InvisibleBox.hpp"

namespace labirinto {

    class LabirintoMap : public entre_portais::IObject {
    public:
        LabirintoMap();

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        glm::vec2 getPlayerPosition();

        int posToIndex(glm::vec2 position);

        std::vector<int> indexSquare(int index);

        void indexesDraw(std::vector<int> indexes);

        glm::vec2 indexToPos(int index);

        void buildCollision();

        void buildInitialCollision();

    private:
        int index_;
        std::array<std::shared_ptr<InvisibleBox>, 9> colliders_;
        const int array_[1681] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                                  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1,
                                  1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1,
                                  0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
                                  1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1,
                                  0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                                  1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1,
                                  0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1,
                                  1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
                                  0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0,
                                  1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1,
                                  1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
                                  1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0,
                                  1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
                                  1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
                                  1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
                                  1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
                                  1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1,
                                  1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0,
                                  0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
                                  1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0,
                                  1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
                                  0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
                                  1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1,
                                  1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                                  0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1,
                                  1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0,
                                  1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1,
                                  1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1,
                                  1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                                  1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0,
                                  1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0,
                                  0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1,
                                  1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1,
                                  1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                                  1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
                                  1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1,
                                  1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                                  1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1,
                                  1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
                                  1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0,
                                  1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1,
                                  1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0,
                                  1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0,
                                  0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1,
                                  1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1,
                                  1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1,
                                  1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1,
                                  1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1,
                                  1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1,
                                  1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1,
                                  1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
                                  1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1,
                                  1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0,
                                  1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1,
                                  1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1,
                                  1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
                                  1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1,
                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    };

} // labirinto

#endif //ENTREPORTAIS_LABIRINTOMAP_HPP
