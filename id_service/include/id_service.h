#pragma once
#include "utility/i_id_service.h"
#include "type_names.h"

namespace TowerDefence
{
    /**
     * @brief Реализация сервиса для генерации уникальных идентификаторов.
     *
     * Предоставляет простой счетчик, который выдает уникальные ID
     * и позволяет сохранять/загружать свое состояние.
     */
    class IDService final: public IIDService {
        ID IDCounter = 1; ///< Текущее значение счетчика идентификаторов.

    public:
        /**
         * @brief Генерирует и возвращает новый уникальный ID.
         * @return Новый уникальный идентификатор.
         */
        ID getID() override {return IDCounter++;}

        /**
         * @brief Загружает состояние счетчика из конфигурации.
         * @param config Объект для чтения конфигурации.
         */
        void setFromConfig(const IConfigReader &config) override {IDCounter = config.getSize_t("counter");}

        /**
         * @brief Сохраняет состояние счетчика в конфигурацию.
         * @param config Объект для записи конфигурации.
         */
        void toConfig(IConfigReader &config) override {config.writeSize_t("counter", IDCounter);}
    };
}