#pragma once

#include "i_tower.h"

#include <memory>

namespace TowerDefence
{
    /**
     * @class ITowerFactory
     * @brief Фабрика для создания и восстановления башен.
     *
     * Абстрактная фабрика, предоставляющая интерфейс для создания
     * новых экземпляров башен и их восстановления из конфигурации.
     */
    class ITowerFactory {
    public:
        virtual ~ITowerFactory() = default;

        /**
         * @brief Создать новую башню.
         * @param position Vector2<size_t> позиция башни.
         * @param id ID для новой башни.
         * @return std::unique_ptr<ITower> на созданную башню.
         */
        virtual std::unique_ptr<ITower> create(Vector2<size_t> position, ID id) = 0;

        /**
         * @brief Восстановить башню из конфигурации.
         * @param cfg IConfigReader с данными для восстановления.
         * @return std::unique_ptr<ITower> на восстановленную башню.
         */
        virtual std::unique_ptr<ITower> restore(const IConfigReader& cfg) = 0;
    };
}
