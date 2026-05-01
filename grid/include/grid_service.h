#pragma once

#include "utility/i_grid_service.h"
#include "type_names.h"
#include "vector2.h"
#include "template_matrix.h"

#include <vector>

namespace TowerDefence
{
    /**
     * @brief Реализация IGridService, позволяющая хранить несколько ID в одной ячейке.
     *
     * Использует матрицу векторов для хранения нескольких идентификаторов в каждой ячейке.
     * Поддерживает многопоточный доступ при включенном флаге PARALLEL.
     */
    class VecIDGridService final: public IGridService {
        Matrix<std::vector<ID>> data_; ///< Матрица векторов для хранения ID в каждой ячейке.
#ifdef PARALLEL
        Matrix<std::mutex> cellMutexes_; ///< Матрица мьютексов для синхронизации доступа к ячейкам.
#endif
    public:
        /**
         * @brief Конструктор удален - обязательны размеры.
         */
        VecIDGridService() = delete;

        /**
         * @brief Конструктор с заданием размеров сетки.
         * @param rows Количество строк.
         * @param columns Количество столбцов.
         */
        VecIDGridService(const size_t rows, const size_t columns): data_(rows, columns, std::vector<ID>(1, 0))
        #ifdef PARALLEL
            , cellMutexes_(rows, columns)
        #endif
        {}

        /**
         * @brief Перемещает ID из одной ячейки в другую.
         * @param id Идентификатор для перемещения.
         * @param src Исходная позиция.
         * @param dst Целевая позиция.
         * @return true если перемещение успешно, false если ID не найден в исходной позиции.
         */
        bool replace(ID id, Vector2<size_t> src, Vector2<size_t> dst) override;

        /**
         * @brief Возвращает все ID в указанной точке.
         * @param point Позиция для проверки.
         * @return Вектор ID, находящихся в этой ячейке.
         */
        [[nodiscard]] std::vector<ID> atPoint(Vector2<size_t> point) const override;

        /**
         * @brief Возвращает все ID в заданном радиусе от точки.
         * @param point Центральная точка.
         * @param radius Радиус поиска.
         * @return Вектор ID, находящихся в пределах радиуса.
         */
        [[nodiscard]] std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const override;

        /**
         * @brief Добавляет ID в указанную ячейку.
         * @param id Идентификатор для добавления.
         * @param point Позиция для добавления.
         * @return Всегда true (для совместимости с интерфейсом).
         */
        bool add(ID id, Vector2<size_t> point) override;

        /**
         * @brief Очищает ячейку, удаляя все ID.
         * @param point Позиция ячейки для очистки.
         */
        void reset(Vector2<::size_t> point) override {data_.at(point.getX(), point.getY()).clear();}

        /**
         * @brief Удаляет конкретный ID из ячейки.
         * @param point Позиция ячейки.
         * @param id Идентификатор для удаления.
         * @throw std::invalid_argument если ID не найден в ячейке.
         */
        void remove(Vector2<::size_t> point, ID id) override
        {
            auto& vec = data_.at(point.getX(), point.getY());
            auto it = std::ranges::find(vec, id);
            if (it != vec.end())
                vec.erase(it);
            else
                throw std::invalid_argument("No such ID" + std::to_string(id));
        }

        /**
         * @brief Находит ближайший ID к заданной точке в пределах радиуса.
         * @param point Центральная точка.
         * @param radius Радиус поиска.
         * @return ID ближайшего объекта или INVALID_ID, если ничего не найдено.
         */
    };

    /**
     * @brief Реализация IGridService, позволяющая хранить только один ID в ячейке.
     *
     * Использует матрицу одиночных значений ID. Каждая ячейка может содержать
     * только один идентификатор (или 0 если пуста).
     */
    class SingleIDGridService final: public IGridService {
        Matrix<ID> data_; ///< Матрица для хранения одного ID в каждой ячейке.
    public:
        /**
         * @brief Конструктор удален - обязательны размеры.
         */
        SingleIDGridService() = delete;

        /**
         * @brief Конструктор с заданием размеров сетки.
         * @param rows Количество строк.
         * @param columns Количество столбцов.
         */
        SingleIDGridService(const size_t rows, const size_t columns): data_(rows, columns, 0) {}

        /**
         * @brief Возвращает ID в указанной точке.
         * @param point Позиция для проверки.
         * @return Вектор с одним ID (или пустой, если ячейка пуста).
         */
        [[nodiscard]] std::vector<ID> atPoint(Vector2<size_t> point) const override;

        /**
         * @brief Возвращает все ID в заданном радиусе от точки.
         * @param point Центральная точка.
         * @param radius Радиус поиска.
         * @return Вектор ID, находящихся в пределах радиуса.
         */
        [[nodiscard]] std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const override;

        /**
         * @brief Добавляет ID в указанную ячейку.
         * @param id Идентификатор для добавления.
         * @param point Позиция для добавления.
         * @return true если ячейка была пуста, false если уже занята.
         */
        bool add(ID id, Vector2<size_t> point) override;

        /**
         * @brief Перемещает ID из одной ячейки в другую.
         * @param id Идентификатор для перемещения.
         * @param src Исходная позиция.
         * @param dst Целевая позиция.
         * @return true если перемещение успешно, false если ID не найден в исходной позиции.
         */
        bool replace(ID id, Vector2<size_t> src, Vector2<size_t> dst) override;

        /**
         * @brief Очищает ячейку, устанавливая значение 0.
         * @param point Позиция ячейки для очистки.
         */
        void reset(Vector2<::size_t> point) override {data_.at(point.getX(), point.getY()) = 0;}

        /**
         * @brief Удаляет ID из ячейки, если он там есть.
         * @param point Позиция ячейки.
         * @param id Идентификатор для удаления.
         */
        void remove(Vector2<size_t> point, ID id) override {data_.at(point.getX(), point.getY()) = 0;}

        /**
         * @brief Находит ближайший ID к заданной точке в пределах радиуса.
         * @param point Центральная точка.
         * @param radius Радиус поиска.
         * @return ID ближайшего объекта или INVALID_ID, если ничего не найдено.
         */

    };

    /**
     * @brief Вспомогательная функция для получения значений в радиусе от точки.
     * @tparam T Тип значений в матрице.
     * @param matrix Матрица для поиска.
     * @param point Центральная точка.
     * @param radius Радиус поиска.
     * @return Вектор значений, находящихся в пределах радиуса от точки.
     */
    template<typename T>
    std::vector<T> getInRadius(const Matrix<T>& matrix, Vector2<size_t> point, size_t radius){
        std::vector<T> result;
        size_t startX = std::max(static_cast<int>(point.getX()) - static_cast<int>(radius), 0);
        size_t startY = std::max(static_cast<int>(point.getY()) - static_cast<int>(radius), 0);
        size_t endX = std::min(point.getX() + radius, matrix.rows() - 1);
        size_t endY = std::min(point.getY() + radius, matrix.cols() - 1);
        auto rq = radius * radius;

        for (auto x = startX; x <= endX; ++x) {
            for (auto y = startY; y <= endY; ++y) {
                if (Vector2<size_t>::distanceQ(point, Vector2{x, y}) <= rq) {
                    result.emplace_back(matrix[x][y]);
                }
            }
        }
        return result;
    }
}