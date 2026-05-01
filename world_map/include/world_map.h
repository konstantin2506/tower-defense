#pragma once
#include "template_matrix.h"
#include "config/i_config_reader.h"
#include "world_map/i_world_map.h"

namespace TowerDefence
{
    /**
     * @brief Реализация игровой карты.
     *
     * Хранит матрицу тайлов и предоставляет методы для работы с картой.
     * Отслеживает состояние обновления карты для оптимизации перерасчетов.
     */
    class WorldMap final: public IWorldMap {
        Matrix<TileType> fullMap_; ///< Матрица, содержащая типы тайлов карты.
        bool isUpdated_ = true; ///< Флаг, указывающий на необходимость обновления зависимых систем.

    public:
        /**
         * @brief Конструктор карты.
         * @param config Конфигурация для загрузки карты.
         */
        explicit WorldMap(const IConfigReader& config);

        /**
         * @brief Проверяет, является ли указанная точка пустой (LAND).
         * @param point Проверяемая точка.
         * @return true если тайл является LAND.
         */
        [[nodiscard]] bool isEmpty(Vector2<size_t> point) const override;

        /**
         * @brief Проверяет, является ли указанная точка пустой согласно критерию.
         * @param point Проверяемая точка.
         * @param emptyCriteria Критерий пустоты (какой тип тайла считать пустым).
         * @return true если тайл соответствует критерию пустоты.
         */
        [[nodiscard]] bool isEmptyWithCriteria(Vector2<size_t> point, TileType emptyCriteria) const override;

        /**
         * @brief Устанавливает тип тайла в указанной точке.
         * @param point Позиция тайла.
         * @param type Новый тип тайла.
         * @note Устанавливает флаг isUpdated_ в true, указывая на изменение карты.
         */
        void setTileType(Vector2<size_t> point, TileType type) override;

        /**
         * @brief Сбрасывает флаг обновления (помечает карту как обработанную).
         */
        void updateHandled() override {isUpdated_ = false;}

        /**
         * @brief Проверяет, была ли карта обновлена с момента последней обработки.
         * @return true если карта была изменена и требует обновления зависимых систем.
         */
        [[nodiscard]] bool isUpdated() const override {return isUpdated_;}

        /**
         * @brief Возвращает размер карты.
         * @return Вектор {количество строк, количество столбцов}.
         */
        [[nodiscard]] Vector2<size_t> size() const override {return {fullMap_.rows(), fullMap_.cols()};}

        /**
         * @brief Возвращает тип тайла по координатам.
         * @param x Координата X (строка).
         * @param y Координата Y (столбец).
         * @return Тип тайла в указанной позиции.
         */
        [[nodiscard]] TileType getTile(size_t x, size_t y) const override {return fullMap_[x][y];}

        /**
         * @brief Сбрасывает состояние карты (устанавливает флаг обновления).
         */
        void reset() override {isUpdated_ = true;}
    };
}