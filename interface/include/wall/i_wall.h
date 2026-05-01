#pragma once



#include <memory>

#include "config/i_configurable.h"
#include "effects/i_effectable.h"
#include "entity_interafaces/i_healthable.h"
#include "entity_interafaces/i_repairable.h"
#include "entity_interafaces/i_updatable.h"
#include "entity_interafaces/i_upgradable.h"
#include "utility/i_id_object.h"

namespace TowerDefence
{
    /**
     * @class IWall
     * @brief Интерфейс стены в игре Tavern Defense.
     *
     * Композитный интерфейс, объединяющий возможности ремонта, улучшения,
     * применения эффектов, управления здоровьем и конфигурации.
     * Представляет базовый тип для всех стен в игре.
     */
    class IWall : public IRepairable, public IUpgradable,
    public IEffectable<IWall>, public IIDObject, public IHealthable, public IConfigurable, public IUpdatable<void>{
    public:
        virtual ~IWall() = default;

        /**
         * @brief Создать копию стены.
         * @return std::unique_ptr<IWall> на клонированную стену.
         */
        virtual std::unique_ptr<IWall> clone()  = 0;

        /**
         * @brief Сохранить состояние стены в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader& config) = 0;
    };
}
