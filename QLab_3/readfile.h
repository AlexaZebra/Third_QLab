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
    virtual ~IFileReader() = default;   // Виртуальный деструктор
    virtual QList<QPair<QString, qfloat16>> getData(const QString& filePath) = 0;// Извлекает данные типа ключ-значение из указанного файла

};
// Конкретная стратегия для чтения SQLite-файлов
class SqlFileReader: public IFileReader {
public:
    QList<QPair<QString, qfloat16>> getData(const QString&)override;
};
// Конкретная стратегия для чтения JSON-файлов
class JsonFileReader: public IFileReader {
public:
    QList<QPair<QString, qfloat16>> getData(const QString&)override;
};

// Конкретная стратегия для чтения CSV-файлов
class CsvFileReader: public IFileReader {
public:
    QList<QPair<QString, qfloat16>> getData(const QString&)override;
};

// Класс для использования
class FileReader
{
private:
    std::shared_ptr<IFileReader> strategy;                                      // Указатель на стратегию чтения файла
public:
    FileReader(std::shared_ptr<IFileReader> strategy) : strategy(strategy) {}   // Конструктор класса FileReader, принимает стратегию чтения файла

    QList<QPair<QString, qfloat16>> getData(const QString& filePath);           // Метод для получения данных из файла, принимает путь к файлу
};

#endif // READFILE_H
