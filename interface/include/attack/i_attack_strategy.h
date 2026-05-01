#pragma once

#include "../utility/i_id_object.h"
#include "../config/i_config_reader.h"

namespace TowerDefence
{
    /**
     * @class IAttackStrategy
     * @brief Интерфейс стратегии атаки.
     *
     * Определяет поведение атаки для игровых объектов (башен, врагов).
     * Стратегия инкапсулирует логику поиска цели, нанесения урона
     * и управления параметрами атаки.
     */
    class IAttackStrategy {
    public:
        virtual ~IAttackStrategy() = default;

        /**
         * @brief Попытаться атаковать.
         * @param attacker Объект, выполняющий атаку (реализует IIDObject).
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         * @return true если атака была выполнена, false в противном случае.
         */
        virtual bool tryAttack(IIDObject& attacker, float dt) = 0;

        /**
         * @brief Умножить урон атаки.
         * @param multiplier Множитель урона.
         */
        virtual void multiplyDamage(float multiplier) = 0;

        /**
         * @brief Умножить радиус атаки.
         * @param multiplier Множитель радиуса.
         */
        virtual void multiplyRadius(float multiplier) = 0;

        /**
         * @brief Создать копию стратегии.
         * @return std::unique_ptr<IAttackStrategy> клонированная стратегия.
         */
        [[nodiscard]] virtual std::unique_ptr<IAttackStrategy> clone() = 0;

        /**
         * @brief Настроить стратегию из конфигурации.
         * @param cfg IConfigReader с параметрами стратегии.
         */
        virtual void setFromConfig(const IConfigReader& cfg) = 0;

        /**
         * @brief Сохранить стратегию в конфигурацию.
         * @param name Имя стратегии для сохранения.
         * @param config IConfigReader для записи конфигурации.
         */
        virtual void toConfig(const std::string& name, IConfigReader& config) = 0;
    };
}
