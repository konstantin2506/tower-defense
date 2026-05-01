#pragma once
#include <concepts>
#include <string>
#include <format>
#include <cmath>

namespace TowerDefence
{
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>; ///< Концепт для арифметических типов.

    /**
     * @brief Шаблонный класс для работы с двумерными векторами.
     *
     * @tparam T Арифметический тип координат.
     */
    template <Arithmetic T>
    class Vector2{
        T x_, y_; ///< Координаты вектора.

    public:
        Vector2() = default; ///< Конструктор по умолчанию.
        Vector2(T x, T y) : x_(x), y_(y) {} ///< Конструктор с координатами.
        Vector2(const Vector2& vec) = default; ///< Конструктор копирования.
        Vector2& operator=(const Vector2& vec) = default; ///< Оператор присваивания копированием.

        bool operator==(const Vector2 & vector2) const = default; ///< Оператор сравнения.

        Vector2(Vector2&& vec) = default; ///< Конструктор перемещения.
        Vector2& operator=(Vector2&& vec) = default; ///< Оператор присваивания перемещением.

        [[nodiscard]] T getX() const {return x_;} ///< Возвращает X координату.
        [[nodiscard]] T getY() const {return y_;} ///< Возвращает Y координату.

        void setX(T x) {x_ = x;} ///< Устанавливает X координату.
        void setY(T y) {y_ = y;} ///< Устанавливает Y координату.

        Vector2 operator+(const Vector2& other) const {return Vector2(x_ + other.x_, y_ + other.y_);} ///< Сложение векторов.
        Vector2 operator-(const Vector2& other) const {return Vector2(x_ - other.x_, y_ - other.y_);} ///< Вычитание векторов.

        explicit operator std::pair<T, T>() const {return std::make_pair(x_, y_);} ///< Преобразование в пару.
        operator Vector2<int>() const {return Vector2<int>(x_, y_);} ///< Преобразование в Vector2<int>.
        operator Vector2<size_t>() const {return Vector2<size_t>(x_, y_);} ///< Преобразование в Vector2<size_t>.

        /**
         * @brief Преобразует вектор в строку.
         * @return Строковое представление вектора.
         */
        [[nodiscard]] std::string toString() const {return std::format("({},{})", x_, y_);}

        /**
         * @brief Вычисляет евклидово расстояние между двумя точками.
         * @tparam type Тип координат.
         * @param point1 Первая точка.
         * @param point2 Вторая точка.
         * @return Расстояние между точками.
         */
        template<Arithmetic type>
        static double distance(const Vector2<type> point1, const Vector2<type> point2)
        {
            const auto dx = static_cast<double>(point1.getX() - point2.getX());
            const auto dy = static_cast<double>(point1.getY() - point2.getY());
            return std::sqrt(dx*dx + dy*dy);
        }

        /**
         * @brief Вычисляет квадрат расстояния между двумя точками.
         * @tparam type Тип координат.
         * @param point1 Первая точка.
         * @param point2 Вторая точка.
         * @return Квадрат расстояния между точками.
         */
        template<Arithmetic type>
        static type distanceQ(const Vector2<type> point1, const Vector2<type> point2)
        {
            auto xmax = std::max(point1.getX(), point2.getX());
            auto ymax = std::max(point1.getY(), point2.getY());
            auto xmin = std::min(point1.getX(), point2.getX());
            auto ymin = std::min(point1.getY(), point2.getY());
            const auto dx = static_cast<double>(xmax - xmin);
            const auto dy = static_cast<double>(ymax - ymin);
            return dx*dx + dy*dy;
        }

        /**
         * @brief Проверяет, находится ли точка внутри прямоугольника.
         * @param lu Левая верхняя точка прямоугольника.
         * @param rd Правая нижняя точка прямоугольника.
         * @return true если точка внутри прямоугольника.
         */
        [[nodiscard]] bool in(Vector2 lu, Vector2 rd)
        {
            return (x_ <= rd.x_ && x_ >= lu.x_) && (y_ <= rd.y_ && y_ >= lu.y_);
        }
    };
}