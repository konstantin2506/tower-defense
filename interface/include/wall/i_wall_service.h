#pragma once

#include "i_wall.h"
#include "type_names.h"
#include "vector2.h"

#include <functional>

namespace TowerDefence
{
    /**
     * @class IWallService
     * @brief Сервис управления стенами в игре.
     *
     * Предоставляет интерфейс для создания, улучшения, разрушения, ремонта стен,
     * управления эффектами и обновления состояния всех стен на игровом поле.
     */
    class IWallService {
    public:
        virtual ~IWallService() = default;

        /**
         * @brief Создать новую стену.
         * @param wallName Имя типа стены.
         * @param position Vector2<size_t> позиция для создания стены.
         * @return true если стена успешно создана, false в противном случае.
         */
        [[nodiscard("check if not created")]] virtual bool createWall(const std::string& wallName, Vector2<size_t> position) = 0;

        /**
         * @brief Улучшить стену.
         * @param wall ID стены для улучшения.
         * @return true если стена успешно улучшена, false в противном случае.
         */
        [[nodiscard("check if not upgraded")]] virtual bool upgradeWall(ID wall) = 0;

        /**
         * @brief Разрушить стену на указанной позиции.
         * @param position Vector2<size_t> позиция стены.
         * @return true если стена успешно разрушена, false в противном случае.
         */
        virtual bool destroyWall(Vector2<size_t> position) = 0;

        /**
         * @brief Отремонтировать стену.
         * @param wall ID стены для ремонта.
         * @return true если стена успешно отремонтирована, false в противном случае.
         */
        virtual bool repairWall(ID wall) = 0;

        /**
         * @brief Собрать информацию о разрушенных стенах.
         * @return std::vector<std::pair<ID, std::string>> пар (ID, тип) разрушенных стен.
         */
        virtual std::vector<std::pair<ID, std::string>> collectDestroyed() = 0;

        /**
         * @brief Выполнить функцию для каждой стены.
         * @param callback std::function применяемая к каждой стене.
         */
        virtual void forEach(std::function<void(const IWall&)>) const = 0;

        /**
         * @brief Нанести урон стене.
         * @param wall ID стены.
         * @param damage Количество урона.
         */
        virtual void dealDamage(ID wall, float damage) = 0;

        /**
         * @brief Добавить эффект стене.
         * @param wall ID стены.
         * @param effect std::unique_ptr<IEffect<IWall>> эффект.
         */
        virtual void addEffect(ID wall, std::unique_ptr<IEffect<IWall>> effect) = 0;

        /**
         * @brief Получить стены в радиусе от точки.
         * @param point Vector2<size_t> центральная точка.
         * @param radius Радиус поиска.
         * @return std::vector<ID> стен в радиусе.
         */
        [[nodiscard]] virtual std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const = 0;

        /**
         * @brief Получить позицию стены.
         * @param wall ID стены.
         * @return Vector2<size_t> позиция стены.
         */
        [[nodiscard]] virtual Vector2<size_t> getPosition(ID wall) const = 0;

        /**
         * @brief Проверить, уничтожена ли стена.
         * @param wall ID стены.
         * @return true если стена уничтожена, false в противном случае.
         */
        [[nodiscard]] virtual bool isKilled(ID) const = 0;

        /**
         * @brief Обновить состояние всех стен.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        virtual void update(float dt) = 0;

        /**
         * @brief Инициализировать сервис из конфигурации.
         * @param cfg IConfigReader для чтения конфигурации.
         */
        virtual void init(const IConfigReader& cfg) = 0;

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