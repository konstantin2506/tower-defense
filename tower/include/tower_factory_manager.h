#pragma once

#include "attack_strategy_factory_manager.h"
#include "tower_factory.h"
#include "tower/i_tower_repository.h"
#include "utility/factory_manager.h"

namespace TowerDefence
{
    /**
     * @brief Менеджер фабрик башен.
     *
     * Наследуется от шаблонного FactoryManager для управления фабриками,
     * создающими объекты типа ITower.
     */
    class TowerFactoryManager : public FactoryManager<ITower, ITowerFactory>{
        const AttackStrategyFactoryManager& attackManager_; ///< Ссылка на менеджер фабрик стратегий атаки.

    public:
        /**
         * @brief Конструктор менеджера фабрик башен.
         * @param asfm Менеджер фабрик стратегий атаки.
         */
        explicit TowerFactoryManager(const AttackStrategyFactoryManager& asfm) : FactoryManager(), attackManager_(asfm) {}

        /**
         * @brief Инициализирует менеджер, создавая фабрики из конфигурации.
         * @param config Конфигурация с описанием всех типов башен.
         * @param repo Репозиторий башен для получения ссылок на типы.
         */
        void init(const IConfigReader& config, const ITowerRepository& repo);

    private:
        /**
         * @brief Добавляет новую фабрику башен по имени.
         * @param name Имя типа башни.
         * @param config Конфигурация для данного типа башни.
         * @param repo Репозиторий башен для получения ссылки на тип.
         */
        void addFactory(const std::string& name, const IConfigReader& config, const ITowerRepository& repo)
        {
            addIDObjectFactory<TowerFactory>(name, repo.getTypeRef(name) , attackManager_.create(config.getString("attack_strategy")));
        }
    };
}