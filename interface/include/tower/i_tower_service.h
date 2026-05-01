#pragma once

#include "i_tower.h"
#include "vector2.h"

#include <functional>

namespace TowerDefence
{
    class AttackStrategyFactoryManager;

    /**
     * @class ITowerService
     * @brief Сервис управления башнями в игре.
     *
     * Предоставляет интерфейс для создания, улучшения, разрушения башен,
     * управления эффектами и обновления состояния всех башен на игровом поле.
     */
    class ITowerService {
    public:
        virtual ~ITowerService() = default;

        /**
         * @brief Создать новую башню.
         * @param towerName Имя типа башни.
         * @param position Vector2<size_t> позиция для создания башни.
         * @return true если башня успешно создана, false в противном случае.
         */
        [[nodiscard ("check if not created")]] virtual bool createTower(const std::string &towerName, Vector2<size_t> position) = 0;

        /**
         * @brief Улучшить башню.
         * @param tower ID башни для улучшения.
         * @return true если башня успешно улучшена, false в противном случае.
         */
        [[nodiscard("check if not upgraded")]] virtual bool upgradeTower(ID tower) const = 0;

        /**
         * @brief Разрушить башню на указанной позиции.
         * @param position Vector2<size_t> позиция башни.
         * @return true если башня успешно разрушена, false в противном случае.
         */
        virtual bool destroyTower(Vector2<size_t> position) = 0;

        /**
         * @brief Собрать информацию о разрушенных башнях.
         * @return std::vector<std::pair<ID, std::string>> пар (ID, тип) разрушенных башен.
         */
        virtual std::vector<std::pair<ID, std::string>>  collectDestroyed() = 0;

        /**
         * @brief Получить позицию башни.
         * @param tower ID башни.
         * @return Vector2<size_t> позиция башни.
         */
        [[nodiscard]] virtual Vector2<size_t> getPosition(ID tower) const = 0;

        /**
         * @brief Получить башни в радиусе от точки.
         * @param point Vector2<size_t> центральная точка.
         * @param radius Радиус поиска.
         * @return std::vector<ID> башен в радиусе.
         */
        [[nodiscard]] virtual std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const = 0;

        /**
         * @brief Добавить эффект башне.
         * @param tower ID башни.
         * @param effect std::unique_ptr<IEffect<ITower>> эффект.
         */
        virtual void addEffect(ID tower, std::unique_ptr<IEffect<ITower>> effect) = 0;

        /**
         * @brief Выполнить функцию для каждой башни.
         * @param callback std::function применяемая к каждой башне.
         */
        virtual void forEach(std::function<void(const ITower&)>) const = 0;

        /**
         * @brief Инициализировать сервис из конфигурации.
         * @param config IConfigReader для чтения конфигурации.
         */
        virtual void init(const IConfigReader& config) = 0;

        /**
         * @brief Обновить состояние всех башен.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        virtual void update(float dt) = 0;

        /**
         * @brief Сбросить состояние сервиса.
         */
        virtual void reset() = 0;

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader& config) = 0;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader для чтения состояния.
         */
        virtual void load(const IConfigReader& config) = 0;
    };
}
