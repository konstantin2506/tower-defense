#pragma once

#include "config/i_config_reader.h"
#include "i_wall.h"

#include <memory>

namespace TowerDefence
{
    /**
     * @class IWallFactory
     * @brief Фабрика для создания и восстановления стен.
     *
     * Абстрактная фабрика, предоставляющая интерфейс для создания
     * новых экземпляров стен и их восстановления из конфигурации.
     */
    class IWallFactory {
    public:
        virtual ~IWallFactory() = default;

        /**
         * @brief Создать новую стену.
         * @param position Vector2<size_t> позиция стены.
         * @param id ID для новой стены.
         * @return std::unique_ptr<IWall> на созданную стену.
         */
        virtual std::unique_ptr<IWall> create(Vector2<size_t> position, ID id) = 0;

        /**
         * @brief Восстановить стену из конфигурации.
         * @param cfg IConfigReader с данными для восстановления.
         * @return std::unique_ptr<IWall> на восстановленную стену.
         */
        virtual std::unique_ptr<IWall> restore(const IConfigReader& cfg) = 0;
    };
}
