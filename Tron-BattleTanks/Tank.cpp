#include "Tank.h"

tron::Tank::Tank(std::unique_ptr<TankState> initialState)
	: m_pState(std::move(initialState))
{
	if (m_pState)
		m_pState->OnEnter(*this);
}

void tron::Tank::HandleInput()
{
	if (!m_pState) return;

	auto newState = m_pState->HandleInput(*this);
	if (newState)
	{
		m_pState = std::move(newState);
	}

}

void tron::Tank::Update()
{
	if (!m_pState) return;

	auto newState = m_pState->Update(*this);
	if (newState)
	{
		m_pState = std::move(newState);
	}
}

void tron::Tank::Wander()
{
}

void tron::Tank::Shoot()
{
}

bool tron::Tank::IsTargetInRange() const
{
	return false;
}
