#pragma once

#include "config/i_config_reader.h"
#include <nlohmann/json.hpp>

namespace TowerDefence
{
    using json = nlohmann::json;

    /**
     * @class JsonReader
     * @brief Реализация IConfigReader для работы с JSON-конфигурациями.
     *
     * Финальный класс, предоставляющий чтение и запись конфигурационных данных
     * в формате JSON с использованием библиотеки nlohmann/json.
     */
    class JsonReader final: public IConfigReader {
        json cfg_;
        std::string path_;
    public:
        /**
         * @brief Создать конфигурацию из файла.
         * @param path Путь к JSON-файлу.
         * @return std::unique_ptr<IConfigReader> загруженная конфигурация.
         */
        [[nodiscard]] std::unique_ptr<IConfigReader> fromFile(const std::string& path) const override;

        /**
         * @brief Создать JsonReader из готового JSON-объекта.
         * @param json JSON-объект для инициализации.
         * @return std::unique_ptr<JsonReader> созданный читатель.
         */
        static std::unique_ptr<JsonReader> fromJson(json&&);

        /**
         * @brief Конструктор JsonReader.
         * @param json_ JSON-объект для работы.
         * @param path Путь к файлу (по умолчанию "NoPath").
         */
        explicit JsonReader(json json_, std::string path = "NoPath") : cfg_(std::move(json_)), path_(std::move(path)) {}

        /**
         * @brief Получить значение поля заданного типа.
         * @tparam T Тип возвращаемого значения.
         * @param field Имя поля.
         * @return T значение поля.
         * @throws std::logic_error если поле не найдено или тип не совпадает.
         */
        template<typename T>
        [[nodiscard]] T getField(const std::string& field) const
        {
            try {
                return cfg_.at(field).get<T>();
            }catch (const std::exception& e) {
                throw std::logic_error("Error getting field " + field + ": " + e.what() + "; Path: " + path_);
            }
        }

        /**
         * @brief Получить путь к файлу конфигурации.
         * @return std::string путь к файлу.
         */
        [[nodiscard]] std::string getPath() const override { return path_; }

        /**
         * @brief Получить строковое представление JSON.
         * @return std::string JSON в виде строки с отступами.
         */
        [[nodiscard]] std::string dump() const override { return cfg_.dump(4); }

        /**
         * @brief Очистить конфигурацию.
         */
        void clear() override { cfg_.clear(); }

        // Реализации методов чтения базовых типов

        /**
         * @brief Получить булево значение.
         * @param field Имя поля.
         * @return bool значение.
         */
        [[nodiscard]] bool getBool(const std::string &field) const override {return getField<bool>(field);}

        /**
         * @brief Получить целочисленное значение.
         * @param field Имя поля.
         * @return int значение.
         */
        [[nodiscard]] int getInt(const std::string &field) const override {return  getField<int>(field);}

        /**
         * @brief Получить строковое значение.
         * @param field Имя поля.
         * @return std::string значение.
         */
        [[nodiscard]] std::string getString(const std::string &field) const override {return getField<std::string>(field);}

        /**
         * @brief Получить значение size_t.
         * @param field Имя поля.
         * @return size_t значение.
         */
        [[nodiscard]] size_t getSize_t(const std::string &field) const override {return getField<size_t>(field);}

        /**
         * @brief Получить значение с плавающей точкой.
         * @param field Имя поля.
         * @return float значение.
         */
        [[nodiscard]] float getFloat(const std::string &field) const override {return getField<float>(field);}

        /**
         * @brief Получить коллекцию вложенных конфигураций.
         * @param field Имя поля.
         * @return std::map<std::string, std::unique_ptr<IConfigReader>> коллекция конфигураций.
         */
        [[nodiscard]] std::map<std::string, std::unique_ptr<IConfigReader>> getItems(const std::string& field) const override;

        /**
         * @brief Получить коллекцию строк.
         * @param field Имя поля.
         * @return std::map<std::string, std::string> коллекция строк.
         */
        [[nodiscard]] std::map<std::string, std::string> getStrings(const std::string& field) const override;

        /**
         * @brief Получить коллекцию значений size_t.
         * @param field Имя поля.
         * @return std::map<std::string, size_t> коллекция значений.
         */
        [[nodiscard]] std::map<std::string, size_t> getSize_ts(const std::string& field) const override;

        /**
         * @brief Получить вложенную конфигурацию.
         * @param field Имя поля.
         * @return std::unique_ptr<IConfigReader> вложенная конфигурация.
         */
        [[nodiscard]] std::unique_ptr<IConfigReader> getNested(const std::string& field) const override;

        // Реализации методов записи базовых типов

        /**
         * @brief Записать строковое значение.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeString(const std::string &field, const std::string &value) override {cfg_[field] = value; }

        /**
         * @brief Записать значение size_t.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeSize_t(const std::string &field, size_t value) override {cfg_[field] = value; }

        /**
         * @brief Записать целочисленное значение.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeInt(const std::string &field, int value) override {cfg_[field] = value; }

        /**
         * @brief Записать булево значение.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeBool(const std::string &field, bool value) override {cfg_[field] = value; }

        /**
         * @brief Записать значение с плавающей точкой.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeFloat(const std::string &field, float value) override {cfg_[field] = value; }

        // Методы записи с указанием пути (одноуровневая вложенность)

        /**
         * @brief Записать строковое значение по пути.
         * @param atField Путь к родительскому полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeStringAt(const std::string &atField, const std::string &field, const std::string &value) override {cfg_[atField][field] = value; }

        /**
         * @brief Записать значение size_t по пути.
         * @param atField Путь к родительскому полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeSize_tAt(const std::string &atField, const std::string &field, size_t value) override {cfg_[atField][field] = value; }

        /**
         * @brief Записать целочисленное значение по пути.
         * @param atField Путь к родительскому полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeIntAt(const std::string &atField, const std::string &field, int value) override {cfg_[atField][field] = value; }

        /**
         * @brief Записать булево значение по пути.
         * @param atField Путь к родительскому полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeBoolAt(const std::string &atField, const std::string &field, bool value) override {cfg_[atField][field] = value; }

        /**
         * @brief Записать значение с плавающей точкой по пути.
         * @param atField Путь к родительскому полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeFloatAt(const std::string &atField, const std::string &field, float value) override {cfg_[atField][field] = value; }

        // Методы записи с указанием вложенного пути (двухуровневая вложенность)

        /**
         * @brief Записать строковое значение по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeStringAtAt(const std::string &atField1, const std::string &atField2, const std::string &field,
            const std::string &value) override{cfg_[atField1][atField2][field] = value; }

        /**
         * @brief Записать значение size_t по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeSize_tAtAt(const std::string &atField1, const std::string &atField2, const std::string &field,
            size_t value) override {cfg_[atField1][atField2][field] = value; }

        /**
         * @brief Записать целочисленное значение по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeIntAtAt(const std::string &atField1, const std::string &atField2, const std::string &field,
            int value) override {cfg_[atField1][atField2][field] = value; }

        /**
         * @brief Записать булево значение по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeBoolAtAt(const std::string &atField1, const std::string &atField2, const std::string &field,
            bool value) override {cfg_[atField1][atField2][field] = value; }

        /**
         * @brief Записать значение с плавающей точкой по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        void writeFloatAtAt(const std::string &atField1, const std::string &atField2, const std::string &field,
            float value) override {cfg_[atField1][atField2][field] = value; }
    };
}