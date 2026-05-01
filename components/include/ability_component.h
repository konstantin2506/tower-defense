#pragma once

#include "ability/i_ability.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace TowerDefence
{
    /**
     * @class AbilityComponent
     * @brief Компонент для управления способностями объекта.
     *
     * Инкапсулирует коллекцию способностей (IAbility) и предоставляет
     * методы для их добавления и использования. Поддерживает копирование
     * и перемещение через конструкторы и операторы присваивания.
     */
    class AbilityComponent {
        std::vector<std::unique_ptr<IAbility>> abilities_;
    public:
        AbilityComponent() = default;

        /**
         * @brief Конструктор с инициализацией способностей.
         * @param abilities std::vector<std::unique_ptr<IAbility>> начальные способности.
         */
        explicit AbilityComponent(std::vector<std::unique_ptr<IAbility>> abilities) : abilities_(std::move(abilities)) {}

        AbilityComponent(const AbilityComponent& other) = default;
        AbilityComponent(AbilityComponent&& other) = default;
        AbilityComponent& operator=(const AbilityComponent& other) = default;
        AbilityComponent& operator=(AbilityComponent&& other) = default;

        /**
         * @brief Добавить способность в компонент.
         * @param ability std::unique_ptr<IAbility> новая способность.
         */
        void addAbility(std::unique_ptr<IAbility> ability);

        /**
         * @brief Попытаться использовать все способности.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         * @param owner Ссылка на врага-владельца способностей (IEnemy&).
         */
        void tryUseAll(float dt, IEnemy& owner);

        /**
         * @brief Получить все способности.
         * @return std::vector<std::unique_ptr<IAbility>> копии всех способностей.
         */
        [[nodiscard]] std::vector<std::unique_ptr<IAbility>> getAllAbilities() const;
    };
}