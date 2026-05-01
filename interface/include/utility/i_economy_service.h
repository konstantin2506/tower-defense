#pragma once

#include "entity_type.h"
#include "type_names.h"

#include <string>

namespace TowerDefence
{
    class UpgradableType;

    /**
     * @class IEconomyService
     * @brief Сервис экономики игры.
     *
     * Управляет игровой экономикой: покупкой, улучшением, ремонтом зданий,
     * начислением наград за убийство врагов и отслеживанием баланса.
     */
    class IEconomyService {
    public:
        virtual ~IEconomyService() = default;

        /**
         * @brief Проверить возможность улучшения здания.
         * @param buildingType Тип здания.
         * @param level Уровень, на который нужно улучшить.
         * @return true если улучшение возможно, false в противном случае.
         */
        [[nodiscard]] virtual bool canUpgrade(const std::string& buildingType, Level level) const = 0;

        /**
         * @brief Попытаться улучшить здание.
         * @param buildingType UpgradableType здания.
         * @param level Уровень, на который нужно улучшить.
         * @return true если улучшение успешно, false в противном случае.
         */
        virtual bool tryUpgrade(const UpgradableType& buildingType, Level level) = 0;

        /**
         * @brief Попытаться построить здание.
         * @param buildingType UpgradableType здания.
         * @return true если строительство успешно, false в противном случае.
         */
        virtual bool tryBuild(const UpgradableType& buildingType) = 0;

        /**
         * @brief Попытаться отремонтировать здание.
         * @param buildingType UpgradableType здания.
         * @param level Текущий уровень здания.
         * @param currentPercent Текущий процент прочности здания.
         * @return true если ремонт успешен, false в противном случае.
         */
        virtual bool tryRepair(const UpgradableType& buildingType, Level level, float currentPercent) = 0;

        /**
         * @brief Наградить игрока за убийство врага.
         * @param enemyType Тип убитого врага.
         */
        virtual void rewardForKill(const std::string& enemyType) = 0;

        /**
         * @brief Оплатить разрушение здания (возмещение средств).
         * @param buildingType UpgradableType разрушенного здания.
         * @return true если оплата успешна, false в противном случае.
         */
        virtual bool payDestruction(const UpgradableType& buildingType) = 0;

        /**
         * @brief Получить текущий баланс игрока.
         * @return Money текущий баланс.
         */
        [[nodiscard]] virtual Money balance() const = 0;

        /**
         * @brief Сбросить состояние экономики.
         */
        virtual void reset() = 0;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader для чтения состояния.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader& config) const = 0;
    };
}
