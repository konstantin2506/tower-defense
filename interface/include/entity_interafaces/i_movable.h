#pragma once
#include "vector2.h"
#include "type_names.h"

namespace TowerDefence
{
    /**
     * @class IMovable
     * @brief Интерфейс для подвижных объектов в игре.
     *
     * Определяет базовые операции для всех объектов, способных перемещаться
     * по игровому полю, включая управление скоростью и направлением взгляда.
     */
    class IMovable {
    public:
        virtual ~IMovable() = default;

        /**
         * @brief Переместить объект на заданный вектор.
         * @param vector Vector2<int> вектор перемещения.
         */
        virtual void move(Vector2<int> vector) = 0;

        /**
         * @brief Умножить скорость объекта.
         * @param multiplier Множитель скорости.
         */
        virtual void multiplyVelocity(float multiplier) = 0;
        /**
         * @brief Получить тип передвижения объекта.
         * @return MoveType тип передвижения (например, LANDED, AIR).
         */
        [[nodiscard]] virtual MoveType getMoveType() const = 0;
        /**
         * @brief Получить текущее возможное перемещение.
         * @return float перемещение (ds).
         */
        [[nodiscard]] virtual float getDs() const = 0;

        /**
         * @brief Получить вектор направления взгляда.
         * @return Vector2<int> вектор направления.
         */
        [[nodiscard]] virtual Vector2<int> getSightVector() const = 0;

        /**
         * @brief Установить вектор направления взгляда.
         * @param vec Vector2<int> новый вектор направления.
         */
        virtual void setSightVector(Vector2<int> vec) = 0;
    };
}
