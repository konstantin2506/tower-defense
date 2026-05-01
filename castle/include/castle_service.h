#pragma once
#include "castle/i_castle_repository.h"
#include "castle/i_castle_service.h"
#include "utility/i_economy_service.h"
#include "utility/i_id_service.h"


namespace TowerDefence
{
    /**
     * @class CastleService
     * @brief Сервис управления замком.
     *
     * Финальная реализация ICastleService, координирующая
     * взаимодействие между репозиторием замка, экономикой
     * и сервисом идентификаторов. Обеспечивает бизнес-логику
     * для улучшения, ремонта и управления состоянием замка.
     */
    class CastleService final : public ICastleService {
        ICastleRepository& castleRepository_;
        IIDService& idService_;
        IEconomyService& economyService_;
    public:
        /**
         * @brief Конструктор сервиса замка.
         * @param repo Ссылка на репозиторий замка (ICastleRepository&).
         * @param iidService Ссылка на сервис идентификаторов (IIDService&).
         * @param economyService Ссылка на сервис экономики (IEconomyService&).
         * @param castlePosition Vector2<size_t> позиция замка на карте.
         */
        CastleService(ICastleRepository& repo, IIDService& iidService, IEconomyService& economyService, Vector2<size_t> castlePosition);

        /**
         * @brief Получить позицию замка.
         * @return Vector2<size_t> позиция замка на карте.
         */
        [[nodiscard]] Vector2<size_t> getPosition() const override;

        /**
         * @brief Попытаться отремонтировать замок.
         * @return true если ремонт успешен, false в противном случае.
         * @details Проверяет возможность ремонта через экономику,
         *          и если возможно, восстанавливает здоровье замка.
         */
        [[nodiscard]] bool repairCastle() override;

        /**
         * @brief Попытаться улучшить замок.
         * @return true если улучшение успешно, false в противном случае.
         * @details Проверяет возможность улучшения через экономику,
         *          и если возможно, улучшает замок на следующий уровень.
         */
        [[nodiscard]] bool upgradeCastle() override;

        /**
         * @brief Нанести урон замку.
         * @param damage Количество урона.
         */
        void dealDamage(float damage) override;

        /**
         * @brief Проверить, жив ли замок.
         * @return true если замок жив, false если уничтожен.
         */
        [[nodiscard]] bool isAlive() const override {return castleRepository_.getCastle().isAlive();}

        /**
         * @brief Получить ID замка.
         * @return ID уникальный идентификатор замка.
         */
        [[nodiscard]] ID getCastleID() const override;

        /**
         * @brief Обновить состояние сервиса замка.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @details Обновляет состояние замка (регенерация, эффекты).
         */
        void update(float dt) override;

        /**
         * @brief Сбросить состояние сервиса.
         * @details Удаляет текущий замок и сбрасывает состояние.
         */
        void reset() override;

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        void toConfig(IConfigReader &config) override;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader для чтения состояния.
         */
        void load(const IConfigReader &config) override;
    };
}