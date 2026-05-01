#pragma once

#include "attack_strategies.h"
#include "attack/i_attack_strategy_factory.h"
#include <concepts>

namespace TowerDefence
{
    /**
     * @class AttackStrategyFactory
     * @brief Фабрика для создания стратегий атаки с dependency injection.
     *
     * Реализация IAttackStrategyFactory, которая инкапсулирует создание
     * конкретных типов стратегий атаки с передачей зависимостей в конструктор.
     * @tparam ResultT Тип создаваемой стратегии атаки.
     * @tparam Services Типы сервисов, передаваемых в конструктор стратегии.
     * @requires ResultT должен быть конструируемым из IConfigReader и Services.
     */
    template <typename ResultT, typename... Services>
    requires std::constructible_from<ResultT, const IConfigReader&, Services...>
    class AttackStrategyFactory final: public IAttackStrategyFactory {
        std::unique_ptr<ResultT> donor_;
    public:
        /**
         * @brief Конструктор фабрики стратегий атаки.
         * @param cfg IConfigReader для инициализации донорской стратегии.
         * @param services Сервисы для передачи в конструктор стратегии.
         */
        explicit AttackStrategyFactory(const IConfigReader& cfg, Services&& ...services)
        {
            donor_ = std::make_unique<ResultT>(cfg, std::forward<Services>(services)...);
        }

        /**
         * @brief Создать новую стратегию атаки.
         * @return std::unique_ptr<IAttackStrategy> созданная стратегия.
         */
        [[nodiscard]] std::unique_ptr<IAttackStrategy> create() const override { return donor_->clone(); }

        /**
         * @brief Восстановить стратегию атаки из конфигурации.
         * @param cfg IConfigReader с данными для восстановления.
         * @return std::unique_ptr<IAttackStrategy> восстановленная стратегия.
         */
        [[nodiscard]] std::unique_ptr<IAttackStrategy> restore(const IConfigReader& cfg) const override
        {
            auto restored =  donor_->clone();
            restored->setFromConfig(cfg);
            return restored;
        }
    };
}