#pragma once
#include "../../interface/include/wall/i_wall_factory.h"


namespace TowerDefence
{
    /**
     * @brief Фабрика для создания стен.
     *
     * Реализует интерфейс IWallFactory.
     * Использует прототип (donor) для создания новых экземпляров.
     */
    class WallFactory final : public IWallFactory {
        std::unique_ptr<IWall> donor_; ///< Прототип стены для клонирования.

    public:
        /**
         * @brief Конструктор фабрики.
         * @param type Тип создаваемых стен.
         */
        explicit WallFactory(const UpgradableType& type);

        /**
         * @brief Создает новую стену в указанной позиции.
         * @param position Позиция для размещения стены.
         * @param id Уникальный идентификатор для новой стены.
         * @return unique_ptr на созданную стену.
         */
        std::unique_ptr<IWall> create(Vector2<size_t> position, ID id) override;

        /**
         * @brief Восстанавливает стену из конфигурации.
         * @param cfg Конфигурация с сохраненным состоянием стены.
         * @return unique_ptr на восстановленную стену.
         */
        std::unique_ptr<IWall> restore(const IConfigReader &cfg) override;
    };
}