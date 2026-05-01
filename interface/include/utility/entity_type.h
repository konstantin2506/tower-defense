#pragma once

#include "../config/i_config_reader.h"
#include "type_names.h"


namespace TowerDefence
{
    /**
     * @class Type
     * @brief Базовый класс для всех типов сущностей в игре.
     *
     * Содержит базовую информацию о типе сущности - ее имя.
     */
    class Type {
        std::string name_;
    public:
        explicit Type(std::string name) : name_(std::move(name)) {}
        virtual ~Type() = default;

        /**
         * @brief Получить имя типа.
         * @return std::string имя типа.
         */
        [[nodiscard]] std::string getName() const { return name_; }
    };

    /**
     * @class UpgradableType
     * @brief Тип сущности, которую можно улучшать (здания, башни).
     *
     * Расширяет базовый Type, добавляя поддержку уровней улучшения
     * и соответствующих конфигураций для каждого уровня.
     */
    class UpgradableType : public Type {
        std::vector<std::unique_ptr<IConfigReader>> levelConfigs_;
    public:
        UpgradableType() : Type("None") {}
        UpgradableType(std::string name, std::vector<std::unique_ptr<IConfigReader>> configs) : Type(std::move(name)), levelConfigs_(std::move(configs)) {}

        /**
         * @brief Получить конфигурацию для определенного уровня.
         * @param level Level для которого нужна конфигурация.
         * @return const IConfigReader& конфигурация уровня.
         */
        [[nodiscard]] const IConfigReader& getConfig(Level level) const {return *levelConfigs_.at(level);}

        /**
         * @brief Получить максимальный уровень улучшения.
         * @return Level максимальный уровень.
         */
        [[nodiscard]] Level getMaxLevel() const {return levelConfigs_.size() - 1;}
    };

    /**
     * @class MovableType
     * @brief Тип подвижной сущности (враги, юниты).
     *
     * Расширяет базовый Type, добавляя информацию о типе передвижения.
     */
    class MovableType : public Type {
        MoveType move_type_;
    public:
        MovableType() : Type("None"), move_type_(MoveType::LANDED) {}
        MovableType(std::string name, MoveType move) : Type(std::move(name)), move_type_(move) {}

        /**
         * @brief Получить тип передвижения.
         * @return MoveType тип передвижения.
         */
        [[nodiscard]] MoveType getMoveType() const {return move_type_;}
    };
}
