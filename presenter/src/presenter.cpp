#include "presenter.h"

#include <fstream>
#include <iostream>

using namespace TowerDefence;
void Presenter::present() // sfml, fuck you (n times as ifs count), why there is no event.type() method ..............
{
    MenuState state = MenuState::MENU;
    while (state != MenuState::EXIT) {
        view_.clear();
        while (handleMenu(state)) {
            drawMenu();
            view_.drawAll();
            view_.show();
        }
        if (state == MenuState::EXIT) {
            return;
        }

        if (state == MenuState::LOAD) {
            view_.clear();
            drawTileMap();
            drawPanel();
            model_.load();
        }

        if (state == MenuState::START || state == MenuState::LOAD) {
            if (state == MenuState::START) {
                view_.clear();
                drawTileMap();
                drawPanel();
            }

            sf::Clock clock;
            Timer timer{10000};

            auto delta = 1 / 60.f;
            auto finishTime = 0;

            size_t fpassed = 0;
            while (handleEvents(state)) {
                //int c = 0;
                // for (size_t x = 0; x < model_.world_.size().getX(); ++x) {
                //     for (size_t y = 0; y < model_.world_.size().getY(); ++y, ++c) {
                //         if (c % 2 == 0 && model_.world_.getTile(x,y) == TileType::LAND) {
                //             auto res = model_.towers_.createTower("basic_tower", {x, y});
                //             model_.enemies_.createEnemy("archer", {x, y});
                //         }
                //         else if (c % 2 == 0){
                //             model_.walls_.createWall("basic_wall", {x, y});
                //         }
                //     }
                // }
                if (!model_.castle_.isAlive() && !finishTime) {
                    finishTime = timer.getPassed();
                }
                timer.update(delta);
                if (finishTime > 0) {
                    view_.pushText({700, 540}, 50, "GAME OVER!\nYOU LOOSE!!!");
                }
                if (timer.getPassed() - finishTime > 5 && finishTime > 0) {
                    state = MenuState::MENU;
                    view_.stateSelectedPos(false);
                    view_.clear();
                    model_.reset();
                    break;
                }
                auto passed = static_cast<size_t>(timer.getPassed());
                if (fpassed != passed) {
                    fpassed = passed;
                    if (fpassed % 3 == 0) {
                        model_.enemies_.createEnemy("warrior", {9, 9});

                    }
                    if (fpassed % 5 == 0) {
                        model_.enemies_.createEnemy("archer", {9, 0});
                    }
                    if (fpassed % 5 == 0) {
                        model_.enemies_.createEnemy("balloon", {5, 0});
                    }
                    // if (fpassed % 25 == 0) {
                    //     model_.enemies_.createEnemy("archer_boss", {9, 9});
                    // }
                    //if (fpassed % 60 == 0) {
                    //    model_.towers_.forEach([&](const auto& tower)
                    //    {
                    //       model_.towers_.destroyTower(tower.getPosition());
                    //    });
                    //}
                }

                auto timeString = std::format("Time: {}:{}", passed / 60, passed % 60);
                view_.pushText({10, 50}, 20, timeString);

                model_.pathfindingService_.update();
                model_.walls_.update(delta);
                model_.towers_.update(delta);
                model_.castle_.update(delta);
                model_.enemies_.update(delta);

                auto killedWalls = model_.walls_.collectDestroyed();
                auto killedTowers = model_.towers_.collectDestroyed();
                auto killedEnemies = model_.enemies_.collectDestroyed();


                drawCastle(delta);
                drawWalls(killedWalls, delta);
                drawTowers(killedTowers, delta);
                drawEnemies(killedEnemies, delta);

                drawFps(delta);
                drawMoney();

                view_.drawAll();
                view_.show();
                clock.restart();
            }
        }
    }
}
//}


void Presenter::drawTileMap()
{
    auto size = model_.world_.size();
    size_t id = 0;
    auto ts = view_.getTileSize();
    for (auto x = 0; x < size.getX(); ++x) {
        for (auto y = 0; y < size.getY(); ++y) {
            auto tile = model_.world_.getTile(y,x);
            if (tile == TileType::CASTLE) {
                view_.pushImage(typeTileMap.at(TileType::LAND), {static_cast<float>(ts/2 + x * ts) , static_cast<float>(ts/2+ y * ts) }, {ts , ts} , 1, 1, id++);
            }
            view_.pushImage(typeTileMap.at(tile), {static_cast<float>(ts/2 + x * ts) , static_cast<float>(ts/2+ y * ts) }, {ts , ts} , 1, 1, id++);

        }
    }
}

void Presenter::drawPanel()
{
    auto start = view_.getPanelBorders().first;
    auto finish = view_.getPanelBorders().second;

    auto center = start + finish;
    center.setX(center.getX() / 2);
    center.setY(center.getY() / 2);
    auto sfmlCenter = Vector2{center.getY(), center.getX()};
    auto xSize = finish.getY() - start.getY();
    auto ySize = finish.getX() - start.getX();

    for (auto& [num, name] : buildings) {
        auto pictureCenter = Vector2<float>{static_cast<float>(sfmlCenter.getX()), static_cast<float>(num * view_.getPanelButtonSize() - view_.getPanelButtonSize() / 2)};
        view_.pushImage(name, pictureCenter, {static_cast<unsigned long>(xSize * 0.9), static_cast<unsigned long>(ySize/buildings.size() * 0.9)}, 1 , 1);
    }
}

void Presenter::drawWalls(const std::vector<std::pair<ID, std::string>> & killed, float dt)
{
    for (auto& [id, name] : killed) {
        view_.kill(name, id);
        drawHP(0, 0, {view_.getTileSize(), view_.getTileSize()}, id);
    }
    std::vector<std::tuple<ID, Vector2<size_t>, std::string, float, float>> walls;
    model_.walls_.forEach([&](auto& wall)
    {
        walls.emplace_back(wall.getID(), wall.getPosition(), wall.getUpgradableType().getName(), wall.getHealth(), wall.getMaxHealth());
    });

    for (auto& [id, position, name, health, maxHealth] : walls) {
        position.setX(position.getX() * view_.getTileSize());
        position.setY(position.getY() * view_.getTileSize());
        auto ts = view_.getTileSize();
        drawHP(health, maxHealth, {position.getY(), position.getX()}, id);
        if (view_.contains(name, id)) {
            view_.update(name, id, dt);
            continue;
        }
        view_.pushImage(name, {static_cast<float>(position.getY() + ts/2), static_cast<float>(position.getX() +ts/2)}, {ts, ts}, 1 , 1, id);

    }
}

void Presenter::drawTowers(const std::vector<std::pair<ID, std::string>> &killed, float dt)
{
    for (auto& [id, name] : killed) {
        view_.kill(name, id);

    }
    std::vector<std::tuple<ID, Vector2<size_t>, std::string>> towers;
    model_.towers_.forEach([&](auto& tower)
    {
        towers.emplace_back(tower.getID(), tower.getPosition(), tower.getUpgradableType().getName());
    });

    for (auto& [id, position, name] : towers) {
        position.setX(position.getX() * view_.getTileSize());
        position.setY(position.getY() * view_.getTileSize());
        auto ts = view_.getTileSize();
        if (view_.contains(name, id)) {
            view_.update(name, id, dt);
            continue;
        }
        view_.pushImage(name, {static_cast<float>(position.getY() + ts/2), static_cast<float>(position.getX() + ts/2)}, {ts, ts}, 1 , 1, id);
    }
}

void Presenter::drawEnemies(const std::vector<std::pair<ID, std::string>> &killed, float dt)
{

    for (auto& [id, name] : killed) {
        view_.kill(name, id);
        drawHP(0, 0, {view_.getTileSize(), view_.getTileSize()}, id);
    }

    std::vector<std::tuple<ID, Vector2<size_t>, std::string, float, float, Vector2<int>, float>> enemies;
        model_.enemies_.forEach([&](auto& enemy)
        {
            enemies.emplace_back(enemy.getID(), enemy.getPosition(), enemy.getType().getName(), enemy.getHealth(), enemy.getMaxHealth(), enemy.getSightVector(), enemy.getDs());
        });

    auto ts = view_.getTileSize();
    for (auto& [id, position, name, health, maxHealth, sv, ds] : enemies) {
        auto length = Vector2<float>::distance({0, 0}, sv);
        auto windowX = position.getY() * ts;
        auto windowY = position.getX() * ts;
        float fac = length > 1 ? 14 : 10;

        auto normDs = (ds / fac) * ts;

        auto normSV = Vector2<float>{static_cast<float>(sv.getY()), static_cast<float>(sv.getX())};

        auto dx = normSV.getX() * normDs;
        auto dy = normSV.getY() * normDs;


        view_.pushImage(name,{windowX + dx, windowY + dy}, {ts, ts}, 1 , 1, id);
        drawHP(health, maxHealth, {static_cast<unsigned long>(windowX + dx), static_cast<unsigned long>(windowY + dy)}, id);

    }

}

void Presenter::drawCastle(float dt)
{
    const auto& castle = model_.castleRepository_.getCastle();
    auto id = castle.getID();
    auto health = castle.getHealth();
    auto maxHealth = castle.getMaxHealth();
    auto position = castle.getPosition();
    auto ts = view_.getTileSize();
    drawHP(health, maxHealth, {position.getY() * ts, position.getX() * ts}, id);
    if (view_.contains("castle", id)) {
        view_.update("castle", id, dt);
    }
}

void Presenter::drawHP(float health, float maxHealth, Vector2<size_t> pos, ID id)
{
    auto ts = view_.getTileSize();
    int tiles = static_cast<int>(health / maxHealth * 10);
    std::string text = std::to_string(static_cast<unsigned>(health)) + "/" + std::to_string(static_cast<unsigned>(maxHealth));
    view_.pushText(pos, 10, text);

    if (tiles == 0) {
        for (int i = 0; i < 10; ++i) {
            if (view_.contains("health_bar_tile", 10*id+i)) {
                view_.kill("health_bar_tile", 10*id+i);
            }
        }
        return;
    }

    for (int i = 0; i < 10; ++i) {
        if (tiles <= i ) {
            view_.kill("health_bar_tile", id*10 + i);
            continue;
        }
        auto position = pos;
        position.setX(pos.getX() + i* ts/20);
        position.setY(pos.getY() );
        view_.pushImage("health_bar_tile", {static_cast<float>(position.getX()), static_cast<float>(position.getY())}, {ts / 20, ts / 10}, 1, 1, 10*id+i);
    }
}

void Presenter::drawMenu()
{
    constexpr auto startStr = "Start";
    constexpr auto loadStr = "Load";
    constexpr auto exitStr = "Exit";


    unsigned long width = view_.size().x;
    unsigned long height = view_.size().y;

    view_.pushImage("menu_background", {static_cast<float>(width/2), static_cast<float>(height/2)}, {width, height}, 1, 1, 0);

    view_.pushText({2*width/5, 2*height/5}, 75, startStr);
    view_.pushText({2*width/5, 2*height/5 + 150}, 75, loadStr);
    view_.pushText({2*width/5, 2*height/5 + 300}, 75, exitStr);

}

bool Presenter::handleMenu(MenuState& exit)
{
    while (auto event = view_.getEvent()) {
        if (event->is<sf::Event::Closed>()) {
            handleClose(*event->getIf<sf::Event::Closed>());
            exit = MenuState::EXIT;
        }
        if (event->is<sf::Event::MouseButtonPressed>()) {
            handleMenuMouseClick(*event->getIf<sf::Event::MouseButtonPressed>(), exit);
        }
    }

    return exit == MenuState::MENU;
}

void Presenter::handleMenuMouseClick(const sf::Event::MouseButtonPressed &buttonEvent, MenuState& exit)
{
    auto pos = buttonEvent.position;

    constexpr auto offset = 75;
    float w = view_.size().x * 2/5;
    float h = view_.size().y * 2/5;

    auto buttonStart = std::make_pair(Vector2<float>{w, h}, Vector2<float>{w + 3*offset, h+offset});
    auto buttonLoad = std::make_pair(Vector2<float>{w, h+2*offset}, Vector2<float>{w + 3*offset, h+3*offset});
    auto buttonExit = std::make_pair(Vector2<float>{w, h+4*offset}, Vector2<float>{w + 3*offset, h+5*offset});
    if (Vector2<int>{pos.x, pos.y}.in(buttonStart.first, buttonStart.second)) {
        view_.pushText({1,1}, 40, "pressed start");
        exit = MenuState::START;
    }
    if (Vector2<int>{pos.x, pos.y}.in(buttonLoad.first, buttonLoad.second)) {
        view_.pushText({1,1}, 40, "pressed load");
        exit = MenuState::LOAD;
    }
    if (Vector2<int>{pos.x, pos.y}.in(buttonExit.first, buttonExit.second)) {
        view_.pushText({1,1}, 40, "pressed exit");
        exit = MenuState::EXIT;
    }

}

void Presenter::drawFps(float dt)
{
    auto FPS = static_cast<unsigned>(1 / dt);
    view_.pushText({10, 10}, 20, "FPS: " + std::to_string(FPS));
}

void Presenter::drawMoney()
{
    auto money = model_.economyService_.balance();
    view_.pushText({10, 30}, 20, "Money: " + std::to_string(money));
}

Model::Model(IConfigReader& e, IConfigReader& wSave, IConfigReader& t, IConfigReader& c, IConfigReader& economySave, IConfigReader& idSave, IEnemyService &es, IWallService &ws, ITowerService &ts, ICastleService &cs,
    const IWorldMap &w, const IEnemyRepository &enemiesRepository_, const IWallRepository &wallRepository_,
    const ITowerRepository &towerRepository_, const ICastleRepository &castleRepository_,
    IEconomyService &economyService_, IPathfindingService &pathfinding, IIDService& idService): enemySave_(e), wallSave_(wSave), towerSave_(t), castleSave_(c), economySave_(economySave), idSave_(idSave), enemies_(es), walls_(ws), towers_(ts), castle_(cs), enemiesRepository_(enemiesRepository_), wallRepository_(wallRepository_), towerRepository_(towerRepository_), castleRepository_(castleRepository_)
                                                                        , world_(w), idService_(idService), economyService_(economyService_), pathfindingService_(pathfinding)
{}

void Model::reset()
{
    enemies_.reset();
    walls_.reset();
    towers_.reset();
    castle_.reset();
    economyService_.reset();
}

void Model::save()
{
    enemySave_.clear();
    wallSave_.clear();
    towerSave_.clear();
    castleSave_.clear();

    enemies_.toConfig(enemySave_);
    writeConfig(enemySave_);

    walls_.toConfig(wallSave_);
    writeConfig(wallSave_);

    towers_.toConfig(towerSave_);
    writeConfig(towerSave_);

    castle_.toConfig(castleSave_);
    writeConfig(castleSave_);

    idService_.toConfig(idSave_);
    writeConfig(idSave_);

    economyService_.toConfig(economySave_);
    writeConfig(economySave_);
}

void Model::load()
{
    enemies_.load(enemySave_);
    walls_.load(wallSave_);
    towers_.load(towerSave_);
    castle_.load(castleSave_);
    idService_.setFromConfig(idSave_);
    economyService_.setFromConfig(economySave_);
}

void Model::writeConfig(IConfigReader &config)
{
    std::ofstream file{config.getPath()};
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file " + config.getPath());
    }
    file << config.dump();
    file.close();
}

//choose then act
// состояние которое могло быть (1)уже выбрана клетка и нажимаем че то на клавиатуре (2) уже выбрана постройка и тыкаем на клетку
//
TileContext Presenter::handleTileTap(const TileEvent& event)
{
    if (isTileActivated_) {
        activatedTileType_ = model_.world_.getTile(event.mouseClickPosition.getX(), event.mouseClickPosition.getY());

        if (event.isUpgradeKeyPressed && activatedTileType_ == TileType::TOWER) {
            auto ids = model_.towers_.getInRadius(activeTile_, 0);
            if (!ids.empty())
                return {model_.towers_.upgradeTower(ids[0]), ids[0], TileOption::Upgrade};
        }
        if (event.isUpgradeKeyPressed && activatedTileType_ == TileType::WALL) {
            auto ids = model_.walls_.getInRadius(activeTile_, 0);
            if (!ids.empty())
                return {model_.walls_.upgradeWall(ids[0]), ids[0], TileOption::Upgrade};
        }
        if (event.isUpgradeKeyPressed && activatedTileType_ == TileType::CASTLE) {
            auto id = model_.castle_.getCastleID();
                return {model_.castle_.upgradeCastle(), id, TileOption::Upgrade};
        }
        if (event.isDestroyKeyPressed && activatedTileType_ == TileType::TOWER) {

            auto ids = model_.towers_.getInRadius(activeTile_, 0);
            if (!ids.empty())
                return {model_.towers_.destroyTower(activeTile_), ids[0], TileOption::Destroy};
        }
        if (event.isDestroyKeyPressed && activatedTileType_ == TileType::WALL) {

            auto ids = model_.walls_.getInRadius(activeTile_, 0);
            if (!ids.empty()) {
                auto status = model_.walls_.destroyWall(activeTile_);

                return TileContext{status, ids[0], TileOption::Destroy};
            }
        }
        return {};
    }


    if (activatedBuildingType_ && isTower(activatedBuildingType_)) {

        auto status =  model_.towers_.createTower(buildings.at(activatedBuildingType_), event.mouseClickPosition);
        if (status) {
            auto ids = model_.towers_.getInRadius(event.mouseClickPosition, 0);
            return {status, ids[0], TileOption::Create};
        }
        isTileActivated_ = false;
        activatedBuildingType_ = 0;
        return {};
    }
    if (activatedBuildingType_ && isWall(activatedBuildingType_)) {
        auto status = model_.walls_.createWall(buildings.at(activatedBuildingType_), event.mouseClickPosition);
        if (status) {
            auto ids = model_.walls_.getInRadius(event.mouseClickPosition, 0);
            return {status, ids[0], TileOption::Create};
        }
        isTileActivated_ = false;
        activatedBuildingType_ = 0;
        return {};
    }
    isTileActivated_ = true;
    activeTile_ = event.mouseClickPosition;

    return {};
}

void Presenter::handleMouseClick(const sf::Event::MouseButtonPressed &buttonEvent)
{
    auto sfmlVec = buttonEvent.position;
    auto position = Vector2<size_t>(sfmlVec.y, sfmlVec.x);
    auto [leftUp, rightDown] =  view_.getPanelBorders();
    auto [leftUpTile, rightDownTile] =  view_.getTileMapBorders();

    //panel
    if ( position.in(leftUp, rightDown)) {
        activatedBuildingType_ = position.getX() / view_.getPanelButtonSize() + 1;
        //view.draw_border may be like this idn
    }
    //tilemap
    else if ( position.in(leftUpTile, rightDownTile) ) {
        auto modelPosition = Vector2(position.getX() / view_.getTileSize(), position.getY() / view_.getTileSize());
        TileEvent event  = {modelPosition, false, false};
        isTileActivated_ = activatedBuildingType_ == 0;
        activeTile_ = event.mouseClickPosition;

        auto tilePos = Vector2<size_t>{static_cast<unsigned>(buttonEvent.position.x), static_cast<unsigned>(buttonEvent.position.y)};
        view_.setSelectedTilePos(activeTile_);
        view_.stateSelectedPos(true);
        handleTileTap(event);

        activatedBuildingType_ = 0;
        //view_.stateSelectedPos(false);
    }
}

void Presenter::handleMouseScroll(const sf::Event::MouseWheelScrolled &mouseEvent)
{
    if (mouseEvent.wheel == sf::Mouse::Wheel::Vertical) {
        view_.zoomView(mouseEvent.delta);
    }
}

void Presenter::handleKeyPress(const sf::Event::KeyPressed &keyEvent)
{
    auto code = keyEvent.code;


    TileEvent event{activeTile_, false, false};
    int n = 10;
    switch (code) {
        case sf::Keyboard::Key::U:
            if (isTileActivated_) event.isUpgradeKeyPressed = true;
            break;
        case sf::Keyboard::Key::D:
            if (isTileActivated_) event.isDestroyKeyPressed = true;
            break;
        case sf::Keyboard::Key::A:
            if (isTileActivated_)
                for (int i = 0; i < n; ++i)
                    model_.enemies_.createEnemy("archer", activeTile_);
            break;
        case sf::Keyboard::Key::B:
            if (isTileActivated_)
                for (int i = 0; i < n; ++i)
                    model_.enemies_.createEnemy("balloon", activeTile_);
            break;
        case sf::Keyboard::Key::K:
            if (isTileActivated_)
                for (int i = 0; i < n; ++i)
                    model_.enemies_.createEnemy("archer_boss", activeTile_);
            break;
        case sf::Keyboard::Key::W:
            if (isTileActivated_)
                for (int i = 0; i < n; ++i)
                    model_.enemies_.createEnemy("warrior", activeTile_);
            break;
        case sf::Keyboard::Key::E:
            static bool type = false;
            if (type)
                view_.setZoomedView();
            else {
                view_.setPanelView();
            }
            type = !type;
            break;
        default:
            break;
    }

    auto ctx = handleTileTap(event);
    if (ctx.status == false && (ctx.option != TileOption::None)) {
        view_.stateSelectedPos(true, sf::Color::Red);
    }
    if (ctx.status == true) {
        switch (ctx.option) {
            default:
                break;
            case TileOption::Destroy:
                view_.stateSelectedPos(true, sf::Color::Yellow);
                break;
            case TileOption::Upgrade:
                view_.stateSelectedPos(true, sf::Color::Blue);
                break;

        }
    }
}

bool Presenter::handleEvents(MenuState& state)
{
    while (auto event = view_.getEvent()) {
        if (event->is<sf::Event::Closed>()) {
            handleClose(*event->getIf<sf::Event::Closed>());
            state = MenuState::MENU;
            view_.clear();
            view_.stateSelectedPos(false);
            model_.save();
            model_.reset();
            return false;
        }
        if (event->is<sf::Event::KeyPressed>()) {
            handleKeyPress(*event->getIf<sf::Event::KeyPressed>());
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            handleMouseClick(*event->getIf<sf::Event::MouseButtonPressed>());
        }
        else if (event->is<sf::Event::MouseWheelScrolled>()) {
            handleMouseScroll(*event->getIf<sf::Event::MouseWheelScrolled>());
        }

    }
    return true;

}

void Presenter::handleClose(const sf::Event::Closed &closed)
{
    std::cout << "Closing..." << std::endl;
}