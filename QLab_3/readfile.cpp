#include "readfile.h"

bool SqlFileReader::getData(const QString& filePath)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection"); // Чтобы не возникала ошибка "duplicate connection name"
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");  // Создание объекта для работы с SQLite
    db.setDatabaseName(filePath);                            // Установка пути к файлу SQLite
    if (!db.open())                                          // Попытка открытия базы данных
        return false;                                        // В случае ошибки открытия базы данных возвращается false

    // Создание объекта запроса к базе данных SQLite, который выбирает все столбцы из первой таблицы и ограничивает результат до 10 строк
    QSqlQuery query ("SELECT * FROM " + db.tables().takeFirst() + " LIMIT 0,10");

    // Перебор результатов запроса. Вывод значений первого и второго столбцов (индексы 0 и 1) каждой строки с помощью qDebug()
    while (query.next()) {
        qDebug() << query.value(0).toString() << query.value(1).toDouble();
    }

    db.close();                                             // Закрытие базы данных

    return true;                                            // В случае успешного чтения файла возвращается true
}

bool JsonFileReader::getData(const QString& filePath)
{
    QFile file(filePath);                                   // Создание объекта файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  // Открытие файла в режиме чтения
        return false;                                       // В случае ошибки открытия файла возвращается false

    QByteArray jsonData = file.readAll();                   // Чтение содержимого файла
    file.close();                                           // Закрытие файла

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);  // Создание JSON-документа из данных файла
    QJsonObject jsonObj = jsonDoc.object();                 // Получение JSON-объекта из документа

    QStringList keys (jsonObj.keys());                      // Получение списка ключей (имен полей) из JSON-объекта
    int i = 0;
    QListIterator<QString> iterator(keys);                  // Создание итератора для перебора списка ключей
    while (iterator.hasNext() && i < 10) {
        QString key = iterator.next();                      // Получение следующего ключа
        double value = jsonObj.value(key).toDouble();       // Получение значения по ключу и преобразование его в тип double
        qDebug() << key << value;                           // Вывод ключа и значения с помощью qDebug()
        i++;                                                // Увеличение счетчика, чтобы ограничить вывод 10-ю парами ключ-значение
    }

    return true;                                            // В случае успешного чтения файла возвращается true
}

void FileReader::setStrategy(std::shared_ptr<IFileReader> strategy)
{
    this->strategy = strategy;  // Установка переданной стратегии чтения файла
}

bool FileReader::getData(const QString& filePath)
{
    if (strategy)                               // Если стратегия существует
        return strategy->getData(filePath);     // Вызываем метод getData на стратегии чтения файла

    return false;                               // Возвращаем false, если стратегия не существует
}
