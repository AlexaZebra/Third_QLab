#include "readfile.h"

QList<QPair<QString, qfloat16>> SqlFileReader::getData(const QString& filePath)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection"); // Чтобы не возникала ошибка "duplicate connection name"
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");  // Создание объекта для работы с SQLite
    db.setDatabaseName(filePath);                            // Установка пути к файлу SQLite
    QList<QPair<QString, qfloat16>> result;
    if (db.open()) {                                         // Попытка открытия базы данных
        // Создание объекта запроса к базе данных SQLite, который выбирает все столбцы из первой таблицы и ограничивает результат до 10 строк
        QSqlQuery query ("SELECT * FROM " + db.tables().takeFirst() + " LIMIT 0,10");

        // Перебор результатов запроса. Запись значений первого и второго столбцов (индексы 0 и 1) каждой строки в result
        while (query.next()) {
            QPair<QString,qfloat16> temp{query.value(0).toString(), query.value(1).toDouble()};
            result.push_back(temp);
        }

        db.close();                                             // Закрытие базы данных
    }

    return result;                                              // В случае успешного чтения файла возвращается result
}

QList<QPair<QString, qfloat16>> JsonFileReader::getData(const QString& filePath)
{
    QFile file(filePath);                                       // Создание объекта файла
    QList<QPair<QString, qfloat16>> result;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){      // Открытие файла в режиме чтения

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
            QPair<QString,qfloat16> temp{key, value};
            result.push_back(temp);
            i++;                                                // Увеличение счетчика, чтобы ограничить вывод 10-ю парами ключ-значение
        }
    }
    return result;
}

QList<QPair<QString, qfloat16>> CsvFileReader::getData(const QString& filePath)
{
    QFile file(filePath);                                       // Создание объекта файла
    QList<QPair<QString, qfloat16>> result;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){      // Открытие файла в режиме чтения

        QTextStream in(&file);                                  // Создание текстового потока для чтения файла

        int i = 0;
        while (!in.atEnd() && i < 10) {
            QString line = in.readLine();                       // Чтение строки файла
            QStringList values = line.split(",");               // Разделение строки на значения

            if (values.size() == 2) {
                QString key = values[0].trimmed();              // Извлечение ключа (первое значение)
                qfloat16 value = values[1].trimmed().toFloat(); // Извлечение значения (второе значение)

                QPair<QString, qfloat16> pair(key, value);
                result.append(pair);
                i++;
            }
        }

        file.close();                                            // Закрытие файла
    }
    return result;
}

QList<QPair<QString, qfloat16>> FileReader::getData(const QString& filePath)
{
    return strategy->getData(filePath);     // Вызываем метод getData на стратегии чтения файла
}
