#pragma once
#include "enemy/i_enemy_factory.h"
#include "utility/entity_type.h"

namespace TowerDefence
{
    /**
     * @brief Фабрика для создания врагов.
     *
     * Реализует интерфейс IEnemyFactory.
     * Использует паттерн "Прототип", храня донорский объект (donor_)
     * для клонирования при создании новых экземпляров.
     */
    class EnemyFactory final : public IEnemyFactory {
        std::unique_ptr<IEnemy> donor_; ///< Прототип (донор) врага, используемый для клонирования.

    public:
        /**
         * @brief Конструктор фабрики.
         * @param type Тип создаваемых врагов.
         * @param attackStrategy Стратегия атаки для создаваемых врагов.
         * @param abilities Способности для создаваемых врагов.
         * @param config Конфигурация для инициализации донорского объекта.
         */
        EnemyFactory(const MovableType& type, std::unique_ptr<IAttackStrategy> attackStrategy, std::vector<std::unique_ptr<IAbility>> abilities, const IConfigReader& config);

        /**
         * @brief Создает нового врага в указанной позиции с заданным ID.
         * @param position Позиция для размещения врага.
         * @param id Уникальный идентификатор для нового врага.
         * @return unique_ptr на созданного врага.
         */
        [[nodiscard]] std::unique_ptr<IEnemy> create(Vector2<size_t> position, ID id) const override;

        /**
         * @brief Восстанавливает врага из конфигурации.
         * @param cfg Объект для чтения конфигурации с сохраненным состоянием врага.
         * @return unique_ptr на восстановленного врага.
         */
        [[nodiscard]] std::unique_ptr<IEnemy> restore(const IConfigReader &cfg) const override;
    };
}