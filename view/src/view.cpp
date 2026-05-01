#include "view.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <utility>
#include <utility>
#include <utility>
#include <SFML/Graphics/Texture.hpp>

#include "config/i_config_reader.h"

TowerDefence::View::View(const IConfigReader& cfg, const IWorldMap& worldMap, const std::unordered_map<size_t, std::string>& buildings)
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned width = desktopMode.size.x;
    unsigned height = desktopMode.size.y;


    window_ = {sf::VideoMode({width, height}), "My window", sf::State::Fullscreen};
    window_.setVerticalSyncEnabled(true);


    view_ =  sf::View{sf::FloatRect({0.f, 0.f}, {static_cast<float>(height), static_cast<float>(height)})};
    window_.setView(view_);
    view_.setViewport(sf::FloatRect({0.f, 0.f}, {1200.f / 1920.f, 1.f}));

    view_.zoom(1.f);
    setPanelView();


    panelBorders_ = {Vector2<size_t>(0, height), Vector2<size_t>(height, width)};

    if (!font_.openFromFile(cfg.getString("font"))) {
        throw std::runtime_error("Could not open font file");
    }

    tileMapBorders_ = {Vector2<size_t>(0, 0), Vector2<size_t>(height, height)};
    tileSize_ = height / worldMap.size().getY();
    selectedTile_ = {{0,0}, tileSize_};

    mapSizeInCells_ = worldMap.size().getY();
    buttonsInPanel_ = buildings.size();
    panelButtonSize_ = height / buttonsInPanel_;

    for (const auto& [typeName, type] : drawTypeMap ) {
        for (auto&& [name, path] : cfg.getStrings(typeName)) {
            sf::Texture texture;
            if (!texture.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            addRenderer(name, std::move(texture), type);
        }
    }
}

bool TowerDefence::View::contains(const std::string &name, ID id) const
{
    return renderers_.contains(name) && renderers_.at(name).contains(id);
}

void TowerDefence::View::update(const std::string &name, ID id, float dt)
{
    renderers_.at(name).update(dt);
}

void TowerDefence::View::kill(const std::string &name, ID id)
{
    renderers_.at(name).removeObject(id);
}

void TowerDefence::View::addRenderer(const std::string &name, sf::Texture&& texture, DrawType type)
{
    renderers_.emplace(name, Renderer(std::move(texture), type));
}

void TowerDefence::View::drawTexts()
{
    std::ranges::for_each(texts_, [&](auto& text){window_.draw(text);});
    texts_.clear();
}

void TowerDefence::View::pushImage(const std::string& name, Vector2<float> imagePosition, Vector2<size_t> imageSize, size_t maxFrames, float frameTime, ID id)
{
    if (renderers_.at(name).getDrawType() == DrawType::DYNAMIC && contains(name, id)) {
        renderers_.at(name).replaceObject(id, imagePosition);
        return;
    }
    renderers_.at(name).addObject(id, imageSize, imagePosition, maxFrames, frameTime);
}

void TowerDefence::View::drawAll()
{
    window_.clear(sf::Color::Black);

    std::array order = {
        DrawType::BACKGROUND,
        DrawType::STATIC,
        DrawType::DYNAMIC
    };

    for (auto& type : order) {
        std::ranges::for_each(renderers_, [&](auto& pair)
        {
            if (pair.second.getDrawType() == type) {
                pair.second.render(window_);
            }
        });
    }

    drawTexts();
    selectedTile_.draw(window_);
}


void TowerDefence::View::pushText(Vector2<size_t> position, size_t size, const std::string &text)
{
    auto& t = texts_.emplace_back(font_, text, size);
    t.setPosition({static_cast<float>(position.getX()), static_cast<float>(position.getY())});
}

void TowerDefence::View::clear()
{
    for (auto& renderer : renderers_ | std::views::values) {
        renderer.clear();
    }
}

void TowerDefence::Object::update(float dt)
{
    //timer_.update(dt);
    //if (timer_.isPassed()) {
    //    frame_ = (frame_ + 1) % maxFrames_;
    //    updateSprite();
    //}
}

void TowerDefence::Object::updateSprite()
{
    int texX = 0;//frame_ * (size_.getX() - 1);  // смещение по X
    int texY = 0;                     // все кадры в первой строке

    sprite_.setTextureRect(sf::IntRect({texX, texY}, {static_cast<int>(size_.getX()), static_cast<int>(size_.getY())}));
}

void TowerDefence::Renderer::addObject(ID id, Vector2<size_t> size, Vector2<float> center, size_t maxFrames, float frameTime)
{
    sf::Sprite sprite{texture_};

    auto textureWidth = static_cast<float>(size.getX());
    auto textureHeight = static_cast<float>(size.getY());

    auto x0 = static_cast<float>(center.getX());
    auto y0 = static_cast<float>(center.getY());

    auto x = x0 - textureWidth / 2;
    auto y = y0 - textureHeight / 2;

    sprite.setPosition({x, y});
    float scaleX = static_cast<float>(size.getX()) / static_cast<float>(texture_.getSize().x);
    float scaleY = static_cast<float>(size.getY()) / static_cast<float>(texture_.getSize().y);

    sprite.setScale({scaleX, scaleY});

    objects_.emplace(id, Object{std::move(sprite), size, center, frameTime, maxFrames});
}

void TowerDefence::Renderer::update(float dt)
{
    for (auto& object : objects_ | std::views::values) {
        object.update(dt);
    }
}

void TowerDefence::Renderer::render(sf::RenderWindow& window)
{

    for (const auto& object : objects_ | std::views::values) {
        window.draw(object.getSprite());
    }
}


