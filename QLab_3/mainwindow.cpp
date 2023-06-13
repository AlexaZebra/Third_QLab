#include "mainwindow.h"

#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
#include <QtCharts/QChart>
int IOCContainer::s_nextTypeId = 1;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Создание и настройка виджетов

    this->setGeometry(100, 100, 1000, 600);
    this->setMinimumSize(900, 300);
    DirectoryPath = "";                                                  // инициализация директории

    FileModel = new QFileSystemModel(this);                              // модель файловой системы
    FileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    DirectoryPath = QDir::currentPath();                                 // путь к текущему каталогу
    FileModel->setRootPath(DirectoryPath);                               // задание корневой директории
    QModelIndex PathIndex = FileModel->setRootPath(DirectoryPath);

    TableFileView = new QTableView(this);                                // виджет таблицы файлов
    TableFileView->setModel(FileModel);                                  // установка модели
    TableFileView->setSelectionMode(QAbstractItemView::SingleSelection); // режим выбора одной строки
    TableFileView->setSelectionBehavior(QAbstractItemView::SelectRows);  // выделение всей строки
    TableFileView->setRootIndex(PathIndex);

    ChartView = new QtCharts::QChartView(this);                          // виджет диаграммы
    ChartView->setRenderHint(QPainter::Antialiasing);

    FileSplitter = new QSplitter(Qt::Horizontal);
    ChartSplitter = new QSplitter();

    FileSplitter->addWidget(TableFileView);
    FileSplitter->addWidget(ChartView);
    //ChartSplitter->addWidget(ChartView);
    //ChartSplitter->addWidget(TableFileView);

    WrapperLayout = new QHBoxLayout();                               // главный компоновщик

    PathLabel = new QLabel(this);                                        // метка для отображения текущего пути
    PathLabel->setText("Choose file");

    BtnPrintChart = new QPushButton("Print Chart", this);                // кнопка печати диаграммы
    BtnChangeDirectory = new QPushButton("Change Directory", this);      // кнопка смены директории в таблице

    ChkbxBlackWhiteChart = new QCheckBox("Black and White Chart", this); // чекбокс для изменения цветов диаграммы

    ComboboxChartType = new QComboBox(this);                             // комбобокс для изменения типа диаграммы
    ComboboxChartType->addItem("Bar Chart");
    ComboboxChartType->addItem("Pie Chart");
    ComboboxChartType->addItem("Line Chart");

    QLabel *label = new QLabel("Choose type of diagrams");

    QGridLayout *ChartWidgetLayout = new QGridLayout();
    //ChartWidgetLayout = new QHBoxLayout();                               // компоновщик для виджетов диаграммы
    ChartWidgetLayout->addWidget(label,1,0);
    ChartWidgetLayout->addWidget(ComboboxChartType,1,1);
    ChartWidgetLayout->addWidget(ChkbxBlackWhiteChart,1,2);
    ChartWidgetLayout->addWidget(BtnPrintChart,1,3);


    //FileExplorerLayout = new QVBoxLayout();                              // компоновщик для проводника файлов
    //FileExplorerLayout->addWidget(FileSplitter,1);
    //FileExplorerLayout->addWidget(BtnChangeDirectory, 0);
    //FileExplorerLayout->addWidget(PathLabel, 0);
    //baseLayout->addWidget(chartView, 1, 0);

    ChartLayout = new QVBoxLayout();                                     // компоновщик для диаграммы
    ChartLayout->addLayout(ChartWidgetLayout);
    ChartLayout->addWidget(FileSplitter,1);
    ChartLayout->addWidget(BtnChangeDirectory, 0);
    ChartLayout->addWidget(PathLabel, 0);


   // WrapperLayout->addLayout(FileExplorerLayout);
    WrapperLayout->addLayout(ChartLayout);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(WrapperLayout);
    setCentralWidget(centralWidget);

    connect(BtnChangeDirectory, &QPushButton::clicked, this, &MainWindow::changeDirectory);
    connect(TableFileView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::fileSelection);
    connect(ComboboxChartType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeChartType);
    connect(BtnPrintChart, &QPushButton::clicked, this, &MainWindow::printChart);
    connect(ChkbxBlackWhiteChart, &QCheckBox::toggled, this, &MainWindow::colorSwap);
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
    }
}

void MainWindow::fileSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexes = selected.indexes();               // Получаем список индексов выбранных элементов
    QString filePath = FileModel->filePath(indexes.first());    // Получаем путь к файлу, выбранному в модели

    QFile file(filePath);                                       // Создаем объект файла с указанным путем для проверки ошибок открытия/пустоты

    if (!file.open(QIODevice::ReadOnly)) {                      // Если не удалось открыть файл для чтения
        exceptionCall("File Error", "Unable to open the file"); // Вызываем функцию обработки исключения с сообщением об ошибке
        file.close();                                           // Закрытие файла в случае ошибки открытия, чтобы избежать утечки ресурсов.
        return;                                                 // Возвращаемся из функции
    }

    if (file.size() == 0) {                                         // Проверка размера файла
        exceptionCall("Empty File", "The selected file is empty");  // Файл пустой, обработка исключения
        file.close();
        return;
    }


    if (filePath.endsWith(".json")) {                                       // Если путь к файлу оканчивается на ".json"
        //std::shared_ptr<IFileReader> jsonStrategy(new JsonFileReader());
        //FileReader fileReader(jsonStrategy.get()); // Получение указателя на стратегию через метод get()
        Container.RegisterInstance<IFileReader, JsonFileReader>();
        auto jsonStrategy = Container.GetObject<IFileReader>();
        FileReader fileReader(jsonStrategy);
        fileReader.getData(filePath);
    }
    else if (filePath.endsWith(".sqlite")) {
        //std::shared_ptr<IFileReader> sqlStrategy(new SqlFileReader());
        //sqlStrategy->getData(filePath);
        Container.RegisterInstance<IFileReader, SqlFileReader>();
        auto sqlStrategy = Container.GetObject<IFileReader>();
        FileReader fileReader(sqlStrategy);
        fileReader.getData(filePath);
    }
    else if (filePath.isEmpty()) {
        exceptionCall("File Error", "Selected file is empty");
        return;
    }
    else {
        exceptionCall("Wrong file format", "Please select .json or .sqlite file");
        return;
    }

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
