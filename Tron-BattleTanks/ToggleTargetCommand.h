#pragma once

#include "Command.h"
#include "TankStateComponent.h"

namespace tron
{
    class ToggleTargetCommand : public dae::Command
    {
        TankStateComponent* m_pStateComponent;
    public:
        ToggleTargetCommand(TankStateComponent* pStateComponent) : m_pStateComponent(pStateComponent) {}
        void Execute(float) override
        {
            m_pStateComponent->m_TargetInRange = !m_pStateComponent->m_TargetInRange;
        }
    };
}