#ifndef ENTREPORTAIS_ISCENE_HPP
#define ENTREPORTAIS_ISCENE_HPP

namespace entre_portais {
    class IScene {
    public:
        virtual ~IScene() = default;

        IScene() = default;

        // Remove construtores e operadores de cópia e movimentação
        IScene(const IScene &other) = delete;

        IScene &operator=(const IScene &other) = delete;

        IScene(IScene &&other) = delete;

        IScene &operator=(IScene &&other) = delete;

        virtual void initialize() = 0;

        virtual void update() = 0;

        virtual void render() = 0;

        virtual void onResize(int width, int height) = 0;

        virtual void onKey(int key, int scancode, int action, int mods) = 0;

        virtual void onMouseButton(int button, int action, int mods) = 0;

        // TODO: Adicionar mais eventos
        virtual void onExit() = 0;
    };
}

#endif //ENTREPORTAIS_ISCENE_HPP
