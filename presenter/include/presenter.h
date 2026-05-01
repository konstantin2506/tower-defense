#pragma once
#include <unordered_map>
#include <SFML/Window/Event.hpp>

#include "castle/i_castle_repository.h"
#include "castle/i_castle_service.h"
#include "enemy/i_enemy_repository.h"
#include "enemy/i_enemy_service.h"
#include "pathfinding/i_pathfinding_service.h"
#include "tower/i_tower_repository.h"
#include "tower/i_tower_service.h"
#include "utility/i_economy_service.h"
#include "wall/i_wall_repository.h"
#include "wall/i_wall_service.h"
#include "world_map/i_world_map.h"
#include "view.h"
#include "utility/i_id_service.h"


namespace TowerDefence
{
    /**
     * @brief Модель игры, содержащая все сервисы и репозитории.
     *
     * Агрегирует все игровые сервисы и управляет сохранением/загрузкой состояния.
     */
    class Model {
        IConfigReader& enemySave_;   ///< Конфигурация для сохранения врагов.
        IConfigReader& wallSave_;    ///< Конфигурация для сохранения стен.
        IConfigReader& towerSave_;   ///< Конфигурация для сохранения башен.
        IConfigReader& castleSave_;  ///< Конфигурация для сохранения замка.
        IConfigReader& economySave_; ///< Конфигурация для сохранения экономики.
        IConfigReader& idSave_;      ///< Конфигурация для сохранения ID сервиса.

    public:
        IEnemyService& enemies_;     ///< Сервис управления врагами.
        IWallService& walls_;        ///< Сервис управления стенами.
        ITowerService& towers_;      ///< Сервис управления башнями.
        ICastleService& castle_;     ///< Сервис управления замком.

        const IEnemyRepository& enemiesRepository_; ///< Репозиторий врагов.
        const IWallRepository& wallRepository_;     ///< Репозиторий стен.
        const ITowerRepository& towerRepository_;   ///< Репозиторий башен.
        const ICastleRepository& castleRepository_; ///< Репозиторий замка.

        const IWorldMap& world_;          ///< Игровая карта.
        IIDService& idService_;           ///< Сервис генерации ID.
        IEconomyService& economyService_; ///< Сервис экономики.
        IPathfindingService& pathfindingService_; ///< Сервис поиска пути.

        /**
         * @brief Конструктор модели.
         * @param e Конфигурация для врагов.
         * @param wSave Конфигурация для стен.
         * @param t Конфигурация для башен.
         * @param c Конфигурация для замка.
         * @param economySave Конфигурация для экономики.
         * @param idSave Конфигурация для ID сервиса.
         * @param es Сервис врагов.
         * @param ws Сервис стен.
         * @param ts Сервис башен.
         * @param cs Сервис замка.
         * @param w Игровая карта.
         * @param enemiesRepository_ Репозиторий врагов.
         * @param wallRepository_ Репозиторий стен.
         * @param towerRepository_ Репозиторий башен.
         * @param castleRepository_ Репозиторий замка.
         * @param economyService_ Сервис экономики.
         * @param pathfinding Сервис поиска пути.
         * @param idService Сервис генерации ID.
         */
        Model(IConfigReader& e, IConfigReader& wSave, IConfigReader& t, IConfigReader& c, IConfigReader& economySave, IConfigReader& idSave, IEnemyService& es, IWallService& ws, ITowerService& ts, ICastleService& cs, const IWorldMap& w, const IEnemyRepository& enemiesRepository_, const IWallRepository& wallRepository_,
        const ITowerRepository& towerRepository_, const ICastleRepository& castleRepository_, IEconomyService& economyService_, IPathfindingService& pathfinding, IIDService& idService);

        /**
         * @brief Сбрасывает состояние модели (новый уровень).
         */
        void reset();

        /**
         * @brief Сохраняет текущее состояние всех компонентов.
         */
        void save();

        /**
         * @brief Загружает состояние всех компонентов.
         */
        void load();

        /**
         * @brief Записывает конфигурацию модели.
         * @param config Объект для записи конфигурации.
         */
        void writeConfig(IConfigReader &config);
    };

    /**
     * @brief Событие клика по тайлу.
     */
    struct TileEvent {
        Vector2<size_t> mouseClickPosition; ///< Позиция клика мыши.
        bool isUpgradeKeyPressed = false;   ///< Была ли нажата клавиша улучшения.
        bool isDestroyKeyPressed = false;   ///< Была ли нажата клавиша разрушения.
    };

    /**
     * @brief Опции для взаимодействия с тайлом.
     */
    enum class TileOption {
        None = 0,    ///< Нет действия.
        Upgrade = 1, ///< Улучшение.
        Destroy = 2, ///< Разрушение.
        Create = 3,  ///< Создание.
    };

    /**
     * @brief Контекст тайла для отрисовки и обработки.
     */
    struct TileContext {
        bool status = false;           ///< Статус активности тайла.
        ID id = 0;                    ///< ID объекта на тайле.
        TileOption option = TileOption::None; ///< Выбранная опция.
    };

    /**
     * @brief Состояния игрового меню.
     */
    enum class MenuState {
        MENU,  ///< Главное меню.
        START, ///< Начало игры.
        LOAD,  ///< Загрузка игры.
        EXIT,  ///< Выход.
    };

    /**
     * @brief Презентер, связывающий модель и представление.
     *
     * Обрабатывает пользовательский ввод, обновляет модель и управляет отрисовкой.
     */
    class Presenter {
        Model& model_; ///< Ссылка на модель.
        View& view_;   ///< Ссылка на представление.

        Vector2<size_t> activeTile_ = Vector2{0, 0}; ///< Активный тайл.
        bool isTileActivated_ = false; ///< Флаг активности тайла.
        TileType activatedTileType_ = TileType::CASTLE; ///< Тип активного тайла.
        size_t activatedBuildingType_ = 0; ///< Тип активного строения.

    public:
        /**
         * @brief Конструктор презентера.
         * @param view Ссылка на представление.
         * @param model Ссылка на модель.
         */
        Presenter(View& view, Model& model) : model_(model), view_(view) {}

        /**
         * @brief Обрабатывает клик по тайлу.
         * @param event Событие клика.
         * @return Контекст обработанного тайла.
         */
        TileContext handleTileTap(const TileEvent& event);

        /**
         * @brief Обрабатывает клик мыши.
         * @param buttonEvent Событие нажатия кнопки мыши.
         */
        void handleMouseClick(const sf::Event::MouseButtonPressed& buttonEvent);

        /**
         * @brief Обрабатывает прокрутку колесика мыши.
         * @param mouseEvent Событие прокрутки колесика.
         */
        void handleMouseScroll(const sf::Event::MouseWheelScrolled& mouseEvent);

        /**
         * @brief Обрабатывает нажатие клавиши.
         * @param keyEvent Событие нажатия клавиши.
         */
        void handleKeyPress(const sf::Event::KeyPressed& keyEvent);

        /**
         * @brief Обрабатывает закрытие окна.
         * @param closed Событие закрытия.
         */
        void handleClose(const sf::Event::Closed& closed);

        /**
         * @brief Обрабатывает все события окна.
         * @param state Текущее состояние меню.
         * @return true если игра продолжается, false если выход.
         */
        bool handleEvents(MenuState& state);

        /**
         * @brief Основной метод отрисовки кадра.
         */
        void present();

    private:
        /**
         * @brief Отрисовывает карту тайлов.
         */
        void drawTileMap();

        /**
         * @brief Отрисовывает панель интерфейса.
         */
        void drawPanel();

        /**
         * @brief Отрисовывает стены.
         * @param killed Список уничтоженных стен.
         * @param dt Время с предыдущего кадра.
         */
        void drawWalls(const std::vector<std::pair<ID, std::string>> & killed, float dt);

        /**
         * @brief Отрисовывает башни.
         * @param killed Список уничтоженных башен.
         * @param dt Время с предыдущего кадра.
         */
        void drawTowers(const std::vector<std::pair<ID, std::string>>& killed, float dt);

        /**
         * @brief Отрисовывает врагов.
         * @param killed Список уничтоженных врагов.
         * @param dt Время с предыдущего кадра.
         */
        void drawEnemies(const std::vector<std::pair<ID, std::string>> & killed, float dt);

        /**
         * @brief Отрисовывает замок.
         * @param dt Время с предыдущего кадра.
         */
        void drawCastle(float dt);

        /**
         * @brief Отрисовывает полоску здоровья.
         * @param health Текущее здоровье.
         * @param maxHealth Максимальное здоровье.
         * @param pos Позиция для отрисовки.
         * @param id ID объекта.
         */
        void drawHP(float health, float maxHealth, Vector2<size_t> pos, ID id);

        /**
         * @brief Отрисовывает главное меню.
         */
        void drawMenu();

        /**
         * @brief Обрабатывает события в меню.
         * @param exit Состояние выхода из меню.
         * @return true если меню активно.
         */
        bool handleMenu(MenuState& exit);

        /**
         * @brief Обрабатывает клики мыши в меню.
         * @param buttonEvent Событие клика.
         * @param exit Состояние выхода.
         */
        void handleMenuMouseClick(const sf::Event::MouseButtonPressed& buttonEvent, MenuState& exit);

        /**
         * @brief Отрисовывает FPS.
         * @param dt Время с предыдущего кадра.
         */
        void drawFps(float dt);

        /**
         * @brief Отрисовывает количество денег.
         */
        void drawMoney();

        /**
         * @brief Проверяет, является ли индекс типом башни.
         * @param x Индекс типа.
         * @return true если это башня.
         */
        static bool isTower(size_t x) {return x % 2 == 1;}

        /**
         * @brief Проверяет, является ли индекс типом стены.
         * @param x Индекс типа.
         * @return true если это стена.
         */
        static bool isWall(size_t x) {return x % 2 == 0;}
    };
}