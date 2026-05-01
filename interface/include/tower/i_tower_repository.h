#pragma once
#include <functional>

#include "../utility/entity_type.h"
#include "i_tower.h"

namespace TowerDefence
{
    /**
     * @class ITowerRepository
     * @brief Репозиторий для хранения и управления типами и экземплярами башен.
     *
     * Предоставляет интерфейс для работы с коллекцией башен:
     * добавление типов, получение башен, итерация по коллекции.
     */
    class ITowerRepository {
    public:
        virtual ~ITowerRepository() = default;

        /**
         * @brief Добавить новый тип башни.
         * @param name Имя типа башни.
         * @param allLevelConfigs IConfigReader с конфигурациями всех уровней башни.
         */
        virtual void addType(const std::string& name, const IConfigReader& allLevelConfigs) = 0;

        /**
         * @brief Получить ссылку на тип башни по имени.
         * @param name Имя типа башни.
         * @return const UpgradableType& тип башни.
         */
        [[nodiscard]] virtual const UpgradableType& getTypeRef(const std::string& name) const = 0;

        /**
         * @brief Получить башню по ID.
         * @param id ID башни.
         * @return ITower& ссылка на башню.
         */
        [[nodiscard]] virtual ITower& getTower(ID id) const  = 0;

        /**
         * @brief Выполнить функцию для каждой башни в репозитории.
         * @param callback std::function обратного вызова.
         */
        virtual void forEach(std::function<void(ITower&)> callback) const = 0;

        /**
         * @brief Добавить башню в репозиторий.
         * @param id ID башни.
         * @param tower std::unique_ptr<ITower> на башню.
         */
        virtual void addTower(ID id, std::unique_ptr<ITower> tower) = 0;

        /**
         * @brief Удалить башню из репозитория.
         * @param id ID башни.
         */
        virtual void removeTower(ID id) = 0;
    };
}
