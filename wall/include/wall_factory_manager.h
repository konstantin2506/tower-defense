#pragma once

#include "wall_factory.h"
#include "utility/factory_manager.h"
#include "wall/i_wall_repository.h"

namespace TowerDefence
{
    /**
     * @brief Менеджер фабрик стен.
     *
     * Наследуется от шаблонного FactoryManager для управления фабриками,
     * создающими объекты типа IWall.
     */
    class WallFactoryManager: public FactoryManager<IWall, IWallFactory> {
    public:
        /**
         * @brief Инициализирует менеджер, создавая фабрики из конфигурации.
         * @param config Конфигурация с описанием всех типов стен.
         * @param repo Репозиторий стен для получения ссылок на типы.
         */
        void init(const IConfigReader& config, const IWallRepository& repo);

    private:
        /**
         * @brief Добавляет новую фабрику стен по имени.
         * @param name Имя типа стены.
         * @param repo Репозиторий стен для получения ссылки на тип.
         */
        void addFactory(const std::string& name, const IWallRepository& repo)
        {
            addIDObjectFactory<WallFactory>(name, repo.getTypeRef(name));
        }
    };
}