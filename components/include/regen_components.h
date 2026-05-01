#pragma once
#include <memory>

namespace TowerDefence
{
    class HealthComponent;

    /**
     * @class IRegenable
     * @brief Интерфейс компонента регенерации здоровья.
     *
     * Определяет базовые операции для всех компонентов регенерации:
     * вычисление скорости регенерации и клонирование.
     */
    class IRegenable {
    public:
        virtual ~IRegenable()  = default;

        /**
         * @brief Вычислить скорость регенерации.
         * @param healthComponent Ссылка на компонент здоровья.
         * @return float скорость регенерации (здоровье в секунду).
         */
        virtual float calculateRegenSpeed(const HealthComponent& healthComponent) = 0;

        /**
         * @brief Создать копию компонента регенерации.
         * @return std::unique_ptr<IRegenable> клонированный компонент.
         */
        [[nodiscard]] virtual std::unique_ptr<IRegenable> clone() const = 0;
    };

    /**
     * @class BasicRegenable
     * @brief Базовая реализация компонента регенерации.
     *
     * Вычисляет скорость регенерации как коэффициент от максимального здоровья.
     */
    class BasicRegenable final : public IRegenable {
        float coefficient_ = 1;
    public:
        /**
         * @brief Конструктор базового компонента регенерации.
         * @param coefficient Коэффициент регенерации (доля от максимального здоровья в секунду).
         */
        explicit BasicRegenable(float coefficient) : coefficient_(coefficient) {}

        float calculateRegenSpeed(const HealthComponent& healthComponent) override;

        /**
         * @brief Создать копию компонента регенерации.
         * @return std::unique_ptr<IRegenable> клонированный компонент.
         */
        [[nodiscard]] std::unique_ptr<IRegenable> clone() const override { return std::make_unique<BasicRegenable>(*this);}
    };
}