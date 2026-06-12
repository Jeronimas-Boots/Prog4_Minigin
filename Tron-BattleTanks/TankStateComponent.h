#pragma once 
#include "TankState.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include <memory>
#include <vector>

namespace dae { class GameObject; }

namespace tron
{
	class GridCollisionComponent;
	class GunComponent;
	class GridMovementComponent;

	class TankStateComponent : public dae::Component
	{
	private:
		std::unique_ptr<TankState> m_pState;
		dae::RenderComponent* m_pRenderComponent = nullptr;
		dae::TransformComponent* m_pTransformComponent = nullptr;
		GunComponent* m_pGunComponent = nullptr;
		GridCollisionComponent* m_pCollision = nullptr;
		GridMovementComponent* m_pMovement = nullptr;

		std::vector<dae::GameObject*> m_pTargets; // e.g. player tanks
		float m_TargetRange = 200.f;

	public:
		TankStateComponent(dae::GameObject* owner, std::unique_ptr<TankState> initialState,
			GunComponent* gunComponent, GridCollisionComponent* collision, GridMovementComponent* movement);

		void HandleInput();
		void Update(float deltaTime) override;

		void SetTargets(std::vector<dae::GameObject*> targets) { m_pTargets = std::move(targets); }
		void SetTargetRange(float range) { m_TargetRange = range; }

		bool IsTargetInRange() const;
		glm::vec2 GetDirectionToNearestTarget() const; // returns {0,0} if no target

		dae::TransformComponent* GetTransform() const { return m_pTransformComponent; }
		GunComponent* GetGun() const { return m_pGunComponent; }
		GridCollisionComponent* GetCollision() const { return m_pCollision; }
		GridMovementComponent* GetMovement() const { return m_pMovement; }
	};
}