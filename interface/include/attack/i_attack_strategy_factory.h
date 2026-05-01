#pragma once
#include "i_attack_strategy.h"
#include "../config/i_config_reader.h"
#include <memory>
namespace TowerDefence
{
    /**
     * @class IAttackStrategyFactory
     * @brief Фабрика для создания и восстановления стратегий атаки.
     *
     * Абстрактная фабрика, предоставляющая интерфейс для создания
     * новых экземпляров стратегий атаки и их восстановления из конфигурации.
     */
    class IAttackStrategyFactory{
    public:
        virtual ~IAttackStrategyFactory() = default;

        /**
         * @brief Создать новую стратегию атаки.
         * @return std::unique_ptr<IAttackStrategy> созданная стратегия.
         */
        [[nodiscard]] virtual std::unique_ptr<IAttackStrategy> create() const = 0;

        /**
         * @brief Восстановить стратегию атаки из конфигурации.
         * @param cfg IConfigReader с данными для восстановления.
         * @return std::unique_ptr<IAttackStrategy> восстановленная стратегия.
         */
        [[nodiscard]] virtual std::unique_ptr<IAttackStrategy> restore(const IConfigReader& cfg) const = 0;
    };
}
