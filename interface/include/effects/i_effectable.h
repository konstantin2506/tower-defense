#pragma once
#include "i_effect.h"

namespace TowerDefence
{
    /**
     * @class IEffectable
     * @brief Шаблонный интерфейс для объектов, к которым можно применять эффекты.
     *
     * Определяет операции для объектов, поддерживающих применение
     * и обновление эффектов (башни, враги, стены и т.д.).
     * @tparam OwnerT Тип объекта, который может иметь эффекты.
     */
    template<typename OwnerT>
    class IEffectable {
    public:
        virtual ~IEffectable() = default;

        /**
         * @brief Применить эффект к объекту.
         * @param effect std::unique_ptr<IEffect<OwnerT>> эффект для применения.
         */
        virtual void applyEffect(std::unique_ptr<IEffect<OwnerT>> effect) = 0;

        /**
         * @brief Обновить все активные эффекты объекта.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        virtual void updateEffects(float dt) = 0;
    };
}
