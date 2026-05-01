#pragma once

#include "attack_strategy_factories.h"
#include "utility/factory_manager.h"
#include "attack/i_attack_strategy_factory.h"
#include "tower/i_tower_service.h"

namespace TowerDefence
{
    /**
     * @class AttackStrategyFactoryManager
     * @brief Менеджер фабрик стратегий атаки.
     *
     * Специализация FactoryManager для управления фабриками стратегий атаки.
     * Упрощает регистрацию фабрик с dependency injection.
     */
    class AttackStrategyFactoryManager final : public FactoryManager<IAttackStrategy, IAttackStrategyFactory>{
    public:
        /**
         * @brief Добавить фабрику стратегии атаки с сервисами.
         * @tparam AttackT Тип стратегии атаки.
         * @tparam Services Типы сервисов.
         * @param name Имя стратегии атаки.
         * @param fullConfig Полная конфигурация стратегии.
         * @param services Сервисы для передачи в фабрику.
         * @requires AttackStrategyFactory<AttackT, Services...> должен удовлетворять концепту FactoryWithServiceReferences.
         */
        template <typename AttackT, typename... Services>
        void addFactory(const std::string& name, const IConfigReader& fullConfig, Services&& ...services)
        requires FactoryWithServiceReferences<AttackStrategyFactory<AttackT, Services...>, Services...>
        {
            addFactoryWithRefs<AttackStrategyFactory<AttackT, Services...>>(name, *fullConfig.getNested(name), std::forward<Services>(services)...);
        }
    };
}
// asfm()
// ts(asfm)
// asfm.init(ts)
//ts.init()