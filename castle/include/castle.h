#pragma once
#include "utility/entity_type.h"
#include "health_component.h"
#include "castle/i_castle.h"
#include "level_component.h"


namespace TowerDefence
{
    /**
     * @class Castle
     * @brief Реализация замка игрока.
     *
     * Финальный класс, представляющий главный замок игрока.
     * Комбинирует компоненты здоровья, уровня и позиционирования.
     * Наследует интерфейсы IUpgradable, IRepairable, IIDObject, IHealthable, IUpdatable.
     */
    class Castle final : public ICastle {
        const UpgradableType& type_;
        LevelComponent level_;
        HealthComponent health_;
        ID id_;
        Vector2<size_t> position_;

    public:
        /**
         * @brief Конструктор замка.
         * @param type Ссылка на тип замка (UpgradableType).
         * @param maxLevel Максимальный уровень улучшения.
         * @param health Текущее здоровье.
         * @param maxHealth Максимальное здоровье.
         * @param regen std::unique_ptr<IRegenable> компонент регенерации здоровья.
         * @param id Уникальный идентификатор (ID).
         * @param position Vector2<size_t> позиция на карте.
         */
        Castle(const UpgradableType& type, int maxLevel, float health, float maxHealth, std::unique_ptr<IRegenable> regen, ID id, Vector2<size_t> position)
        : type_(type), level_(maxLevel), health_(health, maxHealth, std::move(regen)), id_(id) , position_(position) {}

        /**
         * @brief Улучшить замок на следующий уровень.
         * @details Увеличивает уровень замка, если это возможно.
         */
        void upgrade() override;

        /**
         * @brief Отремонтировать замок.
         * @param percent Процент от максимального здоровья для восстановления.
         */
        void repair(float percent) override;

        /**
         * @brief Получить ID замка.
         * @return ID уникальный идентификатор замка.
         */
        [[nodiscard]] ID getID() const override { return id_; }

        /**
         * @brief Получить позицию замка.
         * @return Vector2<size_t> позиция замка на карте.
         */
        [[nodiscard]] Vector2<size_t> getPosition() const override { return position_; }

        /**
         * @brief Нанести урон замку.
         * @param damage Количество урона.
         */
        void dealDamage(float damage) override { health_.dealDamage(damage);}

        /**
         * @brief Исцелить замок.
         * @param heal Количество здоровья для восстановления.
         */
        void dealHeal(float heal) override { health_.dealHeal(heal);}

        /**
         * @brief Умножить максимальное здоровье замка.
         * @param multiplier Множитель максимального здоровья.
         */
        void multiplyMaxHealth(float multiplier) override { health_.multiplyMaxHealth(multiplier);}

        /**
         * @brief Умножить поглощаемый урон.
         * @param multiplier Множитель поглощаемого урона.
         */
        void multiplyAbsorbedDamage(float multiplier) override { health_.multiplyAbsorbedDamage(multiplier);}

        /**
         * @brief Умножить поглощаемое лечение.
         * @param multiplier Множитель поглощаемого лечения.
         */
        void multiplyAbsorbedHeal(float multiplier) override { health_.multiplyAbsorbedHeal(multiplier);}

        /**
         * @brief Установить новый ID замка.
         * @param id Новый уникальный идентификатор.
         */
        void setID(ID id) override {id_ = id;}

        /**
         * @brief Установить новую позицию замка.
         * @param position Vector2<size_t> новая позиция.
         */
        void setPosition(Vector2<size_t> position) override {position_ = position;}

        /**
         * @brief Получить текущий уровень замка.
         * @return Level текущий уровень улучшения.
         */
        [[nodiscard]] Level getLevel() const override {return level_.getCurrentLevel();}

        /**
         * @brief Проверить, жив ли замок.
         * @return true если здоровье > 0, false если замок уничтожен.
         */
        [[nodiscard]] bool isAlive() const override {return health_.isAlive();}

        /**
         * @brief Уничтожить замок (установить здоровье в 0).
         */
        void kill() override {health_.kill();}

        /**
         * @brief Получить текущее здоровье замка.
         * @return float текущее здоровье.
         */
        [[nodiscard]] float getHealth() const override {return health_.getHealth();}

        /**
         * @brief Получить максимальное здоровье замка.
         * @return float максимальное здоровье.
         */
        [[nodiscard]] float getMaxHealth() const override {return health_.getMaxHealth();}

        /**
         * @brief Получить тип замка.
         * @return const UpgradableType& ссылка на тип замка.
         */
        [[nodiscard]] const UpgradableType& getUpgradableType() const override {return type_;}

        /**
         * @brief Обновить состояние замка.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         * @details Обновляет регенерацию здоровья и другие временные эффекты.
         */
        void update(float dt) override;

        /**
         * @brief Настроить замок из конфигурации.
         * @param config IConfigReader с параметрами замка.
         */
        void setFromConfig(const IConfigReader &config) override;

        /**
         * @brief Сохранить состояние замка в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        void toConfig(IConfigReader &config) override;
    };
}