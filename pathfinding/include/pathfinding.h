#pragma once

#include "template_matrix.h"
#include "pathfinding/i_pathfinding_service.h"
#include "world_map/i_world_map.h"

#include <unordered_map>

namespace TowerDefence
{
    /**
     * @brief Сервис для расчета путей движения к замку.
     *
     * Вычисляет и хранит карты направлений для различных типов движения.
     * Использует алгоритмы поиска пути для создания векторных карт,
     * указывающих направление к замку из каждой точки карты.
     */
    class PathfindingService final: public IPathfindingService {

        IWorldMap& worldMap_; ///< Ссылка на игровую карту для получения информации о тайлах.

        /// Мапа для хранения карт путей для каждого типа движения.
        /// Ключ - MoveType, значение - матрица векторов направления.
        std::unordered_map<MoveType, Matrix<Vector2<int>>> pathMaps_;

        /// Мапа для хранения весов тайлов для каждого типа движения.
        /// Ключ - MoveType, значение - мапа весов тайлов.
        std::unordered_map<MoveType, std::unordered_map<TileType, size_t>> weightsMap_;

        Vector2<size_t> castlePosition_; ///< Позиция замка (целевая точка для всех путей).

    public:
        /**
         * @brief Конструктор сервиса поиска пути.
         * @param config Конфигурация с весами тайлов для разных типов движения.
         * @param worldMap Ссылка на игровую карту.
         * @param castlePosition Позиция замка на карте.
         */
        PathfindingService(const IConfigReader& config, IWorldMap& worldMap, Vector2<size_t> castlePosition);

        /**
         * @brief Возвращает вектор направления движения из указанной точки.
         * @param type Тип движения (наземный, летающий и т.д.).
         * @param point Текущая позиция.
         * @return Вектор направления к замку.
         */
        Vector2<int> getPath(MoveType type, Vector2<size_t> point) const override;

        /**
         * @brief Обновляет карты путей (пересчитывает при изменении карты).
         */
        void update() override;

    private:
        /**
         * @brief Вычисляет карту путей для заданного типа движения.
         * @param type Тип движения.
         * @return Матрица векторов направления.
         */
        Matrix<Vector2<int>> calculatePathMap(MoveType type) const;

        /**
         * @brief Добавляет конфигурацию для нового типа движения.
         * @param type Тип движения.
         * @param config Конфигурация с весами тайлов для этого типа.
         */
        void addMoveType(MoveType type, const IConfigReader& config);

    };
}