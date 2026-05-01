#pragma once

#include "i_enemy.h"

#include <memory>


namespace TowerDefence
{
    /**
     * @class IEnemyFactory
     * @brief Фабрика для создания и восстановления врагов.
     *
     * Абстрактная фабрика, предоставляющая интерфейс для создания
     * новых экземпляров врагов и их восстановления из конфигурации.
     */
    class IEnemyFactory {
    public:
        virtual ~IEnemyFactory() = default;

        /**
         * @brief Создает нового врага.
         * @param position Vector2<size_t> позиция создания.
         * @param id ID для нового врага.
         * @return std::unique_ptr<IEnemy> на созданного врага.
         */
        [[nodiscard]] virtual std::unique_ptr<IEnemy> create(Vector2<size_t> position, ID id) const = 0;

        /**
         * @brief Восстанавливает врага из конфигурации.
         * @param cfg IConfigReader с данными для восстановления.
         * @return std::unique_ptr<IEnemy> на восстановленного врага.
         */
        [[nodiscard]] virtual std::unique_ptr<IEnemy> restore(const IConfigReader& cfg) const = 0;
    };
}
