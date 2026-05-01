#pragma once

#include "i_ability.h"
#include "config/i_config_reader.h"

namespace TowerDefence
{
    /**
     * @class IAbilityFactory
     * @brief Фабрика для создания и восстановления способностей (Ability).
     *
     * Абстрактная фабрика, предоставляющая интерфейс для создания новых экземпляров
     * способностей и их восстановления из конфигурации.
     */
    class IAbilityFactory {
    public:
        virtual ~IAbilityFactory() = default;

        /**
         * @brief Создает новый экземпляр способности.
         * @return std::unique_ptr<IAbility> на созданную способность.
         */
        [[nodiscard]] virtual std::unique_ptr<IAbility> create() const = 0;

        /**
         * @brief Восстанавливает способность из конфигурации.
         * @param cfg IConfigReader, содержащий данные для восстановления.
         * @return std::unique_ptr<IAbility> на восстановленную способность.
         */
        [[nodiscard]] virtual std::unique_ptr<IAbility> restore(const IConfigReader& cfg) const = 0;
    };
}
