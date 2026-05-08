#pragma once 
#include "TankState.h"
#include "Component.h"
#include "RenderComponent.h"
#include <memory>

namespace tron
{
	class TankStateComponent : public dae::Component
	{
	private:
		std::unique_ptr<TankState> m_pState;
		dae::RenderComponent* m_pGreenTankTexture = nullptr;
		dae::RenderComponent* m_pPinkTankTexture = nullptr;

	public:
		bool m_TargetInRange = false; //this is here to test.


		TankStateComponent(dae::GameObject* owner, std::unique_ptr<TankState> initialState);
			
		void HandleInput();
		void Update(float deltaTime) override;

		void Wander();
		void Shoot();
		bool IsTargetInRange() const;

		void ChangeToWanderVisuals();

		void ChangeToShootVisuals();
		
	};
}