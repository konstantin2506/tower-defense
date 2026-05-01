#pragma once

#include "type_names.h"
#include "utility/entity_type.h"

namespace TowerDefence
{
    /**
     * @class IUpgradable
     * @brief Интерфейс для объектов, которые можно улучшать.
     *
     * Определяет операции для объектов, поддерживающих улучшение уровней,
     * таких как башни или здания.
     */
    class IUpgradable {
    public:
        virtual ~IUpgradable() = default;

        /**
         * @brief Улучшить объект на следующий уровень.
         */
        virtual void upgrade() = 0;

        /**
         * @brief Получить текущий уровень объекта.
         * @return Level текущий уровень.
         */
        [[nodiscard]] virtual Level getLevel() const = 0;

        /**
         * @brief Получить тип улучшаемого объекта.
         * @return const UpgradableType& тип объекта.
         */
        [[nodiscard]] virtual const UpgradableType& getUpgradableType() const = 0;
    };
}
