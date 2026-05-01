#pragma once
#include <memory>

#include "ability.h"
#include "ability/i_ability_factory.h"

namespace TowerDefence
{
    /**
     * @class AbilityFactory
     * @brief Фабрика для создания способностей с dependency injection.
     *
     * Реализация IAbilityFactory, которая инкапсулирует создание
     * конкретных типов способностей с передачей зависимостей в конструктор.
     * @tparam ResultT Тип создаваемой способности.
     * @tparam Services Типы сервисов, передаваемых в конструктор способности.
     * @requires ResultT должен быть конструируемым из IConfigReader и Services.
     */
    template <typename ResultT, typename... Services>
    requires std::constructible_from<ResultT, const IConfigReader&, Services...>
    class AbilityFactory final: public IAbilityFactory {
        std::unique_ptr<ResultT> donor_;
    public:
        /**
         * @brief Конструктор фабрики способностей.
         * @param cfg IConfigReader для инициализации донорской способности.
         * @param services Сервисы для передачи в конструктор способности.
         */
        explicit AbilityFactory(const IConfigReader& cfg, Services&& ...services)
            {
                donor_ = std::make_unique<ResultT>(cfg, std::forward<Services>(services)...);
            }

            /**
             * @brief Создать новую способность.
             * @return std::unique_ptr<IAbility> созданная способность.
             */
            [[nodiscard]] std::unique_ptr<IAbility> create() const override { return donor_->clone(); }

            /**
             * @brief Восстановить способность из конфигурации.
             * @param cfg IConfigReader с данными для восстановления.
             * @return std::unique_ptr<IAbility> восстановленная способность.
             */
            [[nodiscard]] std::unique_ptr<IAbility> restore(const IConfigReader& cfg) const override
            {
                auto restored =  donor_->clone();
                restored->setFromConfig(cfg);
                return restored;
            }
        };
}