#pragma once
#include "enemy_factory_manager.h"
#include "enemy/i_enemy_service.h"
#include "pathfinding/i_pathfinding_service.h"
#include "utility/i_economy_service.h"
#include "utility/i_grid_service.h"
#include "utility/i_id_service.h"

namespace TowerDefence
{
    /**
     * @brief Основной сервис для управления всеми врагами в игре.
     *
     * Реализует интерфейс IEnemyService.
     * Отвечает за создание, обновление, уничтожение врагов,
     * применение к ним урона/лечения/эффектов и взаимодействие с другими сервисами.
     */
    class EnemyService final: public IEnemyService {
        const IPathfindingService& pathfindingService_; ///< Сервис для расчета путей движения.
        IEconomyService& economyService_;               ///< Сервис экономики (для начисления награды за убийство).
        IIDService& idService_;                         ///< Сервис для генерации уникальных ID.
        IEnemyRepository& enemyRepository_;             ///< Репозиторий для хранения врагов.
#ifdef PARALLEL
        std::mutex mux_; ///< Мьютекс для синхронизации в многопоточном режиме.
#endif
        std::vector<ID> destroyed_; ///< Вектор ID врагов, уничтоженных за текущий кадр.
        EnemyFactoryManager enemyFactoryManager_; ///< Менеджер фабрик для создания врагов.
        std::unique_ptr<IGridService> gridService_; ///< Сервис для пространственных запросов (поиск ближайших врагов).

    public:
        /**
         * @brief Конструктор сервиса врагов.
         * @param pathfindingService Сервис поиска пути.
         * @param attackManager Менеджер фабрик стратегий атаки.
         * @param abilityManager Менеджер фабрик способностей.
         * @param repo Репозиторий врагов.
         * @param idService Сервис генерации ID.
         * @param economyService Сервис экономики.
         * @param gridService unique_ptr на сервис пространственных запросов.
         */
        EnemyService(IPathfindingService& pathfindingService, const AttackStrategyFactoryManager& attackManager,
            const AbilityFactoryManager& abilityManager, IEnemyRepository& repo,
            IIDService& idService, IEconomyService& economyService, std::unique_ptr<IGridService> gridService);

        /**
         * @brief Создает врага заданного типа в указанной позиции.
         * @param enemyName Имя типа врага.
         * @param position Позиция для создания.
         */
        void createEnemy(const std::string& enemyName, Vector2<size_t> position) override;

        /**
         * @brief Восстанавливает врага из конфигурации (например, при загрузке игры).
         * @param cfg Конфигурация с данными врага.
         */
        void restoreEnemy(const IConfigReader &cfg) override;

        /**
         * @brief Убивает врага с заданным ID.
         * @details Удаляет врага из репозитория и добавляет его ID в список уничтоженных.
         * @param id ID врага.
         */
        void killEnemy(ID id) override;

        /**
         * @brief Наносит урон врагу.
         * @param id ID врага.
         * @param damage Количество урона.
         */
        void damageEnemy(ID id, float damage) override;

        /**
         * @brief Лечит врага.
         * @param id ID врага.
         * @param heal Количество здоровья.
         */
        void healEnemy(ID id, float heal) override;

        /**
         * @brief Возвращает позицию врага.
         * @param id ID врага.
         * @return Позиция врага.
         */
        [[nodiscard]] Vector2<size_t> getPosition(ID id) const override;

        /**
         * @brief Возвращает ID всех врагов в заданном радиусе от точки.
         * @param point Центральная точка.
         * @param radius Радиус поиска.
         * @return Вектор ID врагов, находящихся в радиусе.
         */
        [[nodiscard]] std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const override;

        /**
         * @brief Накладывает эффект на врага.
         * @param id ID врага.
         * @param effect unique_ptr на эффект.
         */
        void addEffect(ID id, std::unique_ptr<IEffect<IEnemy>> effect) override;

        /**
         * @brief Выполняет callback-функцию для каждого врага (только для чтения).
         * @param callback Функция, принимающая константную ссылку на IEnemy.
         */
        void forEach(std::function<void(const IEnemy &)>) const override;

        /**
         * @brief Инициализирует сервис и фабрику врагов из конфигурации.
         * @param config Конфигурация для инициализации.
         */
        void init(const IConfigReader &config) override;

        /**
         * @brief Собирает информацию об уничтоженных врагах за кадр и очищает список.
         * @return Вектор пар {ID врага, имя его типа}.
         */
        std::vector<std::pair<ID, std::string>> collectDestroyed() override;

        /**
         * @brief Основной метод обновления состояния всех врагов за кадр.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         */
        void update(float dt) override;

        /**
         * @brief Сбрасывает состояние сервиса (очищает репозиторий и списки).
         */
        void reset() override;

        /**
         * @brief Сохраняет состояние всех врагов в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) override;

        /**
         * @brief Загружает состояние всех врагов из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void load(const IConfigReader &config) override;

    private:
        /**
         * @brief Внутренний метод для параллельного обновления врагов (используется, если определен PARALLEL).
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         */
        void updateParallel(float dt);


    };

}