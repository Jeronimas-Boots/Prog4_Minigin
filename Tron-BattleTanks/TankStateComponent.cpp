#include "TankStateComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

tron::TankStateComponent::TankStateComponent(dae::GameObject* owner, std::unique_ptr<TankState> initialState)
	:Component(owner)
	,m_pState(std::move(initialState))
{
	if (m_pState)
		m_pState->OnEnter(*this);
}

void tron::TankStateComponent::HandleInput()
{
	if (!m_pState) return;

	auto newState = m_pState->HandleInput(*this);
	if (newState)
	{
		m_pState->OnExit(*this);
		m_pState = std::move(newState);
		m_pState->OnEnter(*this);
	}

}

void tron::TankStateComponent::Update(float)
{
	if (!m_pState) return;

	auto newState = m_pState->Update(*this);
	if (newState)
	{
		m_pState->OnExit(*this);
		m_pState = std::move(newState);
		m_pState->OnEnter(*this);
	}
}

void tron::TankStateComponent::Wander()
{
}

void tron::TankStateComponent::Shoot()
{
}

bool tron::TankStateComponent::IsTargetInRange() const
{
	return m_TargetInRange;
}

void tron::TankStateComponent::ChangeToWanderVisuals()
{
	if (!m_pGreenTankTexture) m_pGreenTankTexture = GetOwner()->GetComponent<dae::RenderComponent>();

	if (m_pGreenTankTexture)
	{
		m_pGreenTankTexture->SetTexture(dae::ResourceManager::GetInstance().LoadTexture("GreenTank.png"));
	}
}

void tron::TankStateComponent::ChangeToShootVisuals()
{
	if (!m_pGreenTankTexture) m_pGreenTankTexture = GetOwner()->GetComponent<dae::RenderComponent>();

	if (m_pGreenTankTexture)
	{
		m_pGreenTankTexture->SetTexture(dae::ResourceManager::GetInstance().LoadTexture("PinkTank.png"));
	}
}
