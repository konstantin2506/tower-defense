#pragma once

#include "i_castle.h"

namespace TowerDefence
{
    /**
     * @class ICastleRepository
     * @brief Репозиторий для хранения замка.
     *
     * Предоставляет интерфейс для работы с замком игрока:
     * получение типа, доступ к экземпляру замка, добавление и удаление замка.
     */
    class ICastleRepository{
    public:
        virtual ~ICastleRepository() = default;

        /**
         * @brief Получить ссылку на тип замка по имени.
         * @param name Имя типа замка.
         * @return const UpgradableType& тип замка.
         */
        [[nodiscard]] virtual const UpgradableType& getTypeRef(const std::string& name) const = 0;

        /**
         * @brief Получить экземпляр замка.
         * @return ICastle& ссылка на замок.
         */
        [[nodiscard]] virtual ICastle& getCastle() const  = 0;

        /**
         * @brief Добавить замок в репозиторий.
         * @param castle std::unique_ptr<ICastle> на замок.
         */
        virtual void addCastle(std::unique_ptr<ICastle> castle) = 0;

        /**
         * @brief Удалить замок из репозитория.
         */
        virtual void removeCastle() = 0;
    };
}
