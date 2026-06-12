#include "LifetimeComponent.h"
#include "GameObject.h"

dae::LifetimeComponent::LifetimeComponent(GameObject* owner, float lifetimeSeconds)
    : Component(owner)
    , m_TimeLeft(lifetimeSeconds)
{
}

void dae::LifetimeComponent::Update(float deltaTime)
{
    m_TimeLeft -= deltaTime;
    if (m_TimeLeft <= 0.f)
        GetOwner()->MarkForDeath();
}