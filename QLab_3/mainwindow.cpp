#include "mainwindow.h"

#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
#include <QtCharts/QChart>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    DirectoryPath = ""; // инициализация директории

    FileModel = std::make_unique<QFileSystemModel>(this); // модель файловой системы
    FileModel->setRootPath(DirectoryPath); // задание корневой директории

    TableFileView = std::make_unique<QTableView>(this); // виджет таблицы файлов
    TableFileView->setModel(FileModel.get()); // установка модели
    TableFileView->setSelectionMode(QAbstractItemView::SingleSelection); // режим выбора одной строки
    TableFileView->setSelectionBehavior(QAbstractItemView::SelectRows); // выделение всей строки

    ChartView = std::make_unique<QtCharts::QChartView>(this); // виджет диаграммы

    PathLabel = std::make_unique<QLabel>(this); // метка для отображения текущего пути

    BtnPrintChart = std::make_unique<QPushButton>("Print Chart", this); // кнопка печати диаграммы
    BtnChangeDirectory = std::make_unique<QPushButton>("Change Directory", this); // кнопка смены директории в таблице

    ChkbxBlackWhiteChart = std::make_unique<QCheckBox>("Black and White Chart", this); // флажок для изменения цветов диаграммы

    ComboboxChartType = std::make_unique<QComboBox>(this); // комбобокс для изменения типа диаграммы
    ComboboxChartType->addItem("Bar Chart");
    ComboboxChartType->addItem("Pie Chart");
    ComboboxChartType->addItem("Line Chart");

    WrapperLayout = std::make_unique<QHBoxLayout>(this); // главный компоновщик

    ChartWidgetLayout = std::make_unique<QHBoxLayout>(); // компоновщик для виджетов диаграммы
    ChartWidgetLayout->addWidget(BtnPrintChart.get());
    ChartWidgetLayout->addWidget(ChkbxBlackWhiteChart.get());
    ChartWidgetLayout->addWidget(ComboboxChartType.get());

    FileExplorerLayout = std::make_unique<QVBoxLayout>(); // компоновщик для проводника файлов
    FileExplorerLayout->addWidget(TableFileView.get());
    FileExplorerLayout->addWidget(BtnChangeDirectory.get());

    ChartLayout = std::make_unique<QVBoxLayout>(); // компоновщик для диаграммы
    ChartLayout->addLayout(ChartWidgetLayout.get());
    ChartLayout->addWidget(PathLabel.get());
    ChartLayout->addWidget(ChartView.get());

    FileSplitter = std::make_unique<QSplitter>(Qt::Horizontal); // разделитель для разделения проводника файлов и диаграммы
    FileSplitter->addWidget(TableFileView.get());
    FileSplitter->addWidget(BtnChangeDirectory.get());

    ChartSplitter = std::make_unique<QSplitter>(Qt::Vertical); // разделитель для разделения диаграммы и остальных виджетов
    ChartSplitter->addWidget(PathLabel.get());
    ChartSplitter->addWidget(ChartView.get());

    WrapperLayout->addLayout(FileExplorerLayout.get());
    WrapperLayout->addWidget(ChartSplitter.get());

    setLayout(WrapperLayout.get()); // установка главного компоновщика

    setMinimumSize(800, 600); // установка минимального размера окна

    connect(BtnChangeDirectory.get(), &QPushButton::clicked, this, &MainWindow::changeDirectory);
    connect(TableFileView.get()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::fileSelection);
    connect(ComboboxChartType.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeChartType);
    connect(BtnPrintChart.get(), &QPushButton::clicked, this, &MainWindow::printChart);
    connect(ChkbxBlackWhiteChart.get(), &QCheckBox::toggled, this, &MainWindow::colorSwap);
}

MainWindow::~MainWindow()
{
    // Освобождение ресурсов
}

void MainWindow::changeDirectory()
{
    // Обработчик смены директории
    QString newDirectory = QFileDialog::getExistingDirectory(this, "Choose Directory", DirectoryPath);
    if (!newDirectory.isEmpty())
    {
        DirectoryPath = newDirectory;
        FileModel->setRootPath(DirectoryPath);
        TableFileView->setRootIndex(FileModel->index(DirectoryPath));
        PathLabel->setText(DirectoryPath);
    }
}

void MainWindow::fileSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    // Обработчик изменения выбранного файла в таблице
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    // Логика обработки изменения выбранного файла
}

void MainWindow::changeChartType()
{
    // Обработчик изменения типа диаграммы
    // Логика изменения типа диаграммы
}

void MainWindow::printChart()
{
    // Обработчик печати диаграммы
    // Логика печати диаграммы
}

void MainWindow::colorSwap()
{
    // Обработчик смены цвета диаграммы
    // Логика смены цвета диаграммы
}

void MainWindow::exceptionCall(QString title, QString message)
{
    // Вызов диалогового окна с сообщением об ошибке
    QMessageBox::critical(this, title, message);
}

void MainWindow::drawChart()
{
    // Логика рисования диаграммы
}
