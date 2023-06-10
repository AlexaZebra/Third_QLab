#include "mainwindow.h"

#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
#include <QtCharts/QChart>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(100, 100, 1000, 600);
    DirectoryPath = ""; // инициализация директории

    FileModel = std::make_unique<QFileSystemModel>(this); // модель файловой системы
    FileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    DirectoryPath = QDir::currentPath();   // путь к текущему каталогу
    FileModel->setRootPath(DirectoryPath); // задание корневой директории
    QModelIndex PathIndex = FileModel->setRootPath(DirectoryPath);

    TableFileView = std::make_unique<QTableView>(this); // виджет таблицы файлов
    TableFileView->setModel(FileModel.get()); // установка модели
    TableFileView->setRootIndex(PathIndex);

    ChartView = std::make_unique<QtCharts::QChartView>(this); // виджет диаграммы
    ChartView->setRenderHint(QPainter::Antialiasing);

    TableFileView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding); // Установите вертикальный размер TableFileView
    ChartView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    FileSplitter = std::make_unique<QSplitter>(Qt::Vertical); // разделитель для разделения проводника файлов и диаграммы
    FileSplitter->addWidget(TableFileView.get());

    ChartSplitter = std::make_unique<QSplitter>(Qt::Vertical); // разделитель для разделения диаграммы и остальных виджетов
    ChartSplitter->addWidget(ChartView.get());

    FileSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ChartSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    WrapperLayout = std::make_unique<QHBoxLayout>(this); // главный компоновщик

    PathLabel = std::make_unique<QLabel>(this); // метка для отображения текущего пути
    PathLabel->setText("Choose file");

    BtnPrintChart = std::make_unique<QPushButton>("Print Chart", this); // кнопка печати диаграммы
    BtnChangeDirectory = std::make_unique<QPushButton>("Change Directory", this); // кнопка смены директории в таблице

    ChkbxBlackWhiteChart = std::make_unique<QCheckBox>("Black and White Chart", this); // флажок для изменения цветов диаграммы

    ComboboxChartType = std::make_unique<QComboBox>(this); // комбобокс для изменения типа диаграммы
    ComboboxChartType->addItem("Bar Chart");
    ComboboxChartType->addItem("Pie Chart");
    ComboboxChartType->addItem("Line Chart");

    ChartWidgetLayout = std::make_unique<QHBoxLayout>(); // компоновщик для виджетов диаграммы
    ChartWidgetLayout->addWidget(ComboboxChartType.get());
    ChartWidgetLayout->addWidget(ChkbxBlackWhiteChart.get());
    ChartWidgetLayout->addWidget(BtnPrintChart.get());

    FileExplorerLayout = std::make_unique<QVBoxLayout>(); // компоновщик для проводника файлов
    FileExplorerLayout->addWidget(FileSplitter.get(), 1); // 1 - чтобы  сплиттер был во всю длину окна
    FileExplorerLayout->addWidget(BtnChangeDirectory.get(), 0);
    FileExplorerLayout->addWidget(PathLabel.get(), 0);

    ChartLayout = std::make_unique<QVBoxLayout>(); // компоновщик для диаграммы
    ChartLayout->addLayout(ChartWidgetLayout.get());
    ChartLayout->addWidget(ChartSplitter.get());

    WrapperLayout->addLayout(FileExplorerLayout.get());
    WrapperLayout->addLayout(ChartLayout.get());

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
