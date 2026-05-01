#pragma once

#include "utility/i_economy_service.h"
#include "hash_pair.h"
#include "utility/entity_type.h"

namespace TowerDefence
{
    /**
     * @class EconomyService
     * @brief Сервис управления экономикой игры.
     *
     * Финальная реализация IEconomyService, управляющая игровой валютой,
     * стоимостью улучшений, строительства, ремонта и наградами за убийства врагов.
     * Использует хеш-таблицы для быстрого доступа к стоимостям и наградам.
     */
    class EconomyService final : public IEconomyService {
        const Money startMoney_;           ///< Стартовое количество денег
        Money money_ = 0;                  ///< Текущий баланс игрока
        std::unordered_map<std::pair<std::string, Level>, Money, PairHasher> upgradeCosts_;  ///< Стоимости улучшений по типу и уровню
        std::unordered_map<std::string, Money> rewards_;                    ///< Награды за убийства врагов по типу

    public:
        /**
         * @brief Конструктор сервиса экономики.
         * @param cfg IConfigReader для загрузки начальных параметров экономики.
         * @details Загружает стартовые деньги, стоимости улучшений и награды из конфигурации.
         */
        explicit EconomyService(const IConfigReader& cfg);

        /**
         * @brief Проверить возможность улучшения здания.
         * @param buildingType Тип здания (строковый идентификатор).
         * @param level Уровень, на который нужно улучшить.
         * @return true если у игрока достаточно денег для улучшения, false в противном случае.
         */
        [[nodiscard]] bool canUpgrade(const std::string& buildingType, Level level) const override;

        /**
         * @brief Получить текущий баланс игрока.
         * @return Money текущее количество денег.
         */
        [[nodiscard]] Money balance() const override {return money_;}

        /**
         * @brief Наградить игрока за убийство врага.
         * @param enemyType Тип убитого врага.
         * @details Увеличивает баланс игрока на сумму награды за данный тип врага.
         */
        void rewardForKill(const std::string& enemyType) override;

        /**
         * @brief Попытаться улучшить здание.
         * @param buildingType UpgradableType улучшаемого здания.
         * @param level Уровень, на который нужно улучшить.
         * @return true если улучшение успешно (деньги списаны), false в противном случае.
         */
        bool tryUpgrade(const UpgradableType& buildingType, Level level) override;

        /**
         * @brief Попытаться построить здание.
         * @param buildingType UpgradableType строящегося здания.
         * @return true если строительство успешно (деньги списаны), false в противном случае.
         */
        bool tryBuild(const UpgradableType& buildingType) override;

        /**
         * @brief Попытаться отремонтировать здание.
         * @param buildingType UpgradableType ремонтируемого здания.
         * @param level Текущий уровень здания.
         * @param currentPercent Текущий процент прочности здания (0-1).
         * @return true если ремонт успешен (деньги списаны), false в противном случае.
         */
        bool tryRepair(const UpgradableType &buildingType, Level level, float currentPercent) override;
        /**
         * @brief Оплатить разрушение здания (взиммние средств).
         * @param buildingType UpgradableType разрушенного здания.
         * @return true если оплата успешна (деньги списаны), false в противном случае.
         */
        bool payDestruction(const UpgradableType &buildingType) override;

        /**
         * @brief Сбросить состояние экономики.
         * @details Устанавливает баланс в стартовое значение.
         */
        void reset() override;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader с сохраненным состоянием экономики.
         * @details Загружает только текущий баланс денег.
         */
        void setFromConfig(const IConfigReader &config) override
        {
            money_ = config.getSize_t("money");
        }

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи состояния.
         * @details Сохраняет только текущий баланс денег.
         */
        void toConfig(IConfigReader &config) const override
        {
            config.writeSize_t("money", money_);
        }
    };
}