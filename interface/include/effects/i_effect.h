#pragma once
#include <memory>
#include "config/i_config_reader.h"
namespace TowerDefence
{
    /**
     * @class IEffect
     * @brief Шаблонный интерфейс для эффектов, применяемых к игровым объектам.
     *
     * Определяет базовые операции для всех эффектов в игре: применение,
     * удаление, обновление и клонирование. Эффекты могут временно изменять
     * характеристики объектов (скорость, урон, здоровье и т.д.).
     * @tparam OwnerT Тип объекта, к которому применяется эффект.
     */
    template<typename OwnerT>
    class IEffect {
    public:
        virtual ~IEffect() = default;

        /**
         * @brief Применить эффект к объекту.
         * @param owner Ссылка на объект, к которому применяется эффект.
         */
        virtual void applyEffect(OwnerT&) = 0;

        /**
         * @brief Удалить эффект с объекта.
         * @param owner Ссылка на объект, с которого удаляется эффект.
         */
        virtual void removeEffect(OwnerT&) = 0;

        /**
         * @brief Обновить состояние эффекта.
         * @param owner Ссылка на объект, к которому применен эффект.
         * @param dt Время, прошедшее с предыдущего обновления (delta time).
         */
        virtual void updateEffect(OwnerT&, float dt) = 0;

        /**
         * @brief Создать копию эффекта.
         * @return std::unique_ptr<IEffect> клонированный эффект.
         */
        virtual std::unique_ptr<IEffect> clone() = 0;

        /**
         * @brief Проверить, завершился ли эффект.
         * @return true если эффект завершился, false в противном случае.
         */
        [[nodiscard]] virtual  bool isFinished() const = 0;

        /**
         * @brief Настроить эффект из конфигурации.
         * @param config IConfigReader с параметрами эффекта.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Получить множитель эффекта.
         * @return float множитель эффекта (например, для изменения скорости или урона).
         */
        [[nodiscard]] virtual float getMultiplier() const = 0;
    };
}
