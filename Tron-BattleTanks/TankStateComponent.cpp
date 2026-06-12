#include "TankStateComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GunComponent.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"
#include <cmath>

bool tron::TankStateComponent::HasClearLineOfSight(const glm::vec3& from, const glm::vec2& direction, float distance) const
{
	if (!m_pCollision) return false;

	const float tileSize = m_pCollision->GetTileSize();
	const int steps = static_cast<int>(distance / tileSize);

	// Step along the line one tile at a time, checking for walls
	for (int i = 1; i <= steps; ++i)
	{
		const float checkX = from.x + direction.x * tileSize * static_cast<float>(i);
		const float checkY = from.y + direction.y * tileSize * static_cast<float>(i);

		if (m_pCollision->IsWall(checkX, checkY))
			return false;
	}

	return true;
}

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

glm::vec2 tron::TankStateComponent::GetDirectionToNearestTargetInLineOfSight() const
{
	if (!m_pTransformComponent || !m_pCollision) return { 0.f, 0.f };

	const glm::vec3 selfPos = m_pTransformComponent->GetLocalPosition();
	const float tileSize = m_pCollision->GetTileSize();
	const float alignTolerance = tileSize * 0.5f; // tanks are 2 tiles wide/tall

	const dae::GameObject* nearest = nullptr;
	float nearestDist = m_TargetRange;
	glm::vec2 nearestDirection{ 0.f, 0.f };

	for (const auto* target : m_pTargets)
	{
		if (!target) continue;

		auto* targetTransform = target->GetComponent<dae::TransformComponent>();
		if (!targetTransform) continue;

		const glm::vec3 targetPos = targetTransform->GetLocalPosition();
		const float dx = targetPos.x - selfPos.x;
		const float dy = targetPos.y - selfPos.y;

		glm::vec2 direction{ 0.f, 0.f };
		float dist = 0.f;

		// Check horizontal alignment (same row)
		if (std::abs(dy) <= alignTolerance)
		{
			dist = std::abs(dx);
			if (dist > m_TargetRange) continue;
			direction = { dx > 0.f ? 1.f : -1.f, 0.f };
		}
		// Check vertical alignment (same column)
		else if (std::abs(dx) <= alignTolerance)
		{
			dist = std::abs(dy);
			if (dist > m_TargetRange) continue;
			direction = { 0.f, dy > 0.f ? 1.f : -1.f };
		}
		else
		{
			continue; // not aligned on the grid, can't shoot in a straight line
		}

		if (dist > nearestDist) continue;
		if (dist < 0.0001f) continue; // skip if effectively on top of target

		// Check for walls along the straight path
		if (HasClearLineOfSight(selfPos, direction, dist))
		{
			nearestDist = dist;
			nearest = target;
			nearestDirection = direction;
		}
	}

	if (!nearest) return { 0.f, 0.f };
	return nearestDirection;
}
