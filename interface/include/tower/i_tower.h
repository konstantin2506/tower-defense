#pragma once

#include "attack/i_attackable.h"
#include "effects/i_effectable.h"
#include "entity_interafaces/i_updatable.h"
#include "entity_interafaces/i_upgradable.h"

namespace TowerDefence
{
    /**
     * @class ITower
     * @brief Интерфейс башни в игре Tavern Defense.
     *
     * Композитный интерфейс, объединяющий возможности улучшения, применения эффектов,
     * атаки и обновления. Представляет базовый тип для всех башен в игре.
     */
    class ITower : public IUpgradable, public IEffectable<ITower>, public IAttackable, public IIDObject, public IUpdatable<void>{
    public:
        ~ITower() override = default;

        /**
         * @brief Создать копию башни.
         * @return std::unique_ptr<ITower> на клонированную башню.
         */
        [[nodiscard]] virtual std::unique_ptr<ITower> clone() const = 0;

        /**
         * @brief Настроить башню из конфигурации.
         * @param config IConfigReader с параметрами башни.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Сохранить состояние башни в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader& config) = 0;
    };
}
