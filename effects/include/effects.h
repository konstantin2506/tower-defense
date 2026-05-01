#pragma once

#include <memory>

#include "effects/i_effect.h"
#include "entity_interafaces/i_healthable.h"
#include "entity_interafaces/i_movable.h"
#include "timer.h"


namespace TowerDefence
{
    /**
     * @concept HasInterface
     * @brief Концепт проверяет, что тип T является наследником Interface.
     * @tparam T Проверяемый тип.
     * @tparam Interface Целевой интерфейс.
     */
    template<typename T, typename Interface>
    concept HasInterface = std::derived_from<T, Interface>;

    /**
     * @class BasicEffect
     * @brief Базовый класс для всех временных эффектов.
     *
     * Шаблонный класс, предоставляющий общую функциональность для эффектов:
     * таймер длительности, множитель эффекта и управление состоянием завершения.
     * @tparam OwnerT Тип объекта, к которому применяется эффект.
     */
    template<typename OwnerT>
    class BasicEffect: public IEffect<OwnerT> {
    protected:
        Timer timer_{1};          ///< Таймер длительности эффекта
        float multiplier_ = 1;    ///< Множитель эффекта
        bool finished_ = false;   ///< Флаг завершения эффекта
    public:
        BasicEffect() = default;

        /**
         * @brief Конструктор базового эффекта.
         * @param duration Длительность эффекта в секундах.
         * @param multiplier Множитель эффекта.
         */
        BasicEffect(float duration, float multiplier) : timer_(duration), multiplier_(multiplier) {}

        /**
         * @brief Обновить состояние эффекта.
         * @param owner Ссылка на объект-владелец эффекта.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @details Уменьшает время действия эффекта и удаляет его при истечении.
         */
        void updateEffect(OwnerT& owner, float dt) override
        {
            timer_.update(dt);
            if (!finished_ && timer_.isPassed()) {
                this->removeEffect(owner);
                finished_ = true;
            }
        }

        /**
         * @brief Проверить, завершился ли эффект.
         * @return true если эффект завершился, false в противном случае.
         */
        [[nodiscard]] bool isFinished() const override {return finished_;}

        /**
         * @brief Получить множитель эффекта.
         * @return float множитель эффекта.
         */
        [[nodiscard]] float getMultiplier() const override {return multiplier_;}

        /**
         * @brief Настроить эффект из конфигурации.
         * @param config IConfigReader с параметрами эффекта.
         */
        void setFromConfig(const IConfigReader &config) override
        {
            timer_.setReloadTime(config.getFloat("duration"));
            multiplier_ = config.getFloat("multiplier");
        }
    };

    /**
     * @class VelocityEffect
     * @brief Эффект изменения скорости движения.
     *
     * Временно изменяет скорость движения объекта. Наследуется от BasicEffect.
     * @tparam OwnerT Тип объекта-владельца (должен реализовывать IMovable).
     * @requires OwnerT должен удовлетворять концепту HasInterface<OwnerT, IMovable>.
     */
    template<typename OwnerT>
    // requires HasInterface<OwnerT, IMovable>
    class VelocityEffect final : public BasicEffect<OwnerT> {
    public:
        VelocityEffect() = default;

        /**
         * @brief Конструктор эффекта скорости.
         * @param duration Длительность эффекта в секундах.
         * @param multiplier Множитель скорости.
         */
        VelocityEffect(float duration, float multiplier) : BasicEffect<OwnerT>(duration, multiplier) {}

        /**
         * @brief Применить эффект к объекту.
         * @param owner Ссылка на объект-владелец.
         */
        void applyEffect(OwnerT& owner) override
        {
            owner.multiplyVelocity(this->multiplier_);
        }

        /**
         * @brief Удалить эффект с объекта.
         * @param owner Ссылка на объект-владелец.
         */
        void removeEffect(OwnerT& owner) override
        {
            owner.multiplyVelocity(1);
        }

        /**
         * @brief Создать копию эффекта.
         * @return std::unique_ptr<IEffect<OwnerT>> клонированный эффект.
         */
        std::unique_ptr<IEffect<OwnerT>> clone() override
        {
            return std::make_unique<VelocityEffect>(this->timer_.getReloadTime(), this->multiplier_);
        }
    };

    /**
     * @class MaxHealthEffect
     * @brief Эффект изменения максимального здоровья.
     *
     * Временно изменяет максимальное здоровье объекта. Наследуется от BasicEffect.
     * @tparam OwnerT Тип объекта-владельца (должен реализовывать IHealthable).
     * @requires OwnerT должен удовлетворять концепту HasInterface<OwnerT, IHealthable>.
     */
    template<typename OwnerT>
    requires HasInterface<OwnerT, IHealthable>
    class MaxHealthEffect final : public BasicEffect<OwnerT> {
    public:
        MaxHealthEffect() = default;

        /**
         * @brief Конструктор эффекта максимального здоровья.
         * @param duration Длительность эффекта в секундах.
         * @param multiplier Множитель максимального здоровья.
         */
        MaxHealthEffect(float duration, float multiplier) : BasicEffect<OwnerT>(duration, multiplier) {}

        /**
         * @brief Применить эффект к объекту.
         * @param owner Ссылка на объект-владелец.
         */
        void applyEffect(OwnerT& owner) override
        {
            owner.multiplyMaxHealth(this->multiplier_);
        }

        /**
         * @brief Удалить эффект с объекта.
         * @param owner Ссылка на объект-владелец.
         */
        void removeEffect(OwnerT& owner) override
        {
            owner.multiplyMaxHealth(1);
        }

        /**
         * @brief Создать копию эффекта.
         * @return std::unique_ptr<IEffect<OwnerT>> клонированный эффект.
         */
        std::unique_ptr<IEffect<OwnerT>> clone() override
        {
            return std::make_unique<MaxHealthEffect>(this->timer_.getReloadTime(), this->multiplier_);
        }
    };

    /**
     * @class AbsorbedDamageEffect
     * @brief Эффект изменения поглощаемого урона.
     *
     * Временно изменяет коэффициент поглощения урона объектом. Наследуется от BasicEffect.
     * @tparam OwnerT Тип объекта-владельца (должен реализовывать IHealthable).
     * @requires OwnerT должен удовлетворять концепту HasInterface<OwnerT, IHealthable>.
     */
    template<typename OwnerT>
    requires HasInterface<OwnerT, IHealthable>
    class AbsorbedDamageEffect final : public BasicEffect<OwnerT> {
    public:
        AbsorbedDamageEffect() = default;

        /**
         * @brief Конструктор эффекта поглощения урона.
         * @param duration Длительность эффекта в секундах.
         * @param multiplier Множитель поглощаемого урона.
         */
        AbsorbedDamageEffect(float duration, float multiplier) : BasicEffect<OwnerT>(duration, multiplier) {}

        /**
         * @brief Применить эффект к объекту.
         * @param owner Ссылка на объект-владелец.
         */
        void applyEffect(OwnerT& owner) override
        {
            owner.multiplyAbsorbedDamage(this->multiplier_);
        }

        /**
         * @brief Удалить эффект с объекта.
         * @param owner Ссылка на объект-владелец.
         */
        void removeEffect(OwnerT& owner) override
        {
            owner.multiplyAbsorbedDamage(1);
        }

        /**
         * @brief Создать копию эффекта.
         * @return std::unique_ptr<IEffect<OwnerT>> клонированный эффект.
         */
        std::unique_ptr<IEffect<OwnerT>> clone() override
        {
            return std::make_unique<AbsorbedDamageEffect>(this->timer_.getReloadTime(), this->multiplier_);
        }
    };

    /**
     * @class AbsorbedHealEffect
     * @brief Эффект изменения поглощаемого лечения.
     *
     * Временно изменяет коэффициент поглощения лечения объектом. Наследуется от BasicEffect.
     * @tparam OwnerT Тип объекта-владельца (должен реализовывать IHealthable).
     * @requires OwnerT должен удовлетворять концепту HasInterface<OwnerT, IHealthable>.
     */
    template<typename OwnerT>
    requires HasInterface<OwnerT, IHealthable>
    class AbsorbedHealEffect final : public BasicEffect<OwnerT> {
    public:
        AbsorbedHealEffect() = default;

        /**
         * @brief Конструктор эффекта поглощения лечения.
         * @param duration Длительность эффекта в секундах.
         * @param multiplier Множитель поглощаемого лечения.
         */
        AbsorbedHealEffect(float duration, float multiplier) : BasicEffect<OwnerT>(duration, multiplier) {}

        /**
         * @brief Применить эффект к объекту.
         * @param owner Ссылка на объект-владелец.
         */
        void applyEffect(OwnerT& owner) override
        {
            owner.multiplyAbsorbedHeal(this->multiplier_);
        }

        /**
         * @brief Удалить эффект с объекта.
         * @param owner Ссылка на объект-владелец.
         */
        void removeEffect(OwnerT& owner) override
        {
            owner.multiplyAbsorbedHeal(1);
        }

        /**
         * @brief Создать копию эффекта.
         * @return std::unique_ptr<IEffect<OwnerT>> клонированный эффект.
         */
        std::unique_ptr<IEffect<OwnerT>> clone() override
        {
            return std::make_unique<AbsorbedHealEffect>(this->timer_.getReloadTime(), this->multiplier_);
        }
    };
}