#pragma once

#include <functional>
#include <utility>

namespace TowerDefence
{
    /**
     * @brief Функтор для хеширования пар значений.
     *
     * Предоставляет оператор для вычисления хеша пары любых типов,
     * используя XOR комбинацию хешей элементов пары.
     */
    class PairHasher {
    public:
        /**
         * @brief Вычисляет хеш для пары значений.
         * @tparam FirstT Тип первого элемента пары.
         * @tparam SecondT Тип второго элемента пары.
         * @param pair Пара для хеширования.
         * @return Хеш-значение пары.
         */
        template <typename FirstT, typename SecondT>
        std::size_t operator()(const std::pair<FirstT, SecondT>& pair) const
        {
            return std::hash<FirstT>()(pair.first) ^ std::hash<SecondT>()(pair.second);
        }
    };
}