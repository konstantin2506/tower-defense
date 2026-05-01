#pragma once
#include "type_names.h"
#include "vector2.h"
#include "config/i_config_reader.h"

namespace TowerDefence
{
    /**
     * @class IPathfindingService
     * @brief Сервис поиска пути (pathfinding) для перемещения объектов.
     *
     * Предоставляет интерфейс для вычисления пути перемещения объектов
     * по игровой карте с учетом типа передвижения и препятствий.
     */
    class IPathfindingService {
    public:
        virtual ~IPathfindingService() = default;

        /**
         * @brief Получить следующий вектор движения по пути.
         * @param type MoveType тип передвижения объекта.
         * @param point Vector2<size_t> текущая позиция объекта.
         * @return Vector2<int> вектор следующего шага движения.
         */
        [[nodiscard]] virtual Vector2<int> getPath(MoveType type, Vector2<size_t> point) const = 0;

        /**
         * @brief Обновить данные для поиска пути.
         *
         * Вызывается при изменении карты или препятствий для пересчета
         * доступных путей.
         */
        virtual void update() = 0;
    };
}
