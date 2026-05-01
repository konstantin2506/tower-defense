#pragma once


#include "effect_component.h"
#include "utility/entity_type.h"
#include "tower/i_tower.h"
#include "level_component.h"

namespace TowerDefence
{
    /**
     * @brief Конкретный класс башни в игре.
     *
     * Реализует интерфейс ITower. Содержит компоненты для управления
     * уровнями, эффектами и стратегией атаки.
     */
    class Tower final: public ITower {
        const UpgradableType& type_; ///< Константная ссылка на тип башни.
        std::unique_ptr<IAttackStrategy> attackStrategy_; ///< Стратегия атаки башни.
        EffectComponent<ITower> effectComponent_; ///< Компонент управления эффектами.
        ID id_; ///< Уникальный идентификатор башни.
        LevelComponent level_; ///< Компонент управления уровнями.
        Vector2<size_t> position_; ///< Позиция башни на карте.

    public:
        /**
         * @brief Конструктор башни.
         * @param type Тип башни.
         * @param strategy Стратегия атаки.
         * @param id Уникальный идентификатор.
         * @param maxLevel Максимальный уровень башни.
         * @param position Позиция на карте.
         */
        Tower(const UpgradableType& type, std::unique_ptr<IAttackStrategy> strategy, ID id, Level maxLevel, Vector2<size_t> position) :
        type_(type), attackStrategy_(std::move(strategy)), id_(id), level_(maxLevel), position_(position) {}

        /**
         * @brief Возвращает уникальный идентификатор башни.
         */
        [[nodiscard]] ID getID() const override {return id_;}

        /**
         * @brief Возвращает позицию башни на карте.
         */
        [[nodiscard]] Vector2<size_t> getPosition() const override {return position_;}

        /**
         * @brief Возвращает текущий уровень башни.
         */
        [[nodiscard]] Level getLevel() const override {return level_.getCurrentLevel();}

        /**
         * @brief Выполняет атаку с использованием текущей стратегии.
         * @param dt Время с предыдущего кадра.
         * @return true если атака была совершена.
         */
        bool attack(float dt) override {return attackStrategy_->tryAttack(*this, dt);}

        /**
         * @brief Накладывает эффект на башню.
         * @param effect Эффект для применения.
         */
        void applyEffect(std::unique_ptr<IEffect<ITower>> effect) override {effectComponent_.addEffect(std::move(effect), *this);}

        /**
         * @brief Устанавливает новую позицию башни.
         * @param position Новая позиция.
         */
        void setPosition(Vector2<size_t> position) override {position_ = position;}

        /**
         * @brief Улучшает башню на один уровень.
         */
        void upgrade() override;

        /**
         * @brief Устанавливает новую стратегию атаки.
         * @param attackStrategy Новая стратегия атаки.
         */
        void setAttackStrategy(std::unique_ptr<IAttackStrategy> attackStrategy) override {attackStrategy_ = std::move(attackStrategy);}

        /**
         * @brief Создает копию текущей стратегии атаки.
         * @return unique_ptr на клонированную стратегию.
         */
        [[nodiscard]] std::unique_ptr<IAttackStrategy>
        getAttackStrategy() override {return attackStrategy_->clone();}

        /**
         * @brief Устанавливает новый идентификатор башни.
         * @param id Новый идентификатор.
         */
        void setID(ID id) override { id_ = id; }

        /**
         * @brief Создает глубокую копию башни.
         * @return unique_ptr на клонированную башню.
         */
        [[nodiscard]] std::unique_ptr<ITower> clone() const override {return std::make_unique<Tower>(type_, attackStrategy_->clone(), id_, level_.getMaxLevel(), position_);}

        /**
         * @brief Обновляет состояние всех эффектов на башне.
         * @param dt Время с предыдущего кадра.
         */
        void updateEffects(float dt) override {effectComponent_.updateAllEffects(*this, dt);}

        /**
         * @brief Возвращает константную ссылку на тип башни.
         */
        [[nodiscard]] const UpgradableType & getUpgradableType() const override {return type_; }

        /**
         * @brief Основной метод обновления состояния башни.
         * @param dt Время с предыдущего кадра.
         */
        void update(float dt) override;

        /**
         * @brief Загружает состояние башни из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void setFromConfig(const IConfigReader &config) override;

        /**
         * @brief Сохраняет состояние башни в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) override;
    };
}