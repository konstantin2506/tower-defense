#pragma once

#include <atomic>
#include <iostream>

#include "regen_components.h"

#include <memory>

#include "config/i_config_reader.h"

namespace TowerDefence
{
    /**
     * @class HealthComponent
     * @brief Компонент для управления здоровьем объекта.
     *
     * Инкапсулирует состояние здоровья, включая текущее и максимальное здоровье,
     * коэффициенты поглощения урона/лечения и компонент регенерации.
     * Поддерживает как однопоточный (PARALLEL не определен), так и
     * многопоточный (PARALLEL определен) режимы работы.
     */
    class HealthComponent {
#ifndef PARALLEL
        float health_;
        float maxHealth_;

        float maxHealthCoefficient_ = 1.0f;
        float absorbedDamageCoefficient_ = 1.0f;
        float absorbedHealCoefficient_ = 1.0f;
#else
        std::atomic<float> health_;
        std::atomic<float> maxHealth_;

        std::atomic<float> maxHealthCoefficient_ = 1.0f;
        std::atomic<float> absorbedDamageCoefficient_ = 1.0f;
        std::atomic<float> absorbedHealCoefficient_ = 1.0f;
#endif
        std::unique_ptr<IRegenable> regen_;

    public:
        /**
         * @brief Конструктор компонента здоровья.
         * @param health Текущее здоровье.
         * @param maxHealth Максимальное здоровье.
         * @param regen std::unique_ptr<IRegenable> компонент регенерации.
         */
        HealthComponent(float health, float maxHealth, std::unique_ptr<IRegenable> regen) : health_(health), maxHealth_(maxHealth), regen_(std::move(regen)) {}

        /**
         * @brief Конструктор копирования.
         * @param other Другой компонент здоровья для копирования.
         */
        HealthComponent(const HealthComponent& other) : HealthComponent(other.health_, other.maxHealth_, other.regen_->clone()) {}

        /**
         * @brief Оператор присваивания удален.
         * @details Используйте конструктор копирования вместо оператора присваивания.
         */
        HealthComponent& operator=(const HealthComponent& other) = delete;

        /**
         * @brief Нанести урон объекту.
         * @param damage Количество урона.
         * @details Учитывает коэффициент поглощения урона.
         */
        void dealDamage(float damage);

        /**
         * @brief Исцелить объект.
         * @param heal Количество здоровья для восстановления.
         * @details Учитывает коэффициент поглощения лечения.
         */
        void dealHeal(float heal);

        /**
         * @brief Получить текущее здоровье.
         * @return float текущее здоровье.
         */
        [[nodiscard]] float getHealth() const {return health_;}

        /**
         * @brief Получить максимальное здоровье (с учетом коэффициента).
         * @return float максимальное здоровье.
         */
        [[nodiscard]] float getMaxHealth() const {return maxHealth_ * maxHealthCoefficient_;}

        /**
         * @brief Получить копию компонента регенерации.
         * @return std::unique_ptr<IRegenable> копия компонента регенерации.
         */
        [[nodiscard]] std::unique_ptr<IRegenable> getRegen() const {return regen_->clone();}

        /**
         * @brief Установить новый компонент регенерации.
         * @param regen std::unique_ptr<IRegenable> новый компонент регенерации.
         */
        void setRegen(std::unique_ptr<IRegenable> regen) {regen_ = std::move(regen);}

        /**
         * @brief Установить текущее здоровье.
         * @param health Новое значение здоровья.
         * @details Здоровье ограничивается сверху максимальным здоровьем.
         */
        void setHealth(float health)
        {
#ifndef PARALLEL
            health_ = std::max(health, maxHealth_);
#else
            float currentMaxHealth = maxHealth_.load(std::memory_order_relaxed);
            float desiredHealth = std::max(health, currentMaxHealth);

            float expected = health_.load(std::memory_order_relaxed);
            while (!health_.compare_exchange_weak(expected, desiredHealth, std::memory_order_relaxed, std::memory_order_relaxed)) {
                currentMaxHealth = maxHealth_.load(std::memory_order_relaxed);
                desiredHealth = std::max(health, currentMaxHealth);
            }
#endif
        }

        /**
         * @brief Установить максимальное здоровье.
         * @param maxHealth Новое максимальное здоровье.
         */
        void setMaxHealth(float maxHealth) {maxHealth_ = maxHealth;}

        /**
         * @brief Умножить максимальное здоровье на коэффициент.
         * @param multiplier Множитель максимального здоровья.
         */
        void multiplyMaxHealth(float multiplier)
        {
            maxHealthCoefficient_ = multiplier;
        }

        /**
         * @brief Умножить поглощаемый урон на коэффициент.
         * @param multiplier Множитель поглощаемого урона.
         */
        void multiplyAbsorbedDamage(float multiplier) {absorbedDamageCoefficient_ = multiplier;}

        /**
         * @brief Умножить поглощаемое лечение на коэффициент.
         * @param multiplier Множитель поглощаемого лечения.
         */
        void multiplyAbsorbedHeal(float multiplier) {absorbedHealCoefficient_ = multiplier;}

        /**
         * @brief Обновить состояние здоровья.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @details Применяет регенерацию здоровья.
         */
        void update(float dt);

        /**
         * @brief Проверить, жив ли объект.
         * @return true если здоровье > 0, false в противном случае.
         */
        [[nodiscard]] bool isAlive() const
        {
            #ifndef PARALLEL
            return health_ > 0;
#else
            return health_.load(std::memory_order_relaxed) > 0;
#endif
        }

        /**
         * @brief Убить объект (установить здоровье в 0).
         */
        void kill()
        {
#ifndef PARALLEL
            health_ = 0.f;
#else
            health_.store(0.f, std::memory_order_relaxed);
#endif
        }

        /**
         * @brief Настроить компонент из конфигурации.
         * @param config IConfigReader с параметрами здоровья.
         */
        void setFromConfig(const IConfigReader& config);
    };
}