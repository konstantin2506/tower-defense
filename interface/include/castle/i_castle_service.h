#pragma once
#include "vector2.h"
#include "type_names.h"

namespace TowerDefence
{
    /**
     * @class ICastleService
     * @brief Сервис управления замком в игре.
     *
     * Предоставляет интерфейс для управления главным замком игрока:
     * улучшение, ремонт, получение урона и проверка состояния.
     */
    class ICastleService {
    public:
        virtual ~ICastleService() = default;

        /**
         * @brief Улучшить замок.
         * @return true если улучшение успешно, false в противном случае.
         */
        virtual bool upgradeCastle() = 0;

        /**
         * @brief Отремонтировать замок.
         * @return true если ремонт успешен, false в противном случае.
         */
        virtual bool repairCastle() = 0;

        /**
         * @brief Обновить состояние замка.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        virtual void update(float dt) = 0;

        /**
         * @brief Нанести урон замку.
         * @param damage Количество урона.
         */
        virtual void dealDamage(float damage) = 0;

        /**
         * @brief Получить ID замка.
         * @return ID замка.
         */
        [[nodiscard]] virtual ID getCastleID() const = 0;

        /**
         * @brief Получить позицию замка на карте.
         * @return Vector2<size_t> позиция замка.
         */
        [[nodiscard]] virtual Vector2<size_t> getPosition() const = 0;

        /**
         * @brief Проверить, жив ли замок.
         * @return true если замок жив, false если уничтожен.
         */
        [[nodiscard]] virtual bool isAlive() const = 0;

        /**
         * @brief Сбросить состояние сервиса.
         */
        virtual void reset() = 0;

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader &config) = 0;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader для чтения состояния.
         */
        virtual void load(const IConfigReader& config) = 0;
    };
}
