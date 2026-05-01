#pragma once
#include <memory>

#include "config/i_config_reader.h"

namespace TowerDefence
{
    class IEnemy;

    /**
     * @class IAbility
     * @brief Интерфейс способности, которой может обладать враг.
     *
     * Определяет базовые операции для всех способностей в игре:
     * использование, конфигурация и клонирование.
     */
    class IAbility {
    public:
        virtual ~IAbility() = default;

        /**
         * @brief Попытаться использовать способность.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         * @param owner IEnemy, который владеет этой способностью.
         */
        virtual void tryUseAbility([[maybe_unused]] float dt, IEnemy& owner) = 0;

        /**
         * @brief Настроить способность из конфигурации.
         * @param config IConfigReader с параметрами способности.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Создать копию способности.
         * @return std::unique_ptr<IAbility> на клонированную способность.
         */
        [[nodiscard]] virtual std::unique_ptr<IAbility> clone() const = 0;
    };
}
