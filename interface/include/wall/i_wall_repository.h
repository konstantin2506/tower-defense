#pragma once
#include <functional>

#include "utility/entity_type.h"
#include "i_wall.h"

namespace TowerDefence
{
    /**
     * @class IWallRepository
     * @brief Репозиторий для хранения и управления типами и экземплярами стен.
     *
     * Предоставляет интерфейс для работы с коллекцией стен:
     * добавление типов, получение стен, итерация по коллекции.
     */
    class IWallRepository {
    public:
        virtual ~IWallRepository() = default;

        /**
         * @brief Добавить новый тип стены.
         * @param name Имя типа стены.
         * @param allLevelConfigs IConfigReader с конфигурациями всех уровней стены.
         */
        virtual void addType(const std::string& name, const IConfigReader& allLevelConfigs) = 0;

        /**
         * @brief Получить ссылку на тип стены по имени.
         * @param name Имя типа стены.
         * @return const UpgradableType& тип стены.
         */
        [[nodiscard]] virtual const UpgradableType& getTypeRef(const std::string& name) const = 0;

        /**
         * @brief Получить стену по ID.
         * @param id ID стены.
         * @return IWall& ссылка на стену.
         */
        [[nodiscard]] virtual IWall& getWall(ID id) const  = 0;

        /**
         * @brief Выполнить функцию для каждой стены в репозитории.
         * @param callback std::function обратного вызова.
         */
        virtual void forEach(std::function<void(IWall&)> callback) const = 0;

        /**
         * @brief Добавить стену в репозиторий.
         * @param id ID стены.
         * @param wall std::unique_ptr<IWall> на стену.
         */
        virtual void addWall(ID id, std::unique_ptr<IWall> wall) = 0;

        /**
         * @brief Удалить стену из репозитория.
         * @param id ID стены.
         */
        virtual void removeWall(ID id) = 0;
    };
}
