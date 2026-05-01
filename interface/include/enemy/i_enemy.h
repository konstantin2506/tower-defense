#pragma once


#include "ability/i_ability.h"
#include "attack/i_attackable.h"
#include "effects/i_effectable.h"
#include "entity_interafaces/i_healthable.h"
#include "entity_interafaces/i_movable.h"
#include "entity_interafaces/i_updatable.h"
#include "utility/entity_type.h"

namespace TowerDefence
{
    /**
     * @class IEnemy
     * @brief Интерфейс врага в игре Tavern Defense.
     *
     * Композитный интерфейс, объединяющий возможности передвижения, здоровья,
     * атаки, эффектов и обновления. Представляет базовый тип для всех врагов.
     */
    class IEnemy: public IMovable, public IHealthable, public IAttackable, public IIDObject, public IEffectable<IEnemy>, public IUpdatable<float> {
    public:
        ~IEnemy() override = default;

        /**
         * @brief Добавить способность врагу.
         * @param ability std::unique_ptr<IAbility> на способность.
         */
        virtual void addAbility(std::unique_ptr<IAbility> ability) = 0;

        /**
         * @brief Получить все способности врага.
         * @return std::vector<std::unique_ptr<IAbility>> способностей.
         */
        [[nodiscard]] virtual std::vector<std::unique_ptr<IAbility>> getAbilities() const = 0;

        /**
         * @brief Установить способности врага.
         * @param abilities std::vector<std::unique_ptr<IAbility>> способностей.
         */
        virtual void setAbilities(std::vector<std::unique_ptr<IAbility>> abilities) = 0;

        /**
         * @brief Получить тип врага.
         * @return const MovableType& тип врага.
         */
        [[nodiscard]] virtual const MovableType& getType() const = 0;

        /**
         * @brief Создать копию врага.
         * @return std::unique_ptr<IEnemy> на клонированного врага.
         */
        [[nodiscard]] virtual std::unique_ptr<IEnemy> clone() const = 0;

        /**
         * @brief Настроить врага из конфигурации.
         * @param config IConfigReader с параметрами врага.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Сохранить состояние врага в конфигурацию.
         * @param config IConfigReader для записи.
         */
        virtual void toConfig(IConfigReader& config) const = 0;
    };
}
