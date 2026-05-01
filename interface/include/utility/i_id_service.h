#pragma once
#include "type_names.h"
#include "../config/i_config_reader.h"
namespace TowerDefence
{
    /**
     * @class IIDService
     * @brief Сервис для управления уникальными идентификаторами (ID).
     *
     * Предоставляет механизм генерации и управления уникальными идентификаторами
     * для игровых объектов с возможностью сохранения и загрузки состояния.
     */
    class IIDService {
    public:
        virtual ~IIDService() = default;

        /**
         * @brief Получить новый уникальный ID.
         * @return Новый уникальный ID.
         */
        virtual ID getID() = 0;

        /**
         * @brief Загрузить состояние из конфигурации.
         * @param config IConfigReader для чтения состояния.
         */
        virtual void setFromConfig(const IConfigReader& config) = 0;

        /**
         * @brief Сохранить состояние в конфигурацию.
         * @param config IConfigReader для записи состояния.
         */
        virtual void toConfig(IConfigReader& config) = 0;
    };
}
