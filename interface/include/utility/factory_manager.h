#pragma once

#include "../config/i_config_reader.h"
#include "type_names.h"
#include "vector2.h"

#include <memory>
#include <type_traits>

namespace TowerDefence
{
    /**
     * @concept isDerived
     * @brief Концепт проверяет, что Derived является наследником Base.
     * @tparam Base Базовый класс.
     * @tparam Derived Проверяемый класс.
     */
    template<typename Base, typename Derived>
    concept isDerived = std::is_base_of_v<Base, Derived>;

    /**
     * @concept isFactory
     * @brief Концепт проверяет, что тип является фабрикой с методом create().
     * @tparam FactoryT Проверяемый тип фабрики.
     * @tparam IBuildResultT Ожидаемый тип результата построения.
     */
    template<typename FactoryT, typename IBuildResultT>
    concept isFactory = requires(FactoryT factory)
    {
       {factory.create()} -> std::same_as<std::unique_ptr<IBuildResultT>>;
    };

    /**
     * @concept isIDObjectFactory
     * @brief Концепт проверяет, что тип является фабрикой для объектов с ID.
     * @tparam FactoryT Проверяемый тип фабрики.
     * @tparam IBuildResultT Ожидаемый тип результата построения.
     */
    template<typename FactoryT, typename IBuildResultT>
    concept isIDObjectFactory = requires(FactoryT factory, ID id, Vector2<size_t> position, const IConfigReader& config)
    {
        {factory.create(position, id)} -> std::same_as<std::unique_ptr<IBuildResultT>>;
        {factory.restore(config)} -> std::same_as<std::unique_ptr<IBuildResultT>>;
    };

    /**
     * @concept isIDObjectWithLevelFactory
     * @brief Концепт проверяет, что тип является фабрикой для объектов с ID и уровнем.
     * @tparam FactoryT Проверяемый тип фабрики.
     * @tparam IBuildResultT Ожидаемый тип результата построения.
     */
    template<typename FactoryT, typename IBuildResultT>
    concept isIDObjectWithLevelFactory = requires(FactoryT factory, ID id, Vector2<size_t> position, Level level, const IConfigReader& config)
    {
        {factory.create(position, id, level)} -> std::same_as<std::unique_ptr<IBuildResultT>>;
        {factory.restore(config)} -> std::same_as<std::unique_ptr<IBuildResultT>>;
    };

    /**
     * @concept FactoryWithServiceReferences
     * @brief Концепт проверяет, что фабрика принимает ссылки на сервисы в конструкторе.
     * @tparam FactoryT Проверяемый тип фабрики.
     * @tparam Services Типы сервисов.
     */
    template<typename FactoryT, typename... Services>
    concept FactoryWithServiceReferences = requires(FactoryT factory, const IConfigReader& config, Services&... services)
    {
        FactoryT{config, services...};
    };

    /**
     * @class FactoryManager
     * @brief Менеджер фабрик для создания и восстановления игровых объектов.
     *
     * Управляет коллекцией фабрик, предоставляя единый интерфейс для создания
     * объектов различных типов с поддержкой dependency injection для фабрик.
     * @tparam IBuildResult Тип создаваемого объекта (интерфейс).
     * @tparam IFactoryT Тип фабрики (интерфейс).
     */
    template<typename IBuildResult, typename IFactoryT>
    class FactoryManager {
        std::unordered_map<std::string, std::unique_ptr<IFactoryT>> factories_;
    public:

        /**
         * @brief Создать объект указанного типа.
         * @tparam Args Типы аргументов для фабрики.
         * @param name Имя типа объекта.
         * @param args Аргументы для передачи в фабрику.
         * @return std::unique_ptr<IBuildResult> созданный объект.
         * @throws std::runtime_error если фабрика не найдена.
         */
        template<typename... Args>
        [[nodiscard]] std::unique_ptr<IBuildResult> create(const std::string& name, Args&& ...args) const
        {
            auto it = factories_.find(name);
            if (it == factories_.end()) {
                throw std::runtime_error("ObjectFactory not found in create(): " + name);
            }
            return it->second->create(std::forward<Args>(args)...);
        }

        /**
         * @brief Восстановить объект из конфигурации.
         * @param name Имя типа объекта.
         * @param cfg IConfigReader с данными для восстановления.
         * @return std::unique_ptr<IBuildResult> восстановленный объект.
         * @throws std::runtime_error если фабрика не найдена.
         */
        [[nodiscard]] std::unique_ptr<IBuildResult> restore(const std::string& name, const IConfigReader& cfg) const
        {
            auto it = factories_.find(name);
            if (it == factories_.end()) {
                throw std::runtime_error("ObjectFactory not found in restore(): " + name);
            }
            return it->second->restore(cfg);
        }

        /**
         * @brief Добавить фабрику с конфигурацией.
         * @tparam FactoryT Конкретный тип фабрики.
         * @param name Имя типа объекта.
         * @param cfg IConfigReader для инициализации фабрики.
         * @requires FactoryT должен быть наследником IFactoryT и удовлетворять концепту isFactory.
         */
        template<typename FactoryT>
        requires isDerived<IFactoryT, FactoryT> && isFactory<FactoryT, IBuildResult>
        void addFactory(const std::string& name, const IConfigReader& cfg)
        {
            factories_.emplace(name, std::make_unique<FactoryT>(cfg));
        }

        /**
         * @brief Добавить фабрику с конфигурацией и ссылками на сервисы.
         * @tparam FactoryT Конкретный тип фабрики.
         * @tparam Services Типы сервисов.
         * @param name Имя типа объекта.
         * @param cfg IConfigReader для инициализации фабрики.
         * @param services Ссылки на сервисы.
         * @requires FactoryT должен быть наследником IFactoryT, удовлетворять концепту isFactory
         *           и концепту FactoryWithServiceReferences.
         */
        template<typename FactoryT, typename... Services>
        requires isDerived<IFactoryT, FactoryT> && isFactory<FactoryT, IBuildResult> && FactoryWithServiceReferences<FactoryT, Services...>
        void addFactoryWithRefs(const std::string& name, const IConfigReader& cfg, Services& ... services)
        {
            factories_.emplace(name, std::make_unique<FactoryT>(cfg, services...));
        }

        /**
         * @brief Добавить фабрику для объектов с ID.
         * @tparam FactoryT Конкретный тип фабрики.
         * @tparam Args Типы аргументов для конструктора фабрики.
         * @param name Имя типа объекта.
         * @param args Аргументы для конструктора фабрики.
         * @requires FactoryT должен быть наследником IFactoryT и удовлетворять концепту
         *           isIDObjectFactory или isIDObjectWithLevelFactory.
         */
        template<typename FactoryT, typename... Args>
        requires isDerived<IFactoryT, FactoryT> && (isIDObjectFactory<FactoryT, IBuildResult> || isIDObjectWithLevelFactory<FactoryT, IBuildResult>)
        void addIDObjectFactory(const std::string& name, Args&& ...args)
        {
            factories_.emplace(name, std::make_unique<FactoryT>(std::forward<Args>(args)...));
        }
    };
}
