#pragma once
#include "ECS/Entity.h"

namespace Engine
{
    struct Texture;

    class EntityManager
    {
    public:
        bool Init();
        void Update(float dt);
        void AddEntity(Entity* e);
        void AddEntity(std::unique_ptr<Entity>&& e);

        template <typename TComponent>
        auto GetAllEntitiesWithComponent()
        {
            std::vector<Entity*> returnVec{};

            for (const auto& entity : m_Entities)
            {
                if (entity->HasComponent<TComponent>())
                {
                    returnVec.push_back(entity.get());
                }
            }

            return returnVec;
        }

		template <typename TComponent, typename OutIt>
		OutIt GetAllEntitiesWithComponent(OutIt out_)
		{
			for (const auto& entity : m_Entities)
			{
				if (entity->HasComponent<TComponent>())
				{
					*out_++ = entity.get();
				}
			}
			return out_;
		}
    	
    	
        template <typename... TComponent>
        auto GetAllEntitiesWithComponents()
        {
            std::vector<Entity*> returnVec{};

            for (const auto& entity : m_Entities)
            {
                if (entity->HasComponents<TComponent...>())
                {
                    returnVec.push_back(entity.get());
                }
            }

            return returnVec;
        }

		template <typename... TComponent, typename OutIt>
		OutIt GetAllEntitiesWithComponents(OutIt out)
		{

			for (const auto& entity : m_Entities)
			{
				if (entity->HasComponents<TComponent...>())
				{
					*out++ = (entity.get());
				}
			}

			return out;
		}

        template <typename TComponent>
        auto GetAllComponentInstances()
        {
            std::vector<TComponent*> returnVec{};

            for (const auto& entity : m_Entities)
            {
                if (TComponent* component = entity->GetComponent<TComponent>())
                {
                    returnVec.push_back(component);
                }
            }

            return returnVec;
        }

        template<typename TComponent>
        Entity* GetEntityWithComponent()
        {
            for (const auto& entity : m_Entities)
            {
                if (TComponent* component = entity->GetComponent<TComponent>())
                {
                    return entity.get();
                }
            }
            return nullptr;
        }

        EntityManager() = default;
    private:
        using EntityList = std::vector<std::unique_ptr<Entity>>;
        // TODO(Marko): Segregate entities by their components
        EntityList m_Entities;

        EntityManager(const EntityManager& other) = delete;
        EntityManager& operator=(EntityManager& other) = delete;
    };

}
