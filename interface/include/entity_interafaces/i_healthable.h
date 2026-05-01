#pragma once

namespace TowerDefence
{
    /**
     * @class IHealthable
     * @brief Интерфейс для объектов, имеющих здоровье (HP).
     *
     * Определяет операции для управления здоровьем объектов:
     * получение урона, лечение, проверка жизнеспособности.
     */
    class IHealthable {
    public:
        virtual ~IHealthable() = default;

        /**
         * @brief Нанести урон объекту.
         * @param damage Количество урона.
         */
        virtual void dealDamage(float damage) = 0;

        /**
         * @brief Исцелить объект.
         * @param heal Количество здоровья для восстановления.
         */
        virtual void dealHeal(float heal) = 0;

        /**
         * @brief Умножить максимальное здоровье объекта.
         * @param multiplier Множитель максимального здоровья.
         */
        virtual void multiplyMaxHealth(float multiplier) = 0;

        /**
         * @brief Умножить поглощаемый урон.
         * @param multiplier Множитель поглощаемого урона.
         */
        virtual void multiplyAbsorbedDamage(float multiplier) = 0;

        /**
         * @brief Умножить поглощаемое лечение.
         * @param multiplier Множитель поглощаемого лечения.
         */
        virtual void multiplyAbsorbedHeal(float multiplier) = 0;

        /**
         * @brief Проверить, жив ли объект.
         * @return true если объект жив, false если мертв.
         */
        [[nodiscard]] virtual bool isAlive() const = 0;

        /**
         * @brief Убить объект (установить здоровье в 0).
         */
        virtual void kill() = 0;

        /**
         * @brief Получить текущее здоровье объекта.
         * @return float текущее здоровье.
         */
        [[nodiscard]] virtual float getHealth() const = 0;

        /**
         * @brief Получить максимальное здоровье объекта.
         * @return float максимальное здоровье.
         */
        [[nodiscard]] virtual float getMaxHealth() const = 0;
    };
}