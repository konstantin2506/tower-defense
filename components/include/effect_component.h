#pragma once

#include "effects/i_effect.h"

#include <vector>
#include <algorithm>


namespace TowerDefence
{
    /**
     * @class EffectComponent
     * @brief Шаблонный компонент для управления эффектами объекта.
     *
     * Инкапсулирует коллекцию эффектов (IEffect) и предоставляет
     * методы для их добавления и обновления. Автоматически применяет
     * эффект при добавлении и обновляет все эффекты каждый кадр.
     * @tparam OwnerT Тип объекта-владельца эффектов.
     */
    template<typename OwnerT>
    class EffectComponent {
        std::vector<std::unique_ptr<IEffect<OwnerT>>> effects_;

    public:
        EffectComponent() = default;
        EffectComponent(const EffectComponent<OwnerT>& other) = default;

        /**
         * @brief Добавить эффект к объекту.
         * @param effect std::unique_ptr<IEffect<OwnerT>> эффект для добавления.
         * @param owner Ссылка на объект-владелец эффекта.
         */
        void addEffect(std::unique_ptr<IEffect<OwnerT>> effect, OwnerT& owner)
        {
            effect->applyEffect(owner);
            effects_.push_back(std::move(effect));
        }

        /**
         * @brief Обновить все эффекты объекта.
         * @param owner Ссылка на объект-владелец эффектов.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        void updateAllEffects(OwnerT& owner, float dt)
        {
            std::ranges::for_each(effects_, [&](auto& effect)
            {
                effect->updateEffect(owner, dt);
            });
        }
    };
}