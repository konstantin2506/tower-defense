#pragma once

#include "config/i_configurable.h"
#include "type_names.h"

namespace TowerDefence
{
    /**
     * @class LevelComponent
     * @brief Компонент для управления уровнем объекта.
     *
     * Инкапсулирует текущий и максимальный уровень объекта.
     * Реализует интерфейс IConfigurable для загрузки из конфигурации.
     */
    class LevelComponent : public IConfigurable{
        Level currentLevel_;
        Level maxLevel_;
    public:
        /**
         * @brief Конструктор компонента уровня.
         * @param maxLevel Максимальный уровень.
         * @param currentLevel Текущий уровень (по умолчанию 1).
         */
        explicit LevelComponent(Level maxLevel, Level currentLevel = 1) : currentLevel_(currentLevel), maxLevel_(maxLevel) {}

        LevelComponent(const LevelComponent &other)  = default;
        LevelComponent& operator=(const LevelComponent &other) = default;
        LevelComponent(LevelComponent &&other) = default;
        LevelComponent& operator=(LevelComponent &&other) = default;

        /**
         * @brief Получить текущий уровень.
         * @return Level текущий уровень.
         */
        [[nodiscard]] Level getCurrentLevel() const {return currentLevel_;}

        /**
         * @brief Получить максимальный уровень.
         * @return Level максимальный уровень.
         */
        [[nodiscard]] Level getMaxLevel() const {return maxLevel_;}

        /**
         * @brief Увеличить текущий уровень.
         * @return true если достигнут максимальный уровень, false в противном случае.
         */
        bool incrementCurrentLevel() {return ++currentLevel_ >= maxLevel_;}

        /**
         * @brief Настроить компонент из конфигурации.
         * @param cfg IConfigReader с параметрами уровня.
         */
        void setFromConfig(const IConfigReader &cfg) override;
    };
}