#ifndef READFILE_H
#define READFILE_H

#include <QJsonDocument>    // Подключение класса для работы с JSON-документами
#include <QJsonObject>      // Подключение класса для работы с JSON-объектами
#include <QJsonArray>       // Подключение класса для работы с JSON-массивами
#include <QFile>            // Подключение класса для работы с файлами
#include <QtSql>            // Подключение классов для работы с SQLite

// Интерфейс стратегии для чтения файлов
class IFileReader {
public:
    virtual bool getData(const QString& filePath) = 0;  // Виртуальный метод чтения файла
    virtual ~IFileReader() = default;                   // Виртуальный деструктор
};
// Конкретная стратегия для чтения SQLite-файлов
class SqlFileReader: public IFileReader {
public:
    bool getData(const QString&)override;
};
// Конкретная стратегия для чтения JSON-файлов
class JsonFileReader: public IFileReader {
public:
    bool getData(const QString&)override;
};

// Класс, который использует стратегию для чтения файлов
class FileReader
{
private:
    std::shared_ptr<IFileReader> strategy;                                      // Указатель на стратегию чтения файла
public:
    FileReader(std::shared_ptr<IFileReader> strategy) : strategy(strategy) {}   // Конструктор класса FileReader, принимает стратегию чтения файла

    void setStrategy(std::shared_ptr<IFileReader> strategy);                    // Метод для установки стратегии чтения файла

    bool getData(const QString& filePath);                                      // Метод для получения данных из файла, принимает путь к файлу
};

#endif // READFILE_H
