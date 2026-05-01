#pragma once
#include "../../interface/include/wall/i_wall.h"
#include "effect_component.h"
#include "../../interface/include/utility/entity_type.h"
#include "health_component.h"
#include "level_component.h"


namespace TowerDefence
{
    /**
     * @brief Конкретный класс стены в игре.
     *
     * Реализует интерфейс IWall. Содержит компоненты для управления
     * здоровьем, уровнями и эффектами.
     */
    class Wall final : public IWall {
        const UpgradableType& type_; ///< Константная ссылка на тип стены.
        EffectComponent<IWall> effectComponent_; ///< Компонент управления эффектами.
        LevelComponent level_; ///< Компонент управления уровнями.
        HealthComponent health_; ///< Компонент управления здоровьем.
        ID id_; ///< Уникальный идентификатор стены.
        Vector2<size_t> position_; ///< Позиция стены на карте.

    public:
        /**
         * @brief Конструктор стены.
         * @param type Тип стены.
         * @param maxLevel Максимальный уровень стены.
         * @param health Текущее здоровье.
         * @param maxHealth Максимальное здоровье.
         * @param regen Компонент регенерации здоровья.
         * @param id Уникальный идентификатор.
         * @param position Позиция на карте.
         */
        Wall(const UpgradableType& type, Level maxLevel, float health, float maxHealth, std::unique_ptr<IRegenable> regen,
            ID id, Vector2<size_t> position)
        : type_(type), level_(maxLevel), health_(health, maxHealth, std::move(regen)), id_(id), position_(position) {}

        Wall(const Wall& other) = default; ///< Конструктор копирования.

        /**
         * @brief Наносит урон стене.
         * @param damage Количество урона.
         */
        void dealDamage(float damage) override {health_.dealDamage(damage);}

        /**
         * @brief Лечит стену.
         * @param heal Количество здоровья.
         */
        void dealHeal(float heal) override {health_.dealHeal(heal);}

        /**
         * @brief Умножает максимальное здоровье стены.
         * @param multiplier Множитель.
         */
        void multiplyMaxHealth(float multiplier) override {health_.multiplyMaxHealth(multiplier);}

        /**
         * @brief Умножает поглощаемый урон.
         * @param multiplier Множитель.
         */
        void multiplyAbsorbedDamage(float multiplier) override {health_.multiplyAbsorbedDamage(multiplier);}

        /**
         * @brief Умножает поглощаемое лечение.
         * @param multiplier Множитель.
         */
        void multiplyAbsorbedHeal(float multiplier) override {health_.multiplyAbsorbedHeal(multiplier);}

        /**
         * @brief Возвращает уникальный идентификатор стены.
         */
        [[nodiscard]] ID getID() const override {return id_;}

        /**
         * @brief Возвращает позицию стены на карте.
         */
        [[nodiscard]] Vector2<size_t> getPosition() const override {return position_;}

        /**
         * @brief Накладывает эффект на стену.
         * @param effect Эффект для применения.
         */
        void applyEffect(std::unique_ptr<IEffect<IWall>> effect) override {effectComponent_.addEffect(std::move(effect), *this);}

        /**
         * @brief Ремонтирует стену на указанный процент.
         * @param percent Процент восстановления здоровья.
         */
        void repair(float percent) override;

        /**
         * @brief Устанавливает новый идентификатор стены.
         * @param id Новый идентификатор.
         */
        void setID(ID id) override {id_ = id;}

        /**
         * @brief Устанавливает новую позицию стены.
         * @param position Новая позиция.
         */
        void setPosition(Vector2<size_t> position) override { position_ = position;}

        /**
         * @brief Обновляет состояние всех эффектов на стене.
         * @param dt Время с предыдущего кадра.
         */
        void updateEffects(float dt) override {effectComponent_.updateAllEffects(*this, dt);}

        /**
         * @brief Возвращает текущий уровень стены.
         */
        [[nodiscard]] Level getLevel() const override {return level_.getCurrentLevel();}

        /**
         * @brief Проверяет, жива ли стена (не разрушена).
         */
        [[nodiscard]] bool isAlive() const override {return health_.isAlive();}

        /**
         * @brief Уничтожает стену (устанавливает здоровье в 0).
         */
        void kill() override {health_.kill();}

        /**
         * @brief Создает глубокую копию стены.
         * @return unique_ptr на клонированную стену.
         */
        std::unique_ptr<IWall> clone() override
        {
            return std::make_unique<Wall>(
                type_, level_.getMaxLevel(),
                health_.getHealth(), health_.getMaxHealth(),
                health_.getRegen(), getID(), position_
            );
        }

        /**
         * @brief Улучшает стену на один уровень.
         */
        void upgrade() override;

        /**
         * @brief Возвращает константную ссылку на тип стены.
         */
        [[nodiscard]] const UpgradableType & getUpgradableType() const override {return type_;}

        /**
         * @brief Загружает состояние стены из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void setFromConfig(const IConfigReader& config) override;

        /**
         * @brief Возвращает текущее здоровье стены.
         */
        [[nodiscard]] float getHealth() const override {return health_.getHealth();}

        /**
         * @brief Возвращает максимальное здоровье стены.
         */
        [[nodiscard]] float getMaxHealth() const override {return health_.getMaxHealth();}

        /**
         * @brief Основной метод обновления состояния стены.
         * @param dt Время с предыдущего кадра.
         */
        void update(float dt) override;

        /**
         * @brief Сохраняет состояние стены в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) override;
    };

}