#pragma once

namespace TowerDefence
{
    /**
     * @class IUpdatable
     * @brief Интерфейс для объектов, которые могут обновлять свое состояние.
     *
     * Шаблонный интерфейс, определяющий метод update для объектов,
     * которые должны обновляться каждый игровой кадр.
     * @tparam ReturnValue Тип возвращаемого значения метода update.
     */
    template <typename ReturnValue>
    class IUpdatable {
    public:
        virtual ~IUpdatable() = default;

        /**
         * @brief Обновить состояние объекта.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @return ReturnValue результат обновления.
         */
        virtual ReturnValue update(float dt) = 0;
    };
}