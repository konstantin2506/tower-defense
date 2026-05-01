#pragma once
#include <iostream>
#include <ostream>

namespace TowerDefence
{
    /**
     * @brief Класс таймера для отслеживания временных интервалов.
     *
     * Используется для контроля перезарядки, анимаций и других временных событий.
     */
    class Timer {
        float passed_; ///< Прошедшее время с последнего сброса.
        float reloadTime_; ///< Время, через которое таймер считается пройденным.

    public:
        /**
         * @brief Конструктор таймера.
         * @param reloadTime Время перезарядки таймера.
         */
        explicit Timer(const float reloadTime): passed_(0), reloadTime_(reloadTime) {}

        /**
         * @brief Сбрасывает таймер в начальное состояние.
         */
        void reset() {passed_ = 0;}

        /**
         * @brief Устанавливает новое время перезарядки.
         * @param reloadTime Новое время перезарядки.
         */
        void setReloadTime(const float reloadTime) {reloadTime_ = reloadTime;}

        /**
         * @brief Обновляет таймер, добавляя прошедшее время.
         * @param dt Время, прошедшее с предыдущего обновления.
         */
        void update(const float dt)
        {
            passed_ += dt;
        }

        /**
         * @brief Проверяет, истекло ли время таймера.
         * @return true если время прошло, иначе false.
         */
        [[nodiscard]] bool isPassed() const {return passed_ >= reloadTime_;}

        /**
         * @brief Возвращает текущее пройденное время.
         * @return Пройденное время.
         */
        [[nodiscard]] float getPassed() const {return passed_;}

        /**
         * @brief Возвращает время перезарядки таймера.
         * @return Время перезарядки.
         */
        [[nodiscard]] float getReloadTime() const {return reloadTime_;}
    };
}
