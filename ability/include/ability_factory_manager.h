#pragma once

#include "ability_factories.h"
#include "ability/i_ability.h"
#include "ability/i_ability_factory.h"
#include "tower/i_tower_service.h"
#include "utility/factory_manager.h"


namespace TowerDefence
{
    /**
     * @class AbilityFactoryManager
     * @brief Менеджер фабрик способностей.
     *
     * Специализация FactoryManager для управления фабриками способностей.
     * Упрощает регистрацию фабрик с dependency injection.
     */
    class AbilityFactoryManager final: public FactoryManager<IAbility, IAbilityFactory> {
    public:
        AbilityFactoryManager() = default;

        /**
         * @brief Добавить фабрику способности с сервисами.
         * @tparam AbilityT Тип способности.
         * @tparam Services Типы сервисов.
         * @param name Имя способности.
         * @param fullConfig Полная конфигурация способности.
         * @param services Сервисы для передачи в фабрику.
         * @requires AbilityT должен удовлетворять концепту FactoryWithServiceReferences.
         */
        template <typename AbilityT, typename... Services>
        void addFactory(const std::string& name, const IConfigReader& fullConfig, Services&& ...services)
        requires FactoryWithServiceReferences<AbilityT, Services...>
        {
            addFactoryWithRefs<AbilityFactory<AbilityT, Services...>>(name, *fullConfig.getNested(name), std::forward<Services>(services)...);
        }
    };
}
