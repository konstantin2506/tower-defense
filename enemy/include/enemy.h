#pragma once

#include "enemy/i_enemy.h"
#include "utility/entity_type.h"

#include "ability_component.h"
#include "effect_component.h"
#include "health_component.h"
#include "move_component.h"

namespace TowerDefence
{
    /**
     * @brief Класс, представляющий врага в игре.
     *
     * Является конкретной реализацией интерфейса IEnemy.
     * Содержит компоненты для управления здоровьем, движением, эффектами и способностями.
     * Использует паттерн "Стратегия" для атаки.
     * Объект является неизменяемым (final).
     */
    class Enemy final: public IEnemy {
        const MovableType& type_;                       ///< Константная ссылка на тип врага (летающий, наземный и т.д.).
        Vector2<size_t> position_;                      ///< Позиция врага на игровом поле.
        ID id_;                                         ///< Уникальный идентификатор врага.
        std::unique_ptr<IAttackStrategy> attackStrategy_; ///< Стратегия атаки врага.
        HealthComponent health_;                        ///< Компонент управления здоровьем.
        MoveComponent moveComponent_;                   ///< Компонент управления движением.
        EffectComponent<IEnemy> effectComponent_;       ///< Компонент управления наложенными эффектами.
        AbilityComponent abilityComponent_;             ///< Компонент управления способностями.

    public:
        /**
         * @brief Конструктор врага.
         * @param type Ссылка на тип врага (MovableType).
         * @param health Текущее значение здоровья.
         * @param maxHealth Максимальное значение здоровья.
         * @param regen unique_ptr на компонент регенерации здоровья.
         * @param strategy unique_ptr на стратегию атаки.
         * @param abilities Вектор unique_ptr на способности врага.
         * @param position Начальная позиция врага на поле.
         * @param id Уникальный идентификатор врага.
         * @param velocity Скорость движения врага.
         */
        Enemy(const MovableType& type, float health, float maxHealth, std::unique_ptr<IRegenable> regen,
            std::unique_ptr<IAttackStrategy> strategy, std::vector<std::unique_ptr<IAbility>> abilities, Vector2<size_t> position, ID id, float velocity) :
        type_(type), position_(position), id_(id), attackStrategy_(std::move(strategy)),
        health_(health, maxHealth, std::move(regen)), moveComponent_(velocity), abilityComponent_(std::move(abilities)) {}

        /**
         * @brief Перемещает врага на заданный вектор.
         * @param vector Вектор перемещения (может быть отрицательным).
         * @warning Попытка переместиться в отрицательную позицию приводит к неопределенному поведению (UB).
         *          Метод следует использовать только после алгоритма поиска пути, который гарантирует корректный вектор.
         */
        void move(Vector2<int> vector) override;

        /**
         * @brief Умножает текущую скорость врага на множитель.
         * @param multiplier Множитель скорости.
         */
        void multiplyVelocity(float multiplier) override {moveComponent_.multiplyVelocity(multiplier);}

        /**
         * @brief Наносит урон врагу.
         * @param damage Количество урона.
         */
        void dealDamage(float damage) override { health_.dealDamage(damage);}

        /**
         * @brief Лечит врага.
         * @param heal Количество восстанавливаемого здоровья.
         */
        void dealHeal(float heal) override {health_.dealHeal(heal);}

        /**
         * @brief Умножает максимальное здоровье врага на множитель.
         * @param multiplier Множитель максимального здоровья.
         */
        void multiplyMaxHealth(float multiplier) override {health_.multiplyMaxHealth(multiplier);}

        /**
         * @brief Умножает множитель поглощаемого урона.
         * @param multiplier Множитель для поглощения урона.
         */
        void multiplyAbsorbedDamage(float multiplier) override { health_.multiplyAbsorbedDamage(multiplier);}

        /**
         * @brief Умножает множитель поглощаемого лечения.
         * @param multiplier Множитель для поглощения лечения.
         */
        void multiplyAbsorbedHeal(float multiplier) override {health_.multiplyAbsorbedHeal(multiplier);}

        /**
         * @brief Возвращает уникальный идентификатор врага.
         * @return ID врага.
         */
        [[nodiscard]] ID getID() const override {return id_;}

        /**
         * @brief Возвращает текущую позицию врага на поле.
         * @return Вектор позиции.
         */
        [[nodiscard]] Vector2<size_t> getPosition() const override {return position_;}

        /**
         * @brief Пытается атаковать с использованием текущей стратегии.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         * @return true, если атака была совершена, иначе false.
         */
        bool attack(float dt) override {return attackStrategy_->tryAttack(*this, dt);}

        /**
         * @brief Накладывает эффект на врага.
         * @param effect unique_ptr на эффект для применения.
         */
        void applyEffect(std::unique_ptr<IEffect<IEnemy>> effect) override {effectComponent_.addEffect(std::move(effect), *this);}

        /**
         * @brief Добавляет способность врагу.
         * @param ability unique_ptr на добавляемую способность.
         */
        void addAbility(std::unique_ptr<IAbility> ability) override {abilityComponent_.addAbility(std::move(ability));}

        /**
         * @brief Устанавливает новую позицию врага.
         * @param position Новая позиция.
         */
        void setPosition(Vector2<size_t> position) override {position_ = position;}

        /**
         * @brief Устанавливает новую стратегию атаки.
         * @param attackStrategy unique_ptr на новую стратегию атаки.
         */
        void setAttackStrategy(std::unique_ptr<IAttackStrategy> attackStrategy) override {attackStrategy_ = std::move(attackStrategy);}

        /**
         * @brief Устанавливает новый идентификатор врага.
         * @param id Новый идентификатор.
         */
        void setID(ID id) override {id_ = id;}

        /**
         * @brief Создает копию текущей стратегии атаки.
         * @return unique_ptr на клонированную стратегию атаки.
         */
        [[nodiscard]] std::unique_ptr<IAttackStrategy> getAttackStrategy() override {return attackStrategy_->clone();}

        /**
         * @brief Обновляет состояние всех наложенных эффектов.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         */
        void updateEffects(float dt) override {effectComponent_.updateAllEffects(*this, dt);}

        /**
         * @brief Проверяет, жив ли враг.
         * @return true, если здоровье больше 0, иначе false.
         */
        [[nodiscard]] bool isAlive() const override {return health_.isAlive();}

        /**
         * @brief Мгновенно убивает врага (устанавливает здоровье в 0).
         */
        void kill() override {health_.kill();}

        /**
         * @brief Возвращает текущее здоровье врага.
         * @return Значение текущего здоровья.
         */
        [[nodiscard]] float getHealth() const override {return health_.getHealth();}

        /**
         * @brief Возвращает максимальное здоровье врага.
         * @return Значение максимального здоровья.
         */
        [[nodiscard]] float getMaxHealth() const override {return health_.getMaxHealth();}

        /**
         * @brief Создает глубокую копию (клон) текущего врага.
         * @details Идентификатор клона устанавливается в 0.
         * @return unique_ptr на клонированный объект врага.
         */
        [[nodiscard]] std::unique_ptr<IEnemy> clone() const override { return std::make_unique<Enemy>(
            type_, health_.getHealth(), health_.getMaxHealth(), health_.getRegen(),
            attackStrategy_->clone(), abilityComponent_.getAllAbilities(), position_, 0, moveComponent_.getVelocity()
            );}

        /**
         * @brief Возвращает копии всех способностей врага.
         * @return Вектор unique_ptr на способности.
         */
        [[nodiscard]] std::vector<std::unique_ptr<IAbility>> getAbilities() const override {return abilityComponent_.getAllAbilities();}

        /**
         * @brief Полностью заменяет набор способностей врага.
         * @param abilities Вектор unique_ptr на новые способности.
         */
        void setAbilities(std::vector<std::unique_ptr<IAbility>> abilities) override {abilityComponent_ = AbilityComponent(std::move(abilities));}

        /**
         * @brief Основной метод обновления состояния врага за кадр.
         * @details Обновляет движение, эффекты и способности.
         * @param dt Время, прошедшее с предыдущего кадра (delta time).
         * @return Расстояние, которое враг прошел за этот кадр.
         */
        float update(float dt) override;

        /**
         * @brief Возвращает тип движения врага.
         * @return MoveType (например, наземный, летающий).
         */
        [[nodiscard]] MoveType getMoveType() const override {return type_.getMoveType();}

        /**
         * @brief Возвращает константную ссылку на тип врага.
         * @return Ссылка на MovableType.
         */
        [[nodiscard]] const MovableType& getType() const override {return type_;}

        /**
         * @brief Возвращает базовое расстояние, которое враг проходит за единицу времени.
         * @return Базовая дистанция за шаг.
         */
        [[nodiscard]] float getDs() const override {return moveComponent_.getDs();}

        /**
         * @brief Возвращает вектор направления взгляда/движения врага.
         * @return Вектор направления.
         */
        [[nodiscard]] Vector2<int> getSightVector() const override {return moveComponent_.getSightVector();}

        /**
         * @brief Устанавливает вектор направления взгляда/движения врага.
         * @param vec Новый вектор направления.
         */
        void setSightVector(Vector2<int> vec) override {moveComponent_.setSightVector(vec);}

        /**
         * @brief Загружает состояние врага из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void setFromConfig(const IConfigReader &config) override;

        /**
         * @brief Сохраняет состояние врага в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) const override;
    };

}