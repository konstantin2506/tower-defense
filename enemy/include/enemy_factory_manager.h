#pragma once
#include "ability_factory_manager.h"
#include "attack_strategy_factory_manager.h"
#include "enemy_factory.h"
#include "enemy/i_enemy.h"
#include "enemy/i_enemy_factory.h"
#include "enemy/i_enemy_repository.h"
#include "utility/factory_manager.h"


namespace TowerDefence
{
    /**
     * @brief Менеджер фабрик врагов.
     *
     * Наследуется от шаблонного FactoryManager для управления фабриками,
     * создающими объекты типа IEnemy.
     * Использует менеджеры фабрик атак и способностей для их создания.
     */
    class EnemyFactoryManager final: public FactoryManager<IEnemy, IEnemyFactory>{
        const AttackStrategyFactoryManager& attackManager_; ///< Ссылка на менеджер фабрик стратегий атаки.
        const AbilityFactoryManager& abilityManager_;       ///< Ссылка на менеджер фабрик способностей.

    public:
        /**
         * @brief Конструктор менеджера фабрик врагов.
         * @param attackManager Менеджер фабрик стратегий атаки.
         * @param abilityManager Менеджер фабрик способностей.
         */
        explicit EnemyFactoryManager(const AttackStrategyFactoryManager& attackManager, const AbilityFactoryManager& abilityManager): attackManager_(attackManager), abilityManager_(abilityManager) {}

        /**
         * @brief Инициализирует менеджер, создавая фабрики из конфигурации.
         * @param config Конфигурация, содержащая описание всех типов врагов.
         * @param repo Репозиторий врагов для получения ссылок на типы (MovableType).
         */
        void init(const IConfigReader& config, const IEnemyRepository& repo);

        /**
         * @brief Добавляет новую фабрику врагов по имени.
         * @param name Имя типа врага (ключ в конфигурации и репозитории).
         * @param config Конфигурация для данного типа врага.
         * @param repo Репозиторий врагов для получения ссылки на тип.
         */
        void addFactory(const std::string& name, const IConfigReader& config, const IEnemyRepository& repo)
        {
            std::cout << "\nadding factory with name: " << name << std::endl;
            std::vector<std::unique_ptr<IAbility>> abilities;
            auto abilityConfigs = config.getItems("abilities");
            std::cout << "abilities: " << std::endl;
            // Создание способностей через AbilityFactoryManager
            for (const auto& [abilityName , abilityConfig] : abilityConfigs) {
                abilities.emplace_back(abilityManager_.restore(abilityName, *abilityConfig));
                std::cout << "\t+" << abilityName << std::endl;
            }
            // Регистрация фабрики в базовом классе FactoryManager
            addIDObjectFactory<EnemyFactory>(name, repo.getTypeRef(name) , attackManager_.create(config.getString("attack_strategy_name")), std::move(abilities), config);
        }
    };

}