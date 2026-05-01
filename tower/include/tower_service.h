#pragma once


#include "tower_factory_manager.h"
#include "utility/i_economy_service.h"
#include "utility/i_grid_service.h"
#include "utility/i_id_service.h"
#include "world_map/i_world_map.h"


namespace TowerDefence
{
    /**
     * @brief Основной сервис для управления башнями в игре.
     *
     * Реализует интерфейс ITowerService.
     * Отвечает за создание, улучшение, разрушение башен,
     * применение эффектов и взаимодействие с другими сервисами.
     */
    class TowerService final: public ITowerService {
        ITowerRepository& towerRepository_; ///< Репозиторий башен.
        IIDService& idService_; ///< Сервис генерации ID.
        IEconomyService& economyService_; ///< Сервис экономики.
        IWorldMap& worldMap_; ///< Игровая карта.
        TowerFactoryManager towerFactoryManager_; ///< Менеджер фабрик башен.
        std::unique_ptr<IGridService> gridService_; ///< Сервис для пространственных запросов.
        std::vector<ID> destroyed_; ///< Вектор ID уничтоженных башен (кеш).
#ifdef PARALLEL
        std::mutex mux_; ///< Мьютекс для синхронизации в многопоточном режиме.
#endif

    public:
        /**
         * @brief Конструктор сервиса башен.
         * @param towerRepository Репозиторий башен.
         * @param idService Сервис генерации ID.
         * @param economyService Сервис экономики.
         * @param worldMap Игровая карта.
         * @param asfm Менеджер фабрик стратегий атаки.
         * @param gridService Сервис пространственных запросов.
         */
        TowerService(ITowerRepository& towerRepository, IIDService& idService,
            IEconomyService& economyService, IWorldMap& worldMap,
            const AttackStrategyFactoryManager& asfm, std::unique_ptr<IGridService> gridService);

        /**
         * @brief Инициализирует сервис и фабрику башен из конфигурации.
         * @param config Конфигурация для инициализации.
         */
        void init(const IConfigReader& config) override;

        /**
         * @brief Создает башню заданного типа в указанной позиции.
         * @param towerName Имя типа башни.
         * @param position Позиция для создания.
         * @return true если башня успешно создана, false если невозможно создать.
         */
        [[nodiscard("check if not created")]] bool createTower(const std::string &towerName, Vector2<size_t> position) override;

        /**
         * @brief Улучшает башню на один уровень.
         * @param tower ID башни для улучшения.
         * @return true если улучшение успешно, false если невозможно улучшить.
         */
        [[nodiscard("check if not upgraded")]] bool upgradeTower(ID tower) const override;

        /**
         * @brief Возвращает позицию башни.
         * @param id ID башни.
         * @return Позиция башни.
         */
        [[nodiscard]] Vector2<size_t> getPosition(ID id) const override {return towerRepository_.getTower(id).getPosition();}

        /**
         * @brief Возвращает ID всех башен в заданном радиусе от точки.
         * @param tower Центральная точка.
         * @param radius Радиус поиска.
         * @return Вектор ID башен в радиусе.
         */
        [[nodiscard]] std::vector<ID> getInRadius(Vector2<size_t> tower, size_t radius) const override {return gridService_->getInRadius(tower, radius);}

        /**
         * @brief Накладывает эффект на башню.
         * @param id ID башни.
         * @param effect Эффект для применения.
         */
        void addEffect(ID id, std::unique_ptr<IEffect<ITower>> effect) override {towerRepository_.getTower(id).applyEffect(std::move(effect));}

        /**
         * @brief Выполняет callback-функцию для каждой башни (только для чтения).
         * @param callback Функция, принимающая константную ссылку на ITower.
         */
        void forEach(const std::function<void(const ITower&)> callback) const override {towerRepository_.forEach(callback);}

        /**
         * @brief Основной метод обновления состояния всех башен за кадр.
         * @param dt Время с предыдущего кадра.
         */
        void update(float dt) override;

        /**
         * @brief Уничтожает башню в указанной позиции.
         * @param position Позиция башни.
         * @return true если башня была успешно уничтожена.
         */
        bool destroyTower(Vector2<size_t> position) override;

        /**
         * @brief Собирает информацию об уничтоженных башнях за кадр и очищает список.
         * @return Вектор пар {ID башни, имя её типа}.
         */
        std::vector<std::pair<ID, std::string>>  collectDestroyed() override;

        /**
         * @brief Сбрасывает состояние сервиса (очищает репозиторий и списки).
         */
        void reset() override;

        /**
         * @brief Сохраняет состояние всех башен в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) override;

        /**
         * @brief Загружает состояние всех башен из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void load(const IConfigReader &config) override;
    };
}