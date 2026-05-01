#pragma once

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>

#include "timer.h"
#include "vector2.h"
#include "config/i_config_reader.h"
#include "world_map/i_world_map.h"


namespace TowerDefence
{
   class Renderer;

   /**
    * @brief Класс для отображения выбранного тайла.
    *
    * Отображает прямоугольник вокруг выбранного тайла с настраиваемыми цветами.
    */
   class SelectedTile {
      Vector2<size_t> position_{}; ///< Позиция выбранного тайла.
      sf::RectangleShape shape_; ///< Графическая форма выделения.
      bool activated_ = false; ///< Флаг активности выделения.

   public:
      SelectedTile() = default;

      /**
       * @brief Конструктор с заданием позиции и размера.
       * @param position Позиция тайла.
       * @param size Размер тайла в пикселях.
       */
      SelectedTile(Vector2<size_t> position,size_t size): position_(position)
      {
         shape_.setSize({static_cast<float>(size), static_cast<float>(size)});
         shape_.setFillColor({255, 255, 255, 100});
         shape_.setOutlineColor({255, 255, 255});
         shape_.setOutlineThickness(2);
      }

      /**
       * @brief Сбрасывает цвет выделения к значениям по умолчанию.
       */
      void reset()
      {
         shape_.setFillColor({255, 255, 255, 100});
         shape_.setOutlineColor({255, 255, 255});
      }

      /**
       * @brief Отрисовывает выделение на окне.
       * @param window Окно для отрисовки.
       */
      void draw(sf::RenderWindow& window) const
      {
         if (activated_) {
            window.draw(shape_);
         }
      }

      /**
       * @brief Устанавливает новую позицию выделения.
       * @param position Новая позиция.
       */
      void setPosition(Vector2<size_t> position)
      {
         reset();
         position_ = position;
         shape_.setPosition({static_cast<float>(position_.getY()) * shape_.getSize().x, static_cast<float>(position_.getX())* shape_.getSize().y});
      }

      /**
       * @brief Устанавливает флаг активности выделения.
       * @param activated true чтобы активировать выделение.
       */
      void setActivated(bool activated)
      {
         activated_ = activated;
      }

      /**
       * @brief Устанавливает цвет выделения.
       * @param color Новый цвет.
       */
      void setColor(sf::Color color)
      {
         sf::Color newColor = color;
         color.a = 100;
         shape_.setFillColor(color);
         color.a = 255;
         shape_.setOutlineColor(color);
      }
   };

   /**
    * @brief Основной класс представления (View) игры.
    *
    * Отвечает за отрисовку всех графических элементов на экране.
    * Управляет окном, камерой, рендерерами и текстом.
    */
   class View { // просто рисуем че дают и все вью тупой
      sf::RenderWindow window_; ///< Окно SFML для отрисовки.
      sf::View view_; ///< Вид (камера) для масштабирования.
      bool zoomed_ = false; ///< Флаг режима масштабирования.
      std::map<std::string, Renderer> renderers_; ///< Мапа рендереров по имени.
      std::vector<sf::Text> texts_; ///< Вектор текстовых элементов для отрисовки.
      std::pair<Vector2<size_t>, Vector2<size_t>> panelBorders_; ///< Границы панели интерфейса.
      std::pair<Vector2<size_t>, Vector2<size_t>> tileMapBorders_; ///< Границы карты тайлов.
      size_t tileSize_; ///< Размер одного тайла в пикселях.
      size_t mapSizeInCells_; ///< Размер карты в клетках.
      size_t buttonsInPanel_; ///< Количество кнопок в панели.
      size_t panelButtonSize_; ///< Размер кнопки в панели.
      SelectedTile selectedTile_; ///< Выделенный тайл.
      sf::Font font_; ///< Шрифт для текста.
      float zoom_ = 1.f; ///< Текущий уровень масштабирования.

   public:
      /**
       * @brief Конструктор представления.
       * @param cfg Конфигурация для инициализации.
       * @param worldMap Игровая карта.
       * @param buildings Мапа зданий для отрисовки.
       */
      View(const IConfigReader& cfg, const IWorldMap& worldMap, const std::unordered_map<size_t, std::string>& buildings);

      /**
       * @brief Деструктор закрывает окно при необходимости.
       */
      ~View() {if (window_.isOpen()) window_.close();}

      /**
       * @brief Добавляет изображение для отрисовки.
       * @param name Имя рендерера.
       * @param imagePosition Позиция изображения.
       * @param imageSize Размер изображения.
       * @param maxFrames Максимальное количество кадров анимации.
       * @param frameTime Время между кадрами.
       * @param id Уникальный идентификатор объекта.
       */
      void pushImage(const std::string& name, Vector2<float> imagePosition, Vector2<size_t> imageSize, size_t maxFrames, float frameTime, ID id = 0);

      /**
       * @brief Отрисовывает все графические элементы.
       */
      void drawAll();

      /**
       * @brief Получает событие из окна.
       * @return true если есть события, false если нет.
       */
      auto getEvent() {return window_.pollEvent();}

      /**
       * @brief Возвращает размер окна.
       */
      auto size() const {return window_.getSize();}

      /**
       * @brief Добавляет текстовый элемент.
       * @param position Позиция текста.
       * @param size Размер шрифта.
       * @param text Текст для отображения.
       */
      void pushText(Vector2<size_t> position, size_t size, const std::string& text);

      /**
       * @brief Проверяет, открыто ли окно.
       */
      bool isOpen() const {return window_.isOpen();}

      /**
       * @brief Очищает окно.
       */
      void clear();

      /**
       * @brief Устанавливает масштабированный вид.
       */
      void setZoomedView()
      {
         window_.setView(view_);
         zoomed_ = true;
      }

      /**
       * @brief Изменяет масштаб вида.
       * @param delta Изменение масштаба.
       */
      void zoomView(float delta)
      {
         if (!zoomed_) {
            return;
         }
         view_.zoom(zoom_ +  delta*0.1f);
         view_.setSize(view_.getSize() / zoom_);
         window_.setView(view_);
      }

      /**
       * @brief Устанавливает вид панели (без масштабирования).
       */
      void setPanelView()
      {
         zoomed_ = false;
         window_.setView(window_.getDefaultView());
      }

      /**
       * @brief Отображает отрисованный кадр.
       */
      void show() {window_.display();}

      /**
       * @brief Устанавливает позицию выделенного тайла.
       * @param position Новая позиция.
       */
      void setSelectedTilePos(Vector2<size_t> position) {selectedTile_.setPosition(position);}

      /**
       * @brief Устанавливает состояние выделения тайла.
       * @param selected Флаг выделения.
       * @param color Цвет выделения (по умолчанию белый).
       */
      void stateSelectedPos(bool selected, sf::Color color = sf::Color::White)
      {
         selectedTile_.setActivated(selected);
         if (color != sf::Color::White)
            selectedTile_.setColor(color);
      }

      /**
       * @brief Возвращает границы панели.
       */
      std::pair<Vector2<size_t>, Vector2<size_t>> getPanelBorders() const {return panelBorders_;}

      /**
       * @brief Возвращает границы карты тайлов.
       */
      std::pair<Vector2<size_t>, Vector2<size_t>> getTileMapBorders() const {return tileMapBorders_;}

      /**
       * @brief Возвращает размер тайла.
       */
      size_t getTileSize() const {return tileSize_;}

      /**
       * @brief Возвращает размер кнопки в панели.
       */
      size_t getPanelButtonSize() const {return panelButtonSize_;}

      /**
       * @brief Проверяет, содержит ли рендерер объект с заданным ID.
       * @param name Имя рендерера.
       * @param id ID объекта.
       * @return true если объект существует.
       */
      bool contains(const std::string& name, ID id) const;

      /**
       * @brief Обновляет анимацию объекта.
       * @param name Имя рендерера.
       * @param id ID объекта.
       * @param dt Время с предыдущего кадра.
       */
      void update(const std::string& name, ID id, float dt);

      /**
       * @brief Удаляет объект из отрисовки.
       * @param name Имя рендерера.
       * @param id ID объекта.
       */
      void kill(const std::string& name, ID id);

      /**
       * @brief Преобразует SFML вектор в Vector2.
       * @tparam T Арифметический тип.
       * @param vec SFML вектор.
       * @return Vector2<size_t>.
       */
      template<Arithmetic T>
      static Vector2<size_t> toMyVec2(sf::Vector2<T> vec) {return Vector2<size_t>(vec.y, vec.x);}

      /**
       * @brief Преобразует Vector2 в SFML вектор.
       * @tparam T Арифметический тип.
       * @param vec Vector2.
       * @return sf::Vector2<T>.
       */
      template<Arithmetic T>
      static sf::Vector2<T> toSfmlVec2(Vector2<size_t> vec) {return sf::Vector2<T>(vec.getY(), vec.getX());}

   private:
      /**
       * @brief Добавляет новый рендерер.
       * @param name Имя рендерера.
       * @param texture Текстура для рендерера.
       * @param type Тип отрисовки.
       */
      void addRenderer(const std::string& name, sf::Texture&& texture, DrawType type);

      /**
       * @brief Отрисовывает все текстовые элементы.
       */
      void drawTexts();
   };

   /**
    * @brief Класс графического объекта с анимацией.
    *
    * Управляет спрайтом, анимацией и позицией графического объекта.
    */
   class Object {
      sf::Sprite sprite_; ///< Спрайт объекта.
      size_t frame_ = 1; ///< Текущий кадр анимации.
      size_t maxFrames_ = 1; ///< Максимальное количество кадров.
      Timer timer_; ///< Таймер для смены кадров.
      Vector2<size_t> size_; ///< Размер объекта.
      Vector2<size_t> position_; ///< Позиция объекта.

   public:
      /**
       * @brief Конструктор объекта.
       * @param sprite Спрайт объекта.
       * @param size Размер объекта.
       * @param position Позиция объекта.
       * @param frameTime Время между кадрами.
       * @param maxFrames Максимальное количество кадров.
       */
      Object(sf::Sprite&& sprite, Vector2<size_t> size, Vector2<size_t> position, float frameTime, size_t maxFrames) :
      sprite_(std::move(sprite)), maxFrames_(maxFrames), timer_(frameTime) , size_(size), position_(position)
      {
         //sprite_.setOrigin({static_cast<float>(size_.getX() / 2), static_cast<float>(size_.getY() /2)});
      }

      /**
       * @brief Возвращает спрайт объекта.
       */
      const sf::Sprite& getSprite() const {return sprite_;}

      /**
       * @brief Обновляет состояние объекта.
       * @param dt Время с предыдущего кадра.
       */
      void update(float dt);

      /**
       * @brief Перемещает объект в новую позицию.
       * @param position Новая позиция.
       */
      void replace(Vector2<size_t> position){
         sprite_. setPosition({static_cast<float>(position.getX()), static_cast<float>(position.getY())});
         position_ = position;
      }

   private:
      /**
       * @brief Обновляет спрайт (сменяет кадр анимации).
       */
      void updateSprite();
   };

   /**
    * @brief Класс рендерера для управления группой объектов.
    *
    * Хранит и управляет коллекцией объектов с одинаковой текстурой.
    */
   class Renderer {
      std::unordered_map<ID, Object> objects_; ///< Мапа объектов по ID.
      const sf::Texture texture_; ///< Текстура для всех объектов.
      DrawType drawType_; ///< Тип отрисовки.

   public:
      /**
       * @brief Конструктор рендерера.
       * @param texture Текстура для объектов.
       * @param type Тип отрисовки.
       */
      Renderer(sf::Texture texture, DrawType type) : texture_(std::move(texture)), drawType_(type) {}

      /**
       * @brief Добавляет новый объект.
       * @param id Уникальный идентификатор.
       * @param size Размер объекта.
       * @param position Позиция объекта.
       * @param maxFrames Максимальное количество кадров.
       * @param frameTime Время между кадрами.
       */
      void addObject(ID id, Vector2<size_t> size, Vector2<float> position, size_t maxFrames,float frameTime);

      /**
       * @brief Удаляет объект.
       * @param id ID объекта для удаления.
       */
      void removeObject(ID id) {objects_.erase(id);}

      /**
       * @brief Обновляет все объекты рендерера.
       * @param dt Время с предыдущего кадра.
       */
      void update(float dt);

      /**
       * @brief Перемещает объект.
       * @param id ID объекта.
       * @param newPlace Новая позиция.
       */
      void replaceObject(ID id, Vector2<size_t> newPlace) {objects_.at(id).replace(newPlace);}

      /**
       * @brief Отрисовывает все объекты рендерера.
       * @param window Окно для отрисовки.
       */
      void render(sf::RenderWindow& window);

      /**
       * @brief Проверяет наличие объекта.
       * @param id ID объекта.
       * @return true если объект существует.
       */
      bool contains(ID id) const {return objects_.contains(id);}

      /**
       * @brief Возвращает тип отрисовки.
       */
      DrawType getDrawType() const {return drawType_;}

      /**
       * @brief Очищает все объекты.
       */
      void clear() {objects_.clear();}
   };
}