#pragma once

#include "castle/i_castle_repository.h"

namespace TowerDefence
{
    /**
     * @class CastleRepository
     * @brief Репозиторий для хранения замка.
     *
     * Финальная реализация ICastleRepository, управляющая
     * единственным экземпляром замка и его типом.
     * Обеспечивает доступ к замку и его типу по имени.
     */
    class CastleRepository final: public ICastleRepository {
        std::unique_ptr<ICastle> castle_;
        UpgradableType castleType_;
    public:
        /**
         * @brief Конструктор репозитория замка.
         * @param config IConfigReader для инициализации типа замка.
         * @details Создает UpgradableType на основе конфигурации.
         */
        explicit CastleRepository(const IConfigReader& config);

        /**
         * @brief Получить ссылку на тип замка по имени.
         * @param name Имя типа замка.
         * @return const UpgradableType& ссылка на тип замка.
         * @throws Может выбросить исключение, если тип не найден.
         */
        [[nodiscard]] const UpgradableType & getTypeRef(const std::string &name) const override;

        /**
         * @brief Получить экземпляр замка.
         * @return ICastle& ссылка на замок.
         * @throws Может выбросить исключение, если замок не установлен.
         */
        [[nodiscard]] ICastle & getCastle() const override;

        /**
         * @brief Добавить замок в репозиторий.
         * @param castle std::unique_ptr<ICastle> на замок.
         * @details Заменяет существующий замок, если он был.
         */
        void addCastle(std::unique_ptr<ICastle> castle) override {castle_ = std::move(castle);}

        /**
         * @brief Удалить замок из репозитория.
         * @details Освобождает память, занимаемую замком.
         */
        void removeCastle() override;;
    };
}