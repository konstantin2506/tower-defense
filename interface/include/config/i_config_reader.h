#pragma once
#include <map>
#include <memory>
#include <string>


namespace TowerDefence
{
    /**
     * @class IConfigReader
     * @brief Интерфейс для чтения и записи конфигурационных данных.
     *
     * Предоставляет абстракцию для работы с конфигурационными данными
     * различных форматов (JSON, XML, YAML и т.д.). Поддерживает чтение
     * примитивных типов, вложенных структур и запись значений.
     */
    class IConfigReader {
    public:
        virtual ~IConfigReader() = default;

        /**
         * @brief Получить булево значение из конфигурации.
         * @param field Имя поля.
         * @return bool значение поля.
         */
        [[nodiscard]] virtual bool getBool(const std::string& field) const = 0;

        /**
         * @brief Получить целочисленное значение из конфигурации.
         * @param field Имя поля.
         * @return int значение поля.
         */
        [[nodiscard]] virtual int getInt(const std::string& field) const = 0;

        /**
         * @brief Получить строковое значение из конфигурации.
         * @param field Имя поля.
         * @return std::string значение поля.
         */
        [[nodiscard]] virtual std::string getString(const std::string& field) const = 0;

        /**
         * @brief Получить значение size_t из конфигурации.
         * @param field Имя поля.
         * @return size_t значение поля.
         */
        [[nodiscard]] virtual size_t getSize_t(const std::string& field) const = 0;

        /**
         * @brief Получить значение с плавающей точкой из конфигурации.
         * @param field Имя поля.
         * @return float значение поля.
         */
        [[nodiscard]] virtual float getFloat(const std::string& field) const = 0;

        /**
         * @brief Получить коллекцию вложенных конфигураций.
         * @param field Имя поля.
         * @return std::map<std::string, std::unique_ptr<IConfigReader>> коллекция вложенных конфигураций.
         */
        [[nodiscard]] virtual std::map<std::string, std::unique_ptr<IConfigReader>> getItems(const std::string& field) const = 0;

        /**
         * @brief Получить вложенную конфигурацию.
         * @param field Имя поля.
         * @return std::unique_ptr<IConfigReader> вложенная конфигурация.
         */
        [[nodiscard]] virtual std::unique_ptr<IConfigReader> getNested(const std::string& field) const = 0;

        /**
         * @brief Получить коллекцию строк из конфигурации.
         * @param field Имя поля.
         * @return std::map<std::string, std::string> коллекция строк.
         */
        [[nodiscard]] virtual std::map<std::string, std::string> getStrings(const std::string& field) const = 0;

        /**
         * @brief Получить коллекцию значений size_t из конфигурации.
         * @param field Имя поля.
         * @return std::map<std::string, size_t> коллекция значений.
         */
        [[nodiscard]] virtual std::map<std::string, size_t> getSize_ts(const std::string& field) const = 0;

        /**
         * @brief Создать конфигурацию из файла.
         * @param path Путь к файлу.
         * @return std::unique_ptr<IConfigReader> конфигурация из файла.
         */
        [[nodiscard]] virtual std::unique_ptr<IConfigReader> fromFile(const std::string& path) const = 0;

        // Методы записи значений

        /**
         * @brief Записать значение с плавающей точкой.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeFloat(const std::string& field, float value) = 0;

        /**
         * @brief Записать строковое значение.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeString(const std::string& field, const std::string& value) = 0;

        /**
         * @brief Записать значение size_t.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeSize_t(const std::string& field, size_t value) = 0;

        /**
         * @brief Записать целочисленное значение.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeInt(const std::string& field, int value) = 0;

        /**
         * @brief Записать булево значение.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeBool(const std::string& field, bool value) = 0;

        // Методы записи с указанием пути

        /**
         * @brief Записать строковое значение по пути.
         * @param atField Путь к полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeStringAt(const std::string& atField, const std::string& field, const std::string& value) = 0;

        /**
         * @brief Записать значение size_t по пути.
         * @param atField Путь к полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeSize_tAt(const std::string& atField, const std::string& field, size_t value) = 0;

        /**
         * @brief Записать целочисленное значение по пути.
         * @param atField Путь к полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeIntAt(const std::string& atField, const std::string& field, int value) = 0;

        /**
         * @brief Записать булево значение по пути.
         * @param atField Путь к полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeBoolAt(const std::string& atField, const std::string& field, bool value) = 0;

        /**
         * @brief Записать значение с плавающей точкой по пути.
         * @param atField Путь к полю.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeFloatAt(const std::string& atField, const std::string& field, float value) = 0;

        // Методы записи с указанием вложенного пути

        /**
         * @brief Записать строковое значение по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeStringAtAt(const std::string& atField1, const std::string& atField2, const std::string& field, const std::string& value) = 0;

        /**
         * @brief Записать значение size_t по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeSize_tAtAt(const std::string& atField1, const std::string& atField2, const std::string& field, size_t value) = 0;

        /**
         * @brief Записать целочисленное значение по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeIntAtAt(const std::string& atField1, const std::string& atField2, const std::string& field, int value) = 0;

        /**
         * @brief Записать булево значение по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeBoolAtAt(const std::string& atField1, const std::string& atField2, const std::string& field, bool value) = 0;

        /**
         * @brief Записать значение с плавающей точкой по вложенному пути.
         * @param atField1 Первый уровень пути.
         * @param atField2 Второй уровень пути.
         * @param field Имя поля.
         * @param value Значение для записи.
         */
        virtual void writeFloatAtAt(const std::string& atField1, const std::string& atField2, const std::string& field, float value) = 0;

        /**
         * @brief Получить путь к файлу конфигурации.
         * @return std::string путь к файлу.
         */
        [[nodiscard]] virtual std::string getPath() const = 0;

        /**
         * @brief Получить дамп конфигурации в виде строки.
         * @return std::string строковое представление конфигурации.
         */
        [[nodiscard]] virtual std::string dump() const = 0;

        /**
         * @brief Очистить конфигурацию.
         */
        virtual void clear() = 0;
    };
}