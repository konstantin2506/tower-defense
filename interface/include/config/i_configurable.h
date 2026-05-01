#pragma once
#include "i_config_reader.h"

namespace TowerDefence
{
    /**
     * @class IConfigurable
     * @brief Интерфейс для объектов, которые можно настраивать из конфигурации.
     *
     * Определяет метод для инициализации объекта из конфигурационных данных.
     * Используется для всех объектов, которые требуют внешней конфигурации.
     */
    class IConfigurable {
    public:
        virtual ~IConfigurable() = default;

        /**
         * @brief Настроить объект из конфигурации.
         * @param cfg IConfigReader с конфигурационными данными.
         */
        virtual void setFromConfig(const IConfigReader& cfg) = 0;
    };
}
