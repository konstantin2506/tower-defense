#pragma once

#include "ability/i_ability.h"
#include "effects/i_effect.h"
#include "enemy/i_enemy_service.h"
#include "type_names.h"

#include <algorithm>

#include "timer.h"
#include "world_map/i_world_map.h"

namespace TowerDefence
{
    /**
     * @class DodgeAbility
     * @brief Способность уклонения для врагов.
     *
     * Позволяет врагу временно уменьшать получаемый урон
     * с определенной вероятностью.
     */
    class DodgeAbility final: public IAbility {

        Timer timer_;
        float probability;
        IEnemyService& enemyService_;

    public:
        /**
         * @brief Конструктор способности уклонения.
         * @param probability Вероятность срабатывания уклонения (0-1).
         * @param enemyService Ссылка на сервис врагов.
         */
        DodgeAbility(float probability, IEnemyService& enemyService) :
        timer_(1), probability(probability), enemyService_(enemyService) {}

        /**
         * @brief Конструктор из конфигурации.
         * @param config IConfigReader с параметрами способности.
         * @param enemyService Ссылка на сервис врагов.
         */
        DodgeAbility(const IConfigReader& config, IEnemyService& enemyService) : DodgeAbility(config.getFloat("probability"), enemyService){}

        void tryUseAbility([[maybe_unused]]float dt, IEnemy& enemy) override; // es.addEffect(owner, std::make_unique<AbsorbedDamageEffect<IEnemy>>(1, 0)) на одну секунду уменьшаем получаемый урон в 0
        [[nodiscard]] std::unique_ptr<IAbility> clone() const override {return std::make_unique<DodgeAbility>(*this);}

        void setFromConfig(const IConfigReader &config) override;
    };

    /**
     * @class DieDropAbility
     * @brief Способность создания врагов при смерти.
     *
     * При смерти владельца создает новых врагов на его позиции.
     */
    class DieDropAbility final: public IAbility { // сборка мусора должна происходитьт в начале кадра за предыдущий кадр
        std::vector<std::string> enemiesInside_;
        IEnemyService& enemyService_;
        const IWorldMap& world_;
    public:
        /**
         * @brief Конструктор способности создания при смерти.
         * @param enemiesInside Типы врагов, которые создаются при смерти.
         * @param enemyService Ссылка на сервис врагов.
         * @param world Ссылка на игровую карту.
         */
        DieDropAbility(std::vector<std::string> enemiesInside, IEnemyService& enemyService, const IWorldMap& world) :
        enemiesInside_(std::move(enemiesInside)), enemyService_(enemyService), world_(world) {}

        /**
         * @brief Конструктор из конфигурации.
         * @param config IConfigReader с параметрами способности.
         * @param enemyService Ссылка на сервис врагов.
         * @param world Ссылка на игровую карту.
         */
        DieDropAbility(const IConfigReader& config, IEnemyService& enemyService, const IWorldMap& world) : DieDropAbility(std::vector<std::string>{}, enemyService, world)
        {
            setFromConfig(config);
        }

        void tryUseAbility([[maybe_unused]]float dt, IEnemy& owner) override; //getpos, use factory
        [[nodiscard]] std::unique_ptr<IAbility> clone() const override {return std::make_unique<DieDropAbility>(*this);}

        void setFromConfig(const IConfigReader &config) override;
    };

    /**
     * @concept CanAddEffect
     * @brief Концепт проверяет, что сервис может добавлять эффекты.
     * @tparam ServiceT Тип сервиса.
     * @tparam OwnerT Тип объекта-владельца эффекта.
     */
    template<typename ServiceT, typename OwnerT>
    concept CanAddEffect = requires(ServiceT service, std::unique_ptr<IEffect<OwnerT>> effect, ID id)
    {
        service.addEffect(id, std::move(effect));
    };

    /**
     * @concept CanGetInRadius
     * @brief Концепт проверяет, что сервис может получать объекты в радиусе.
     * @tparam ServiceT Тип сервиса.
     */
    template<typename ServiceT>
    concept CanGetInRadius = requires(ServiceT service, Vector2<int> point, float radius)
    {
        service.getInRadius(point, radius);
    };

    /**
     * @class AuraAbility
     * @brief Способность ауры, которая воздействует на объекты в радиусе.
     *
     * Применяет эффект к объектам в определенном радиусе от владельца
     * с заданным интервалом времени.
     * @tparam OwnerT Тип владельца ауры.
     * @tparam ServiceT Тип сервиса для объектов, на которые воздействует аура.
     * @tparam EffectT Тип эффекта, который применяет аура.
     * @requires ServiceT должен удовлетворять концептам CanAddEffect и CanGetInRadius.
     */
    template<typename OwnerT,  typename ServiceT, typename EffectT>
    requires CanAddEffect<ServiceT, OwnerT> && CanGetInRadius<ServiceT>
    class AuraAbility final: public IAbility {
        size_t radius_;
        Timer timer_;
        std::unique_ptr<EffectT> effect_;
        IEnemyService& auraMakersService;
        ServiceT& clientService; // this may be used if boss add debuff to towers

    public:
        /**
         * @brief Конструктор способности ауры.
         * @param radius Радиус действия ауры.
         * @param es Ссылка на сервис врагов (владельцев ауры).
         * @param clientService Ссылка на сервис объектов, на которые воздействует аура.
         * @param cooldown Время перезарядки ауры.
         */
        AuraAbility(size_t radius, IEnemyService& es, ServiceT& clientService, const float cooldown) :
        radius_(radius), timer_(cooldown),/* effect_(std::move(effect)),*/ effect_(std::make_unique<EffectT>()), auraMakersService(es), clientService(clientService) {}

        /**
         * @brief Конструктор из конфигурации.
         * @param config IConfigReader с параметрами способности.
         * @param es Ссылка на сервис врагов.
         * @param clientService Ссылка на сервис объектов.
         */
        AuraAbility(const IConfigReader& config,  IEnemyService& es, ServiceT& clientService) :
        AuraAbility(0, es, clientService, 0)
        {
            setFromConfig(config);
        }

        /**
         * @brief Попытаться использовать ауру.
         * @param dt Время, прошедшее с предыдущего кадра.
         * @param owner Враг-владелец ауры.
         */
        void tryUseAbility([[maybe_unused]]float dt, IEnemy& owner) override
        {
            timer_.update(dt);
            if (timer_.isPassed()){

                std::vector<ID> clients = clientService.getInRadius(owner.getPosition(), radius_);

                std::ranges::for_each(clients, [&](auto id)
                {
                    if (id != owner.getID()) {
                        clientService.addEffect(id, effect_->clone());
                    }
                });
                timer_.reset();
            }
        }

        /**
         * @brief Создать копию способности.
         * @return std::unique_ptr<IAbility> клонированная способность.
         */
        [[nodiscard]] std::unique_ptr<IAbility> clone() const override
        {
            auto ability =  std::make_unique<AuraAbility>(radius_, auraMakersService, clientService, timer_.getReloadTime());
            auto cloned_effect = effect_->clone();
            ability->effect_ = std::unique_ptr<EffectT>(static_cast<EffectT*>(cloned_effect.release()));
            return ability;
        }

        /**
         * @brief Настроить способность из конфигурации.
         * @param config IConfigReader с параметрами способности.
         */
        void setFromConfig(const IConfigReader &config) override
        {
            radius_ = config.getSize_t("radius");
            timer_.setReloadTime(config.getFloat("cooldown"));
            effect_->setFromConfig(*config.getNested("effect"));
        }
    };
}