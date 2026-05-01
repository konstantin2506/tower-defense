#pragma once
#include "wall_factory_manager.h"
#include "utility/i_economy_service.h"
#include "utility/i_grid_service.h"
#include "utility/i_id_service.h"
#include "wall/i_wall_repository.h"
#include "wall/i_wall_service.h"
#include "world_map/i_world_map.h"


namespace TowerDefence
{
    /**
     * @brief Основной сервис для управления стенами в игре.
     *
     * Реализует интерфейс IWallService.
     * Отвечает за создание, улучшение, ремонт, разрушение стен,
     * применение эффектов и взаимодействие с другими сервисами.
     */
    class WallService final: public IWallService {
        WallFactoryManager wallFactoryManager_; ///< Менеджер фабрик стен.
        std::unique_ptr<IGridService> gridService_; ///< Сервис для пространственных запросов.
        IWallRepository& wallRepository_; ///< Репозиторий стен.
        IIDService& idService_; ///< Сервис генерации ID.
        IEconomyService& economyService_; ///< Сервис экономики.
        IWorldMap& worldMap_; ///< Игровая карта.
        std::vector<ID> destroyed_; ///< Вектор ID уничтоженных стен.
#ifdef PARALLEL
        std::mutex mutex_; ///< Мьютекс для синхронизации в многопоточном режиме.
#endif

    public:
        /**
         * @brief Конструктор сервиса стен.
         * @param idService Сервис генерации ID.
         * @param economyService Сервис экономики.
         * @param repository Репозиторий стен.
         * @param world Игровая карта.
         * @param gridService Сервис пространственных запросов.
         */
        WallService(IIDService& idService, IEconomyService& economyService, IWallRepository& repository, IWorldMap& world, std::unique_ptr<IGridService> gridService) :
        gridService_(std::move(gridService)), wallRepository_(repository), idService_(idService), economyService_(economyService), worldMap_(world){}

        /**
         * @brief Инициализирует сервис и фабрику стен из конфигурации.
         * @param cfg Конфигурация для инициализации.
         */
        void init(const IConfigReader& cfg) override;

        /**
         * @brief Улучшает стену на один уровень.
         * @param wall ID стены для улучшения.
         * @return true если улучшение успешно, false если невозможно улучшить.
         */
        [[nodiscard("check if not upgraded")]] bool upgradeWall(ID wall) override;

        /**
         * @brief Создает стену заданного типа в указанной позиции.
         * @param wallName Имя типа стены.
         * @param position Позиция для создания.
         * @return true если стена успешно создана, false если невозможно создать.
         */
        [[nodiscard("check if not created")]] bool createWall(const std::string& wallName, Vector2<size_t> position) override;

        /**
         * @brief Ремонтирует стену.
         * @param wall ID стены для ремонта.
         * @return true если ремонт успешен, false если невозможно отремонтировать.
         */
        [[nodiscard("check if not repaired")]] bool repairWall(ID wall) override;

        /**
         * @brief Уничтожает стену в указанной позиции.
         * @param position Позиция стены.
         * @return true если стена была успешно уничтожена.
         */
        bool destroyWall(Vector2<size_t> position) override;

        /**
         * @brief Проверяет, уничтожена ли стена.
         * @param id ID стены.
         * @return true если стена уничтожена.
         */
        bool isKilled(ID id) const override;

        /**
         * @brief Наносит урон стене.
         * @param id ID стены.
         * @param damage Количество урона.
         */
        void dealDamage(ID id, float damage) override;

        /**
         * @brief Накладывает эффект на стену.
         * @param id ID стены.
         * @param effect Эффект для применения.
         */
        void addEffect(ID id, std::unique_ptr<IEffect<IWall>> effect) override{ wallRepository_.getWall(id).applyEffect(std::move(effect));}

        /**
         * @brief Возвращает ID всех стен в заданном радиусе от точки.
         * @param point Центральная точка.
         * @param radius Радиус поиска.
         * @return Вектор ID стен в радиусе.
         */
        [[nodiscard]] std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const override {return gridService_->getInRadius(point, radius);}

        /**
         * @brief Возвращает позицию стены.
         * @param id ID стены.
         * @return Позиция стены.
         */
        [[nodiscard]] Vector2<size_t> getPosition(ID id) const override {return wallRepository_.getWall(id).getPosition();}

        /**
         * @brief Основной метод обновления состояния всех стен за кадр.
         * @param dt Время с предыдущего кадра.
         */
        void update(float dt) override;

        /**
         * @brief Собирает информацию об уничтоженных стенах за кадр и очищает список.
         * @return Вектор пар {ID стены, имя её типа}.
         */
        std::vector<std::pair<ID, std::string>> collectDestroyed() override;

        /**
         * @brief Выполняет callback-функцию для каждой стены (только для чтения).
         * @param func Функция, принимающая константную ссылку на IWall.
         */
        void forEach(std::function<void(const IWall &)> func) const override {wallRepository_.forEach(func);}

        /**
         * @brief Сбрасывает состояние сервиса (очищает репозиторий и списки).
         */
        void reset() override;

        /**
         * @brief Сохраняет состояние всех стен в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) override;

        /**
         * @brief Загружает состояние всех стен из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void load(const IConfigReader &config) override;
    };
}