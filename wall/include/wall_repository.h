#pragma once

#include "wall/i_wall_repository.h"

namespace TowerDefence
{
    /**
     * @brief Репозиторий для хранения и управления стенами и их типами.
     *
     * Реализует интерфейс IWallRepository.
     * Хранит все экземпляры стен и коллекцию типов стен.
     */
    class WallRepository final: public IWallRepository{
        std::unordered_map<ID, std::unique_ptr<IWall>> allWalls; ///< Мапа всех стен, ключ - ID.
        std::unordered_map<std::string, UpgradableType> wallTypes_; ///< Мапа типов стен.

    public:
        /**
         * @brief Конструктор репозитория.
         * @param allTowersConfig Конфигурация с описанием всех типов стен.
         */
        explicit WallRepository(const IConfigReader& allTowersConfig);

        /**
         * @brief Добавляет новый тип стены в репозиторий.
         * @param name Имя типа.
         * @param allLevelConfigs Конфигурация для создания типа.
         */
        void addType(const std::string& name, const IConfigReader& allLevelConfigs) override;

        /**
         * @brief Возвращает константную ссылку на тип стены по имени.
         * @param name Имя типа.
         * @return Константная ссылка на UpgradableType.
         */
        [[nodiscard]] const UpgradableType& getTypeRef(const std::string& name) const override;

        /**
         * @brief Возвращает ссылку на стену по её ID.
         * @param id Уникальный идентификатор стены.
         * @return Ссылка на объект стены.
         */
        [[nodiscard]] IWall& getWall(ID id) const override {return *allWalls.at(id);}

        /**
         * @brief Выполняет callback-функцию для каждой стены.
         * @param callback Функция, принимающая ссылку на IWall.
         */
        void forEach(std::function<void(IWall& )> callback) const override;

        /**
         * @brief Добавляет новую стену в репозиторий.
         * @param id Уникальный идентификатор для стены.
         * @param wall unique_ptr на объект стены.
         */
        void addWall(ID id, std::unique_ptr<IWall> wall) override;

        /**
         * @brief Удаляет стену из репозитория.
         * @param id Уникальный идентификатор стены для удаления.
         */
        void removeWall(ID id) override;
    };
}