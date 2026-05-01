#pragma once

#include "entity_interafaces/i_healthable.h"
#include "utility/i_id_object.h"
#include "entity_interafaces/i_repairable.h"
#include "entity_interafaces/i_updatable.h"
#include "entity_interafaces/i_upgradable.h"

namespace TowerDefence
{
    /**
     * @class ICastle
     * @brief Интерфейс замка в игре Tavern Defense.
     *
     * Композитный интерфейс, объединяющий возможности улучшения, ремонта,
     * управления здоровьем и обновления. Представляет главный замок игрока.
     */
    class ICastle :  public IUpgradable, public IRepairable, public IIDObject, public IHealthable, public IUpdatable<void> {
    public:
        virtual ~ICastle() = default;

        /**
         * @brief Настроить замок из конфигурации.
         * @param config IConfigReader с параметрами замка.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Сохранить состояние замка в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader& config) = 0;
    };
}
