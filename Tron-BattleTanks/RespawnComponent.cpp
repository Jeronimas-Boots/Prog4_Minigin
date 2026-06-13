#include "RespawnComponent.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "EventID.h"
#include "GridMovementComponent.h"

tron::RespawnComponent::RespawnComponent(dae::GameObject* owner, dae::HealthComponent* health, glm::vec2 spawnPos)
    : Component(owner), m_pHealth(health), m_SpawnPos(spawnPos)
{
    health->AddObserver(this);
}

tron::RespawnComponent::~RespawnComponent()
{
}

void tron::RespawnComponent::Notify(dae::GameObject*, unsigned int eventId)
{
    if (eventId != make_sdbm_hash("HealthChanged")) return;

    // Only respawn if lives remain — last hit lets Die() handle cleanup
    if (m_pHealth->GetLives() > 0)
    {
        if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
            transform->SetLocalPosition(m_SpawnPos.x, m_SpawnPos.y, 0.f);

        if (auto* movement = GetOwner()->GetComponent<tron::GridMovementComponent>())
            movement->Reset();
    }
}