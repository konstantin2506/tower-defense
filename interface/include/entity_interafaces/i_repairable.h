#pragma once

namespace TowerDefence
{
    /**
     * @class IRepairable
     * @brief Интерфейс для объектов, которые можно ремонтировать.
     *
     * Определяет операцию ремонта для объектов, которые могут быть
     * повреждены и требуют восстановления.
     */
    class IRepairable {
    public:
        virtual ~IRepairable() = default;

        /**
         * @brief Отремонтировать объект.
         * @param percent Процент от максимального здоровья для восстановления.
         */
        virtual void repair(float percent) = 0;
    };
}