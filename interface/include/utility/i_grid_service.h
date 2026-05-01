#pragma once
#include "type_names.h"
#include "vector2.h"

namespace TowerDefence
{
    /**
     * @class IGridService
     * @brief Сервис для работы с игровой сеткой (grid).
     *
     * Управляет расположением объектов на игровом поле, предоставляя методы
     * для добавления, перемещения, удаления и поиска объектов по позициям.
     */
    class IGridService {
    public:
        virtual ~IGridService() = default;

        /**
         * @brief Переместить объект с одной позиции на другую.
         * @param id ID перемещаемого объекта.
         * @param src Исходная позиция Vector2<size_t>.
         * @param dst Целевая позиция Vector2<size_t>.
         * @return true если перемещение успешно, false в противном случае.
         */
        virtual bool replace(ID id, Vector2<size_t> src, Vector2<size_t> dst) = 0;

        /**
         * @brief Получить все объекты в указанной точке.
         * @param point Точка Vector2<size_t> для проверки.
         * @return std::vector<ID> объектов в точке.
         */
        [[nodiscard]] virtual std::vector<ID> atPoint(Vector2<size_t> point) const = 0;

        /**
         * @brief Получить объекты в радиусе от точки.
         * @param point Центральная точка Vector2<size_t>.
         * @param radius Радиус поиска.
         * @return std::vector<ID> объектов в радиусе.
         */
        [[nodiscard]] virtual std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const = 0;

        /**
         * @brief Сбросить (очистить) все объекты в указанной точке.
         * @param point Точка Vector2<size_t> для сброса.
         */
        virtual void reset(Vector2<size_t> point) = 0;

        /**
         * @brief Добавить объект в указанную точку.
         * @param id ID добавляемого объекта.
         * @param point Точка Vector2<size_t> для добавления.
         * @return true если добавление успешно, false в противном случае.
         */
        [[nodiscard]] virtual bool add(ID id, Vector2<size_t> point) = 0;

        /**
         * @brief Удалить объект из указанной точки.
         * @param point Точка Vector2<size_t>.
         * @param id ID удаляемого объекта.
         */
        virtual void remove(Vector2<size_t> point, ID id) = 0;

    };
}
