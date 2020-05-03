#pragma once

namespace Engine
{
	class Entity;
	class EntityManager;

    class PhysicsSystem
    {
    public:
        bool Init();
        void Update(float dt, EntityManager* entityManager);

    private:
		std::vector<Entity*> m_EntitiesBuffer;
    };
}