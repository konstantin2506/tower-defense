#pragma once

#include <iostream>
#include <ostream>

#include "vector2.h"

namespace TowerDefence
{
    /**
     * @class MoveComponent
     * @brief Компонент для управления движением объекта.
     *
     * Инкапсулирует состояние движения объекта: направление взгляда,
     * скорость, накопленное расстояние для перемещения и флаг движения.
     */
    class MoveComponent {
        Vector2<int> sightVector_ = Vector2{1.0f, 1.0f};
        float velocity_;
        float velocityCoefficient_ = 1;
        float ds_ = 0;
        bool isMoving_;
    public:
        /**
         * @brief Конструктор компонента движения.
         * @param velocity Базовая скорость объекта.
         */
        explicit MoveComponent(float velocity) :  velocity_(velocity), isMoving_(true) {}

        /**
         * @brief Умножить скорость на коэффициент.
         * @param multiplier Множитель скорости.
         */
        void multiplyVelocity(float multiplier)  {velocityCoefficient_ = multiplier;}

        /**
         * @brief Установить вектор направления взгляда.
         * @param vector Vector2<int> новый вектор направления.
         */
        void setSightVector(Vector2<int> vector)  {sightVector_ = vector;}

        /**
         * @brief Установить флаг движения.
         * @param boolean true если объект движется, false если остановлен.
         */
        void setMoving(bool boolean) { isMoving_ = boolean; }

        /**
         * @brief Добавить накопленное расстояние за время dt.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @details Формула: ds += dt * velocity * velocityCoefficient.
         */
        void addDs(float dt) {ds_ += dt * velocity_ * velocityCoefficient_; } // no acceleration

        /**
         * @brief Уменьшить накопленное расстояние.
         * @param ds Расстояние, которое было пройдено.
         */
        void moveForDs(float ds) {ds_ -= ds;}

        /**
         * @brief Получить накопленное расстояние для перемещения.
         * @return float накопленное расстояние (distance stock).
         */
        [[nodiscard]] float getDs() const {return ds_;}

        /**
         * @brief Проверить, движется ли объект.
         * @return true если объект движется, false в противном случае.
         */
        [[nodiscard]] bool isMoving() const {return isMoving_;}

        /**
         * @brief Получить вектор направления взгляда.
         * @return Vector2<int> текущий вектор направления.
         */
        [[nodiscard]] Vector2<int> getSightVector() const {return sightVector_;}

        /**
         * @brief Получить базовую скорость.
         * @return float базовая скорость объекта.
         */
        [[nodiscard]] float getVelocity() const  {return velocity_;}

        /**
         * @brief Обновить состояние движения.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @return float накопленное расстояние после обновления.
         */
        [[nodiscard]]float update(float dt);
    };
}