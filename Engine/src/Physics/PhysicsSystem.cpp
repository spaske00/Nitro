#include "precomp.h"

#include "PhysicsSystem.h"

#include "ECS/EntityManager.h"
#include "ECS/Component.h"

namespace Engine
{
        bool CheckForCollision(Entity * entity1, Entity * entity2);
        bool CheckForCollisionAABoxVsCircle(Entity * entity1, Entity * entity2);

    bool PhysicsSystem::Init()
    {
        return true;
    }

    void PhysicsSystem::Update(float dt, EntityManager* entityManager)
    {
        // Move
        //auto m_EntitiesBuffer = entityManager->GetAllEntitiesWithComponents<TransformComponent, MoverComponent>();
		m_EntitiesBuffer.clear();
		entityManager->GetAllEntitiesWithComponents<TransformComponent, MoverComponent>(std::back_inserter(m_EntitiesBuffer));
    	
        for (auto& entity : m_EntitiesBuffer)
        {
            auto transform = entity->GetComponent<TransformComponent>();
            auto mover = entity->GetComponent<MoverComponent>();

            transform->m_Position += mover->m_TranslationSpeed * dt;
            transform->m_Rotation += mover->m_RotationSpeed * dt;
        }

        // Collide
        m_EntitiesBuffer.clear();

#if 1
        entityManager->GetAllEntitiesWithComponents<CollidedWithComponent>(std::back_inserter(m_EntitiesBuffer));
        auto passiveCollisionEntities = entityManager->GetAllEntitiesWithComponent<CollisionComponent>();
        for (auto& entity : m_EntitiesBuffer) { entity->GetComponent<CollidedWithComponent>()->m_CollidedWith.clear(); }

        for (size_t i = 0; i < m_EntitiesBuffer.size(); ++i)
        {
            for (size_t j = i + 1; j < m_EntitiesBuffer.size(); ++j)
            {
                Entity* entity1 = m_EntitiesBuffer[i];
                Entity* entity2 = m_EntitiesBuffer[j];
                bool collided = CheckForCollision(entity1, entity2);
                if (collided)
                {
                    entity1->GetComponent<CollidedWithComponent>()->m_CollidedWith.insert(entity2);
                    entity2->GetComponent<CollidedWithComponent>()->m_CollidedWith.insert(entity1);
                }
            }
        }

        for (size_t i = 0; i < m_EntitiesBuffer.size(); ++i)
        {
            for (size_t j = 0; j < passiveCollisionEntities.size(); ++j)
            {
                Entity* entity1 = m_EntitiesBuffer[i];
                Entity* entity2 = passiveCollisionEntities[j];
                bool collided = CheckForCollision(entity1, entity2);
                if (collided)
                {
                    entity1->GetComponent<CollidedWithComponent>()->m_CollidedWith.insert(entity2);
                }
            }
        }

        
#else
        
		entityManager->GetAllEntitiesWithComponent<CollisionComponent>(std::back_inserter(m_EntitiesBuffer));
        for (auto& entity : m_EntitiesBuffer) 
        { 
            if (auto c = entity->GetComponent<CollidedWithComponent>())
            {
                c->m_CollidedWith.clear();
            }
        }

        for (auto& entity1 : m_EntitiesBuffer)
        {
            for (auto& entity2 : m_EntitiesBuffer)
            {
                bool collided = CheckForCollision(entity1, entity2);

                if (collided)
                {
                    if (auto c = entity1->GetComponent<CollidedWithComponent>())
                    {
                        c->m_CollidedWith.insert(entity2);
                    }
                }
            }
        }
#endif
    }

    bool CheckForCollision(Entity* entity1, Entity* entity2)
    {
        if (entity2 == entity1)
        {
            return false;
        }

        auto transform1 = entity1->GetComponent<TransformComponent>();
        auto collider1 = entity1->GetComponent<CollisionComponent>();

        auto transform2 = entity2->GetComponent<TransformComponent>();
        auto collider2 = entity2->GetComponent<CollisionComponent>();

        if (collider1->m_Shape == ECollisionShape::Circle && collider2->m_Shape == ECollisionShape::Circle)
        {
            auto center1 = transform1->m_Position + transform1->m_Size / 2.f;
            auto center2 = transform2->m_Position + transform2->m_Size / 2.f;

            auto sizeSumSqrd = collider1->m_Size.x * collider1->m_Size.x + collider2->m_Size.x * collider2->m_Size.x;
            auto distanceSqrd = distance2(center1, center2);
            
            return sizeSumSqrd >= distanceSqrd;
        }
        else if (collider1->m_Shape == ECollisionShape::AABox && collider2->m_Shape == ECollisionShape::AABox)
        {
            bool collisionX = transform1->m_Position.x + collider1->m_Size.x / 2.f >= transform2->m_Position.x - collider2->m_Size.x / 2.f &&
                                transform2->m_Position.x + collider2->m_Size.x / 2.f >= transform1->m_Position.x - collider1->m_Size.x / 2.f;

            bool collisionY = transform1->m_Position.y + collider1->m_Size.y / 2.f >= transform2->m_Position.y - collider2->m_Size.y / 2.f &&
                                transform2->m_Position.y + collider2->m_Size.y / 2.f >= transform1->m_Position.y - collider1->m_Size.y / 2.f;

            return collisionX && collisionY;
        }
        else
        {
            if (collider1->m_Shape == ECollisionShape::Circle && collider2->m_Shape == ECollisionShape::AABox)
            {
                return CheckForCollisionAABoxVsCircle(entity2, entity1);
            }
            else if (collider1->m_Shape == ECollisionShape::AABox && collider2->m_Shape == ECollisionShape::Circle)
            {
                return CheckForCollisionAABoxVsCircle(entity1, entity2);
            }

        }

        ASSERT(false, "Unknown collision shape combination!");
        return false;
    }

    bool CheckForCollisionAABoxVsCircle(Entity* aabox, Entity* circle)
    {
        auto transform1 = aabox->GetComponent<TransformComponent>();
        auto collider1 = aabox->GetComponent<CollisionComponent>();

        auto transform2 = circle->GetComponent<TransformComponent>();
        auto collider2 = circle->GetComponent<CollisionComponent>();

        // center and half-size of aabox
        vec2 aabbHalfSize{ collider1->m_Size / 2.f };
        vec2 aabbCenter{ transform1->m_Position + aabbHalfSize };

        // center of the circle
        vec2 circleCenter{ transform2->m_Position + collider2->m_Size.x };

        // distance from centers
        vec2 centerDistance = circleCenter - aabbCenter;
        vec2 clampedDistance = glm::clamp(centerDistance, -aabbHalfSize, aabbHalfSize);

        // Add clamped value to AABB_center and we get the value of box closest to circle
        vec2 closest = aabbCenter + clampedDistance;

        // Retrieve vector between center circle and closest point AABB and check if length <= radius
        centerDistance = closest - circleCenter;

        return length(centerDistance) < transform2->m_Size.x;
    }

}