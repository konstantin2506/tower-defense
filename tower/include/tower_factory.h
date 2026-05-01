#pragma once

#include <memory>

#include "utility/entity_type.h"
#include "tower/i_tower_factory.h"

namespace TowerDefence
{
    /**
     * @brief Фабрика для создания башен.
     *
     * Реализует интерфейс ITowerFactory.
     * Использует прототип (donor) для создания новых экземпляров.
     */
    class TowerFactory final: public ITowerFactory {
        std::unique_ptr<ITower> donor_; ///< Прототип башни для клонирования.

    public:
        /**
         * @brief Конструктор фабрики.
         * @param type Тип создаваемых башен.
         * @param strategy Стратегия атаки для башен.
         */
        TowerFactory(const UpgradableType& type , std::unique_ptr<IAttackStrategy> strategy);

        /**
         * @brief Создает новую башню в указанной позиции.
         * @param position Позиция для размещения башни.
         * @param id Уникальный идентификатор для новой башни.
         * @return unique_ptr на созданную башню.
         */
        std::unique_ptr<ITower> create(Vector2<size_t> position, ID id) override;

        /**
         * @brief Восстанавливает башню из конфигурации.
         * @param cfg Конфигурация с сохраненным состоянием башни.
         * @return unique_ptr на восстановленную башню.
         */
        std::unique_ptr<ITower> restore(const IConfigReader& cfg) override;

    };
}