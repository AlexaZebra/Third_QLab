#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"IOCContainer.h"
#include"readfile.h"
#include "chart.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <memory>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void changeDirectory(); // слот смены директории
    //слот изменения выбранного файла в таблице
    void fileSelection(const QItemSelection &selected, const QItemSelection &deselected);
    //слот изменения типа графика
    void changeChartType();
    //слот печати диаграммы
    void printChart();
    //слот смены цвета диаграммы
    void colorSwap();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString DirectoryPath;                              // путь к текущему каталогу
    std::shared_ptr<QFileSystemModel> FileModel;        // модель файловой системы
    std::unique_ptr<QTableView> TableFileView;          // виджет для отображения файловой системы в виде таблицы
    std::unique_ptr<QtCharts::QChartView> ChartView;    // виджет для отображения диаграммы
    std::unique_ptr<QPushButton> BtnPrintChart;         // кнопка для печати диаграммы
    std::unique_ptr<QPushButton> BtnChangeDirectory;    // кнопка для изменения текущего каталога в таблице
    std::unique_ptr<QCheckBox> ChkbxBlackWhiteChart;    // флажок для изменения цветов диаграммы
    std::unique_ptr<QComboBox> ComboboxChartType;       // выпадающий список для изменения типа диаграммы
    std::unique_ptr<QVBoxLayout> ChartLayout;           // компоновка для диаграммы и ее виджетов
    std::unique_ptr<QGridLayout> ChartWidgetLayout;     // компоновка виджетов диаграммы
    std::unique_ptr<QSplitter> Splitter;                // разделитель для разделения виджета таблицы и проводника файловой системы


    IOCContainer Container;
    QList<QPair<QString, qfloat16>> fileData;           // список хранящий данные файла
    bool isChartActive = false;                         // переменная хранящая активность графика
    QString filePath;                                   // строка хранящая путь до файла

    void exceptionCall(QString title, QString message); // вызов messagebox
};
#endif // MAINWINDOW_H
