#pragma once
#include "InputAction.h"

using KeyboardButton = int;

namespace Engine
{
    class EntityManager;
    struct InputComponent;

    class InputManager
    {
    public:
        bool Init();
        void Update(float dt, EntityManager* entityManager);
        void Shutdown();

        static bool IsActionActive(InputComponent* inputComponent, EInputAction targetAction);

        InputManager() = default;
        bool IsActionInState(EInputAction _eAction, EInputActionState _eState) const;

    private:
        void ProcessInput();
        void InitKeybinds();

        std::unordered_map<EInputAction, KeyboardButton> m_InputActions{ };
        std::unordered_map<EInputAction, EInputActionState> m_InputActionStates{ };
        

        InputManager(const InputManager& other) = delete;
        InputManager& operator=(InputManager& other) = delete;
    };
}
