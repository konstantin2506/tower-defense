#pragma once
#include "type_names.h"
#include "vector2.h"

namespace TowerDefence
{
    /**
     * @class IWorldMap
     * @brief Интерфейс игровой карты.
     *
     * Предоставляет доступ к данным игровой карты: проверка доступности клеток,
     * получение типа тайлов, управление обновлениями карты.
     */
    class IWorldMap {
    public:
        virtual ~IWorldMap() = default;

        /**
         * @brief Проверить, пуста ли клетка на карте.
         * @param point Vector2<size_t> позиция для проверки.
         * @return true если клетка пуста, false в противном случае.
         */
        [[nodiscard]] virtual bool isEmpty(Vector2<size_t> point) const = 0;

        /**
         * @brief Проверить, пуста ли клетка по определенному критерию.
         * @param point Vector2<size_t> позиция для проверки.
         * @param emptyCriteria TileType критерий "пустоты" клетки.
         * @return true если клетка соответствует критерию, false в противном случае.
         */
        [[nodiscard]] virtual bool isEmptyWithCriteria(Vector2<size_t> point, TileType emptyCriteria) const = 0;

        /**
         * @brief Получить размеры карты.
         * @return Vector2<size_t> размеры карты (ширина, высота).
         */
        [[nodiscard]] virtual Vector2<size_t> size() const = 0;

        /**
         * @brief Проверить, была ли карта обновлена.
         * @return true если карта была обновлена, false в противном случае.
         */
        [[nodiscard]] virtual bool isUpdated() const = 0;

        /**
         * @brief Установить тип тайла на указанной позиции.
         * @param point Vector2<size_t> позиция тайла.
         * @param type TileType новый тип тайла.
         */
        virtual void setTileType(Vector2<size_t> point, TileType type) = 0;

        /**
         * @brief Получить тип тайла по координатам.
         * @param x Координата X.
         * @param y Координата Y.
         * @return TileType тип тайла.
         */
        [[nodiscard]] virtual TileType getTile(size_t x, size_t y) const = 0;

        /**
         * @brief Отметить обновление карты как обработанное.
         *
         * Используется для сброса флага обновления после того, как
         * зависимые системы (например, pathfinding) обработали изменения.
         */
        virtual void updateHandled() = 0;

        /**
         * @brief Сбросить состояние карты.
         */
        virtual void reset() = 0;
    };
}
