#pragma once

#include "i_enemy.h"
#include "config/i_config_reader.h"
#include "utility/entity_type.h"

#include <functional>

namespace TowerDefence
{
    /**
     * @class IEnemyRepository
     * @brief Репозиторий для хранения и управления типами и экземплярами врагов.
     *
     * Предоставляет интерфейс для работы с коллекцией врагов:
     * добавление типов, получение врагов, итерация по коллекции.
     */
    class IEnemyRepository {
    public:
        virtual ~IEnemyRepository() = default;

        /**
         * @brief Добавить новый тип врага.
         * @param name Имя типа врага.
         * @param config IConfigReader конфигурация типа врага.
         */
        virtual void addType(const std::string& name, const IConfigReader& config) = 0;

        /**
         * @brief Получить ссылку на тип врага по имени.
         * @param name Имя типа врага.
         * @return const MovableType& тип врага.
         */
        [[nodiscard]] virtual const MovableType& getTypeRef(const std::string& name) const = 0;

        /**
         * @brief Получить врага по ID.
         * @param id ID врага.
         * @return IEnemy& ссылка на врага.
         */
        [[nodiscard]] virtual IEnemy& getEnemy(ID id) const  = 0;

        /**
         * @brief Выполнить функцию для каждого врага в репозитории.
         * @param callback std::function обратного вызова.
         */
        virtual void forEach(std::function<void(IEnemy&)> callback) const = 0;

        /**
         * @brief Добавить врага в репозиторий.
         * @param id ID врага.
         * @param Enemy std::unique_ptr<IEnemy> на врага.
         */
        virtual void addEnemy(ID id, std::unique_ptr<IEnemy> Enemy) = 0;

        /**
         * @brief Удалить врага из репозитория.
         * @param id ID врага.
         */
        virtual void removeEnemy(ID id) = 0;
    };
}
