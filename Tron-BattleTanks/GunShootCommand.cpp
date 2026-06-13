#include "GunShootCommand.h"
#include "GunComponent.h"

tron::GunShootCommand::GunShootCommand(GunComponent* gun, const glm::vec2& direction)
    : m_pGun(gun)
    , m_Direction(direction)
{
}

void tron::GunShootCommand::Execute(float)
{
    if (m_pGun)
        m_pGun->Shoot(m_Direction);
}