#pragma once
#include "i_attack_strategy.h"

namespace TowerDefence
{
    /**
     * @class IAttackable
     * @brief Интерфейс для объектов, способных атаковать.
     *
     * Определяет базовые операции для всех атакующих объектов в игре.
     * Позволяет устанавливать и получать стратегию атаки, а также
     * выполнять атаку каждый игровой кадр.
     */
    class IAttackable {
    public:
        virtual ~IAttackable() = default;

        /**
         * @brief Выполнить атаку.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         * @return true если атака была выполнена, false в противном случае.
         */
        virtual bool attack(float dt) = 0;

        /**
         * @brief Установить стратегию атаки.
         * @param attackStrategy std::unique_ptr<IAttackStrategy> новая стратегия.
         */
        virtual void setAttackStrategy(std::unique_ptr<IAttackStrategy> attackStrategy) = 0;

        /**
         * @brief Получить текущую стратегию атаки.
         * @return std::unique_ptr<IAttackStrategy> текущая стратегия атаки.
         * @warning Важно не потерять указатель при использовании этого метода!
         */
        [[nodiscard("this is very important not to lose it!!!!")]]virtual std::unique_ptr<IAttackStrategy> getAttackStrategy() = 0;
    };
}
