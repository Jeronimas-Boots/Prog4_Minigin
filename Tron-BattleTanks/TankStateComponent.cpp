#include "TankStateComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GunComponent.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"

tron::TankStateComponent::TankStateComponent(dae::GameObject* owner, std::unique_ptr<TankState> initialState,
	GunComponent* gunComponent, GridCollisionComponent* collision, GridMovementComponent* movement)
	:Component(owner)
	, m_pState(std::move(initialState))
	, m_pGunComponent(gunComponent)
	, m_pCollision(collision)
	, m_pMovement(movement)
{
	m_pRenderComponent = owner->GetComponent<dae::RenderComponent>();
	m_pTransformComponent = owner->GetComponent<dae::TransformComponent>();

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

void tron::TankStateComponent::Update(float deltaTime)
{
	if (!m_pState) return;

	auto newState = m_pState->Update(*this, deltaTime);
	if (newState)
	{
		m_pState->OnExit(*this);
		m_pState = std::move(newState);
		m_pState->OnEnter(*this);
	}
}

bool tron::TankStateComponent::IsTargetInRange() const
{
	if (!m_pTransformComponent) return false;

	const glm::vec3 selfPos = m_pTransformComponent->GetLocalPosition();

	for (const auto* target : m_pTargets)
	{
		if (!target) continue;

		auto* targetTransform = target->GetComponent<dae::TransformComponent>();
		if (!targetTransform) continue;

		const glm::vec3 targetPos = targetTransform->GetLocalPosition();
		const float dx = targetPos.x - selfPos.x;
		const float dy = targetPos.y - selfPos.y;
		const float distSq = dx * dx + dy * dy;

		if (distSq <= m_TargetRange * m_TargetRange)
			return true;
	}

	return false;
}

glm::vec2 tron::TankStateComponent::GetDirectionToNearestTarget() const
{
	if (!m_pTransformComponent) return { 0.f, 0.f };

	const glm::vec3 selfPos = m_pTransformComponent->GetLocalPosition();

	const dae::GameObject* nearest = nullptr;
	float nearestDistSq = m_TargetRange * m_TargetRange;

	for (const auto* target : m_pTargets)
	{
		if (!target) continue;

		auto* targetTransform = target->GetComponent<dae::TransformComponent>();
		if (!targetTransform) continue;

		const glm::vec3 targetPos = targetTransform->GetLocalPosition();
		const float dx = targetPos.x - selfPos.x;
		const float dy = targetPos.y - selfPos.y;
		const float distSq = dx * dx + dy * dy;

		if (distSq <= nearestDistSq)
		{
			nearestDistSq = distSq;
			nearest = target;
		}
	}

	if (!nearest) return { 0.f, 0.f };

	auto* nearestTransform = nearest->GetComponent<dae::TransformComponent>();
	const glm::vec3 targetPos = nearestTransform->GetLocalPosition();
	const glm::vec2 diff{ targetPos.x - selfPos.x, targetPos.y - selfPos.y };
	const float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
	if (len <= 0.0001f) return { 0.f, 0.f };

	return diff / len;
}