#pragma once
#include "enemy/i_enemy_repository.h"

namespace TowerDefence
{
    /**
     * @brief Репозиторий для хранения и управления врагами и их типами.
     *
     * Реализует интерфейс IEnemyRepository.
     * Хранит все живые экземпляры врагов и коллекцию типов врагов (MovableType).
     */
    class EnemyRepository final: public IEnemyRepository {
        std::unordered_map<ID, std::unique_ptr<IEnemy>> allEnemies_; ///< Хранилище всех врагов, ключ - ID врага.
        std::vector<MovableType> enemyTypes_; ///< Коллекция типов врагов.

    public:
        /**
         * @brief Конструктор репозитория.
         * @details Инициализирует коллекцию типов врагов из общей конфигурации.
         * @param allEnemyConfigs Конфигурация, содержащая описания всех типов врагов.
         */
        explicit EnemyRepository(const IConfigReader& allEnemyConfigs);

        /**
         * @brief Добавляет новый тип врага в репозиторий.
         * @param name Имя типа.
         * @param config Конфигурация для создания типа.
         */
        void addType(const std::string &name, const IConfigReader &config) override;

        /**
         * @brief Возвращает константную ссылку на тип врага по его имени.
         * @param name Имя типа.
         * @return Константная ссылка на MovableType.
         * @throws std::out_of_range если тип с таким именем не найден.
         */
        [[nodiscard]] const MovableType & getTypeRef(const std::string &name) const override;

        /**
         * @brief Возвращает ссылку на врага по его ID.
         * @param id Уникальный идентификатор врага.
         * @return Ссылка на объект врага.
         * @throws std::out_of_range если враг с таким ID не найден.
         */
        [[nodiscard]] IEnemy & getEnemy(ID id) const override;

        /**
         * @brief Выполняет callback-функцию для каждого врага в репозитории.
         * @param callback Функция, принимающая ссылку на IEnemy.
         */
        void forEach(std::function<void(IEnemy &)> callback) const override;

        /**
         * @brief Добавляет нового врага в репозиторий.
         * @param id Уникальный идентификатор для врага.
         * @param enemy unique_ptr на объект врага.
         * @throws std::invalid_argument если враг с таким ID уже существует.
         */
        void addEnemy(ID id, std::unique_ptr<IEnemy> enemy) override;

        /**
         * @brief Удаляет врага из репозитория.
         * @param id Уникальный идентификатор врага для удаления.
         */
        void removeEnemy(ID id) override;
    };
}