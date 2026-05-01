#pragma once


#include "type_names.h"
#include "vector2.h"
#include "i_enemy.h"
#include "config/i_config_reader.h"
#include "tower/i_tower.h"

#include <functional>
#include <vector>

namespace TowerDefence
{
    /**
     * @class IEnemyService
     * @brief Сервис управления врагами в игре.
     *
     * Предоставляет интерфейс для создания, управления, обновления и взаимодействия
     * со всеми врагами на игровом поле.
     */
    class IEnemyService {
    public:
        virtual ~IEnemyService() = default;

        /**
         * @brief Создать нового врага.
         * @param enemyName Имя типа врага.
         * @param position Vector2<size_t> позиция создания врага на карте.
         */
        virtual void createEnemy(const std::string& enemyName, Vector2<size_t> position) = 0;

        /**
         * @brief Восстановить врага из конфигурации.
         * @param cfg IConfigReader с данными врага.
         */
        virtual void restoreEnemy(const IConfigReader& cfg) = 0;

        /**
         * @brief Убить врага.
         * @param id ID врага.
         */
        virtual void killEnemy(ID id) = 0;

        /**
         * @brief Нанести урон врагу.
         * @param id ID врага.
         * @param damage Количество урона.
         */
        virtual void damageEnemy(ID id, float damage) = 0;

        /**
         * @brief Исцелить врага.
         * @param id ID врага.
         * @param heal Количество здоровья для восстановления.
         */
        virtual void healEnemy(ID id, float heal) = 0;

        /**
         * @brief Добавить эффект врагу.
         * @param id ID врага.
         * @param effect std::unique_ptr<IEffect<IEnemy>> на эффект.
         */
        virtual void addEffect(ID id, std::unique_ptr<IEffect<IEnemy>> effect) = 0;

        /**
         * @brief Выполнить функцию для каждого врага.
         * @param callback std::function, применяемая к каждому врагу.
         */
        virtual void forEach(std::function<void(const IEnemy&)>) const = 0;

        /**
         * @brief Получить позицию врага.
         * @param id ID врага.
         * @return Vector2<size_t> позиция врага на карте.
         */
        [[nodiscard]] virtual Vector2<size_t> getPosition(ID id) const = 0;


        [[nodiscard]] virtual std::vector<ID> getInRadius(Vector2<size_t> point, size_t radius) const = 0;
        /**
         * @brief Получить ближайшего врага к точке.
         * @param point Vector2<size_t> точка для поиска.
         * @param radius Максимальный радиус поиска.
         * @return ID ближайшего врага или невалидный ID, если врагов нет.
         *

        /**
         * @brief Инициализировать сервис из конфигурации.
         * @param config IConfigReader.
         */
        virtual void init(const IConfigReader& config) = 0;

        /**
         * @brief Собрать информацию об уничтоженных врагах.
         * @return std::vector<std::pair<ID, std::string>> для уничтоженных врагов.
         */
        virtual std::vector<std::pair<ID, std::string>> collectDestroyed() = 0;

        /**
         * @brief Обновить состояние всех врагов.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        virtual void update(float dt) = 0;

        /**
         * @brief Сбросить состояние сервиса.
         */
        virtual void reset() = 0;

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи.
         */
        virtual void toConfig(IConfigReader& config) = 0;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader для чтения.
         */
        virtual void load(const IConfigReader& config) = 0;
    };
}
