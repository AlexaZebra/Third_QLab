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
    QString DirectoryPath;          // путь к текущему каталогу
    QFileSystemModel* FileModel;    // модель файловой системы
    QTableView* TableFileView;      // виджет для отображения файловой системы в виде таблицы
    QtCharts::QChartView* ChartView;// виджет для отображения диаграммы
    QLabel* PathLabel;              // метка, отображающая текущий путь
    QPushButton* BtnPrintChart;     // кнопка для печати диаграммы
    QPushButton* BtnChangeDirectory;// кнопка для изменения текущего каталога в таблице
    QCheckBox* ChkbxBlackWhiteChart;// флажок для изменения цветов диаграммы
    QComboBox* ComboboxChartType;   // выпадающий список для изменения типа диаграммы
    QVBoxLayout* ChartLayout;       // компоновка для диаграммы и ее виджетов
    QGridLayout *ChartWidgetLayout; // компоновка виджетов диаграммы
    QSplitter* Splitter;            // разделитель для разделения виджета таблицы и проводника файловой системы

    IOCContainer Container;
    QList<QPair<QString, qfloat16>> fileData;           // список хранящий данные файла
    bool isChartActive = false;                         // переменная хранящая активность графика
    QString filePath;                                   // строка хранящая путь до файла

    void exceptionCall(QString title, QString message); // вызов messagebox
};
#endif // MAINWINDOW_H
