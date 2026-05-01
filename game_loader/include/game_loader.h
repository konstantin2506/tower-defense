#pragma once

#include "castle/i_castle_repository.h"
#include "castle/i_castle_service.h"
#include "enemy/i_enemy_repository.h"
#include "enemy/i_enemy_service.h"
#include "pathfinding/i_pathfinding_service.h"
#include "tower/i_tower_repository.h"
#include "tower/i_tower_service.h"
#include "utility/i_economy_service.h"
#include "utility/i_id_service.h"
#include "wall/i_wall_repository.h"
#include "wall/i_wall_service.h"
#include "world_map/i_world_map.h"

#include "ability_factory_manager.h"
#include "attack_strategy_factory_manager.h"
#include "view.h"
#include "presenter.h"

namespace TowerDefence
{
    /**
     * @brief Класс для загрузки и инициализации всех компонентов игры.
     *
     * Отвечает за создание и настройку всех сервисов, репозиториев,
     * фабрик и других игровых систем на основе конфигурации.
     * Управляет жизненным циклом основных игровых объектов.
     */
    class GameLoader {
        std::unique_ptr<IEnemyService> enemyService_;      ///< Сервис управления врагами.
        std::unique_ptr<IWallService> wallService_;        ///< Сервис управления стенами.
        std::unique_ptr<ITowerService> towerService_;      ///< Сервис управления башнями.
        std::unique_ptr<ICastleService> castleService_;    ///< Сервис управления замком.

        std::unique_ptr<IEconomyService> economyService_;  ///< Сервис экономики.
        std::unique_ptr<IIDService> idService_;            ///< Сервис генерации уникальных идентификаторов.
        std::unique_ptr<IWorldMap> worldMap_;              ///< Игровая карта.
        std::unique_ptr<IPathfindingService> pathfindingService_; ///< Сервис поиска пути.

        std::unique_ptr<IEnemyRepository> enemyRepository_; ///< Репозиторий врагов.
        std::unique_ptr<IWallRepository> wallRepository_;   ///< Репозиторий стен.
        std::unique_ptr<ITowerRepository> towerRepository_; ///< Репозиторий башен.
        std::unique_ptr<ICastleRepository> castleRepository_; ///< Репозиторий замка.

        std::unique_ptr<AttackStrategyFactoryManager> attackManager_; ///< Менеджер фабрик стратегий атаки.
        std::unique_ptr<AbilityFactoryManager> abilityManager_; ///< Менеджер фабрик способностей.

        std::unique_ptr<View> view_; ///< Представление (визуальная часть).
        std::unique_ptr<Model> model_; ///< Модель (игровая логика).

        std::unordered_map<std::string, std::unique_ptr<IConfigReader>> allConfigs_; ///< Все конфигурации игры.

    public:
        /**
         * @brief Конструктор по умолчанию.
         */
        GameLoader() = default;

        /**
         * @brief Конструктор с загрузкой конфигурации.
         * @param configOfConfigs Конфигурация, содержащая пути ко всем остальным конфигурационным файлам.
         */
        explicit GameLoader(const IConfigReader& configOfConfigs);

        // Правила работы с перемещением и копированием
        GameLoader(GameLoader&&) = default; ///< Конструктор перемещения.
        GameLoader& operator=(GameLoader&&) = default; ///< Оператор перемещения.
        GameLoader(const GameLoader&) = delete; ///< Конструктор копирования запрещен.
        GameLoader& operator=(const GameLoader&) = delete; ///< Оператор присваивания копированием запрещен.

        /**
         * @brief Возвращает ссылку на сервис врагов.
         */
        [[nodiscard]] IEnemyService& getEnemyService() const {return *enemyService_;}

        /**
         * @brief Возвращает ссылку на сервис стен.
         */
        [[nodiscard]] IWallService& getWallService() const {return *wallService_;}

        /**
         * @brief Возвращает ссылку на сервис башен.
         */
        [[nodiscard]] ITowerService& getTowerService() const {return *towerService_;}

        /**
         * @brief Возвращает ссылку на сервис замка.
         */
        [[nodiscard]] ICastleService& getCastleService() const {return *castleService_;}

        /**
         * @brief Возвращает ссылку на сервис экономики.
         */
        [[nodiscard]] IEconomyService& getEconomyService() const {return *economyService_;}

        /**
         * @brief Возвращает ссылку на сервис идентификаторов.
         */
        [[nodiscard]] IIDService& getIdService() const {return *idService_;}

        /**
         * @brief Возвращает ссылку на игровую карту.
         */
        [[nodiscard]] IWorldMap& getWorldMap() const {return *worldMap_;}

        /**
         * @brief Возвращает ссылку на сервис поиска пути.
         */
        [[nodiscard]] IPathfindingService& getPathfindingService() const {return *pathfindingService_;}

        /**
         * @brief Возвращает ссылку на игровую карту (дублирующий метод для обратной совместимости).
         */
        [[nodiscard]] IWorldMap& getWorldMapRepository() const {return *worldMap_;}

        /**
         * @brief Возвращает ссылку на репозиторий башен.
         */
        [[nodiscard]] ITowerRepository& getTowerRepository() const {return *towerRepository_;}

        /**
         * @brief Возвращает ссылку на репозиторий замка.
         */
        [[nodiscard]] ICastleRepository& getCastleRepository() const {return *castleRepository_;}

        /**
         * @brief Возвращает ссылку на репозиторий врагов.
         */
        [[nodiscard]] IEnemyRepository& getEnemyRepository() const {return *enemyRepository_;}

        /**
         * @brief Возвращает ссылку на репозиторий стен.
         */
        [[nodiscard]] IWallRepository& getWallRepository() const {return *wallRepository_;}

        /**
         * @brief Возвращает ссылку на представление (View).
         */
        [[nodiscard]] View& getView() const {return *view_;}

        /**
         * @brief Возвращает ссылку на модель (Model).
         */
        [[nodiscard]] Model& getModel() const {return *model_;}

    private:
        /// Тип для хранения всех конфигураций.
        using allConfMap = std::unordered_map<std::string, std::unique_ptr<IConfigReader>>;

        /**
         * @brief Основной метод инициализации всех компонентов игры.
         * @param allConfigs Карта со всеми загруженными конфигурациями.
         */
        void init(const allConfMap& allConfigs);

        /**
         * @brief Инициализирует менеджер фабрик способностей.
         * @param config Конфигурация для способностей.
         */
        void initAbilities(const IConfigReader& config);

        /**
         * @brief Инициализирует менеджер фабрик стратегий атаки.
         * @param config Конфигурация для стратегий атаки.
         */
        void initAttacks(const IConfigReader& config);
    };

}