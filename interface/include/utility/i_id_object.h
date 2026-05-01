#pragma once
#include "type_names.h"
#include "vector2.h"

namespace TowerDefence
{
    /**
     * @class IIDObject
     * @brief Интерфейс для объектов, имеющих уникальный идентификатор и позицию.
     *
     * Базовый интерфейс для всех игровых объектов, которые должны иметь
     * уникальный ID и позицию на игровом поле.
     */
    class IIDObject {
    public:
        virtual ~IIDObject() = default;

        /**
         * @brief Получить ID объекта.
         * @return ID объекта.
         */
        [[nodiscard]] virtual ID getID() const = 0;

        /**
         * @brief Получить позицию объекта.
         * @return Vector2<size_t> позиция объекта.
         */
        [[nodiscard]] virtual Vector2<size_t> getPosition() const = 0;

        /**
         * @brief Установить ID объекта.
         * @param id Новый ID.
         */
        virtual void setID(ID id) = 0;

        /**
         * @brief Установить позицию объекта.
         * @param position Новая позиция Vector2<size_t>.
         */
        virtual void setPosition(Vector2<size_t> position) = 0;
    };
}
