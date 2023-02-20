#ifndef ENTREPORTAIS_PLAYER_HPP
#define ENTREPORTAIS_PLAYER_HPP

#include "entrePortaisEngine/Characters/FirstPersonCharacter.hpp"

namespace entre_portais {

    class Player : public FirstPersonCharacter {
    public:
        Player(char *name);

        void loadBodyMesh() override;

    private:

    };

} // entre_portais

#endif //ENTREPORTAIS_PLAYER_HPP
