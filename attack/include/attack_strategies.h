#pragma once

#include "timer.h"
#include "enemy/i_enemy_service.h"
#include "attack/i_attack_strategy.h"
#include "castle/i_castle_service.h"
#include "pathfinding/i_pathfinding_service.h"
#include "wall/i_wall_service.h"

namespace TowerDefence
{
    /**
     * @class TowerAttackStrategy
     * @brief Стратегия атаки для башен.
     *
     * Реализует логику атаки башен: поиск ближайшего врага в радиусе,
     * перезарядка между выстрелами, нанесение урона.
     */
    class TowerAttackStrategy final: public IAttackStrategy {
        IEnemyService& enemyService_;
        Timer timer_;
        size_t radius_;
        float damage_;
        float radiusMultiplier_ = 1;
        float damageMultiplier_ = 1;
    public:
        /**
         * @brief Конструктор стратегии атаки башен.
         * @param enemyService Ссылка на сервис врагов.
         * @param radius Радиус атаки.
         * @param damage Урон за атаку.
         * @param reloadTime Время перезарядки между атаками.
         */
        TowerAttackStrategy(IEnemyService& enemyService, size_t radius, float damage, float reloadTime):
        enemyService_(enemyService), timer_(reloadTime), radius_(radius), damage_(damage) {}

        /**
         * @brief Конструктор из конфигурации.
         * @param config IConfigReader с параметрами стратегии.
         * @param enemyService Ссылка на сервис врагов.
         */
        TowerAttackStrategy(const IConfigReader& config, IEnemyService& enemyService) : TowerAttackStrategy(enemyService, config.getSize_t("radius"), config.getFloat("damage"), config.getFloat("reload_time")) {}

        bool tryAttack(IIDObject& attacker, float dt) override;

        /**
         * @brief Умножить урон атаки.
         * @param multiplier Множитель урона.
         */
        void multiplyDamage(float multiplier) override {damageMultiplier_ = multiplier;}

        /**
         * @brief Умножить радиус атаки.
         * @param multiplier Множитель радиуса.
         */
        void multiplyRadius(float multiplier) override{radiusMultiplier_ = multiplier;}

        [[nodiscard]] std::unique_ptr<IAttackStrategy> clone() override {return std::make_unique<TowerAttackStrategy>(enemyService_, radius_, damage_, timer_.getReloadTime()); }
        void setFromConfig(const IConfigReader& cfg) override;
    private:
        /**
         * @brief Проверить возможность атаки.
         * @return true если атака возможна, false в противном случае.
         */
        [[nodiscard]] bool canAttack() const;

    public:
        void toConfig(const std::string &name, IConfigReader &config) override;
    };

    /**
     * @class RangeEnemyAttackStrategy
     * @brief Стратегия атаки для дальнобойных врагов.
     *
     * Реализует логику атаки врагов с дальним боем: выбор цели
     * (стены или замок), нанесение урона с учетом коэффициентов.
     */
    class RangeEnemyAttackStrategy final: public IAttackStrategy {
        IWallService& wallService_;
        ICastleService& castleService_;
        IPathfindingService& pathfinding_;
        Timer timer_;
        size_t radius_;
        ID currentTarget_ = 0;
        float damage_;
        float radiusMultiplier_ = 1;
        float damageMultiplier_ = 1;
        float castleCoefficient_;
    public:
        /**
         * @brief Конструктор стратегии атаки дальнобойных врагов.
         * @param wallService Ссылка на сервис стен.
         * @param pathfinding Ссылка на сервис поиска пути.
         * @param castleService Ссылка на сервис замка.
         * @param radius Радиус атаки.
         * @param damage Урон за атаку.
         * @param reloadTime Время перезарядки между атаками.
         * @param castleCoefficient Коэффициент урона по замку.
         */
        RangeEnemyAttackStrategy(IWallService& wallService, IPathfindingService& pathfinding, ICastleService& castleService, size_t radius, float damage, float reloadTime, float castleCoefficient):
        wallService_(wallService), castleService_(castleService), pathfinding_(pathfinding), timer_(reloadTime), radius_(radius), damage_(damage), castleCoefficient_(castleCoefficient) {}

        /**
         * @brief Конструктор из конфигурации.
         * @param config IConfigReader с параметрами стратегии.
         * @param wallService Ссылка на сервис стен.
         * @param pathfinding Ссылка на сервис поиска пути.
         * @param castleService Ссылка на сервис замка.
         */
        RangeEnemyAttackStrategy(const IConfigReader& config, IWallService& wallService, IPathfindingService& pathfinding, ICastleService& castleService): RangeEnemyAttackStrategy(wallService, pathfinding, castleService,  config.getSize_t("radius"), config.getFloat("damage"), config.getFloat("reload_time"), config.getFloat("castle_coefficient")) {}
        bool tryAttack(IIDObject& attacker, float dt) override;

        /**
         * @brief Умножить урон атаки.
         * @param multiplier Множитель урона.
         */
        void multiplyDamage(float multiplier) override {damageMultiplier_ = multiplier;}

        /**
         * @brief Умножить радиус атаки.
         * @param multiplier Множитель радиуса.
         */
        void multiplyRadius(float multiplier) override{radiusMultiplier_ = multiplier;}

        [[nodiscard]] std::unique_ptr<IAttackStrategy> clone() override {return std::make_unique<RangeEnemyAttackStrategy>(wallService_,pathfinding_,castleService_,  radius_, damage_, timer_.getReloadTime(), castleCoefficient_); }
        void setFromConfig(const IConfigReader& cfg) override;
        void toConfig(const std::string &name, IConfigReader &config) override;
    private:
        /**
         * @brief Проверить возможность атаки.
         * @return true если атака возможна, false в противном случае.
         */
        [[nodiscard]] bool canAttack() const;
    };

    /**
     * @class AirAttackStrategy
     * @brief Стратегия атаки для воздушных врагов.
     *
     * Реализует логику атаки воздушных врагов: игнорирование стен,
     * атака замка с учетом боезапаса и коэффициентов урона.
     */
    class AirAttackStrategy final: public IAttackStrategy {
        IWallService& wallService_;
        ICastleService& castleService_;
        Timer timer_;
        float castleCoefficient_;
        float damage_;
        float radiusMultiplier_ = 1;
        float damageMultiplier_ = 1;
        size_t ammo_;
    public:
        /**
         * @brief Конструктор стратегии атаки воздушных врагов.
         * @param wallService Ссылка на сервис стен.
         * @param castleService Ссылка на сервис замка.
         * @param damage Урон за атаку.
         * @param reloadTime Время перезарядки между атаками.
         * @param ammo Боезапас (количество атак).
         * @param castleCoefficient Коэффициент урона по замку.
         */
        AirAttackStrategy(IWallService& wallService, ICastleService& castleService, float damage, float reloadTime, size_t ammo, float castleCoefficient):
        wallService_(wallService), castleService_(castleService), timer_(reloadTime), damage_(damage), ammo_(ammo) , castleCoefficient_(castleCoefficient){}

        /**
         * @brief Конструктор из конфигурации.
         * @param config IConfigReader с параметрами стратегии.
         * @param wallService Ссылка на сервис стен.
         * @param castleService Ссылка на сервис замка.
         */
        AirAttackStrategy(const IConfigReader& config, IWallService& wallService, ICastleService& castleService): AirAttackStrategy(wallService, castleService, 0, 0, 0, 0){ setFromConfig(config);}
        bool tryAttack(IIDObject& attacker, float dt) override;

        /**
         * @brief Умножить урон атаки.
         * @param multiplier Множитель урона.
         */
        void multiplyDamage(float multiplier) override {damageMultiplier_ = multiplier;}

        /**
         * @brief Умножить радиус атаки.
         * @param multiplier Множитель радиуса.
         */
        void multiplyRadius(float multiplier) override{radiusMultiplier_ = multiplier;}

        void setFromConfig(const IConfigReader& cfg) override;
    private:
        /**
         * @brief Проверить возможность атаки.
         * @return true если атака возможна, false в противном случае.
         */
        [[nodiscard]] bool canAttack() const;

    public:
        [[nodiscard]] std::unique_ptr<IAttackStrategy> clone() override {return std::make_unique<AirAttackStrategy>(wallService_, castleService_, damage_, timer_.getReloadTime(), ammo_, castleCoefficient_);}

        void toConfig(const std::string &name, IConfigReader &config) override;
    };
}