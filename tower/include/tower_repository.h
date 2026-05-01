#pragma once

#include "utility/entity_type.h"
#include "tower/i_tower.h"
#include "tower/i_tower_repository.h"

#include <ranges>

namespace TowerDefence
{
    /**
     * @brief Репозиторий для хранения и управления башнями и их типами.
     *
     * Реализует интерфейс ITowerRepository.
     * Хранит все экземпляры башен и коллекцию типов башен.
     */
    class TowerRepository final: public ITowerRepository {
        std::unordered_map<ID, std::unique_ptr<ITower>> allTowers_; ///< Мапа всех башен, ключ - ID.
        std::unordered_map<std::string, UpgradableType> towerTypes_; ///< Мапа типов башен.

    public:
        /**
         * @brief Конструктор репозитория.
         * @param allTowersConfig Конфигурация с описанием всех типов башен.
         */
        explicit TowerRepository(const IConfigReader& allTowersConfig);

        /**
         * @brief Добавляет новый тип башни в репозиторий.
         * @param name Имя типа.
         * @param towerConfig Конфигурация для создания типа.
         */
        void addType(const std::string& name, const IConfigReader& towerConfig) override;

        /**
         * @brief Возвращает константную ссылку на тип башни по имени.
         * @param name Имя типа.
         * @return Константная ссылка на UpgradableType.
         */
        const UpgradableType& getTypeRef(const std::string& name) const override;

        /**
         * @brief Возвращает ссылку на башню по её ID.
         * @param id Уникальный идентификатор башни.
         * @return Ссылка на объект башни.
         */
        ITower& getTower(ID id) const override {return  *allTowers_.at(id);}

        /**
         * @brief Добавляет новую башню в репозиторий.
         * @param id Уникальный идентификатор для башни.
         * @param tower unique_ptr на объект башни.
         */
        void addTower(ID id, std::unique_ptr<ITower> tower) override {allTowers_.emplace(id, std::move(tower));}

        /**
         * @brief Выполняет callback-функцию для каждой башни.
         * @param callback Функция, принимающая ссылку на ITower.
         */
        void forEach(std::function<void(ITower&)> callback) const override;

        /**
         * @brief Удаляет башню из репозитория.
         * @param id Уникальный идентификатор башни для удаления.
         */
        void removeTower(ID id) override {allTowers_.erase(id);}


    };
}