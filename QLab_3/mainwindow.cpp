#include "mainwindow.h"

#include <QBrush>
#include <QGraphicsColorizeEffect>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
#include <QtCharts/QChart>
int IOCContainer::s_nextTypeId = 1;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Создание и настройка виджетов

    this->setGeometry(100, 100, 1280, 720);
    this->setMinimumSize(900, 300);
    DirectoryPath = "";                                                  // инициализация директории

    FileModel = std::make_shared<QFileSystemModel>(this);                // модель файловой системы
    FileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    DirectoryPath = QDir::currentPath();                                 // путь к текущему каталогу
    FileModel->setRootPath(DirectoryPath);                               // задание корневой директории
    QModelIndex PathIndex = FileModel->setRootPath(DirectoryPath);       // установка директории корневой

    QStringList formats;                                                 // допустимые форматы
    formats << "sqlite" << "json" << "csv";
    QStringList filters;                                                 // фильтр с допустимыми форматами
    for (const QString& format : qAsConst(formats)) {
        filters.append(QString("*.%1").arg(format));
    }

    FileModel->setNameFilters(filters);                                  // применяем данный фильтр
    FileModel->setNameFilterDisables(false);                             // включили фильтрацию

    TableFileView = std::make_unique<QTableView>(this);// виджет таблицы файлов
    TableFileView->setModel(FileModel.get());                            // установка модели
    TableFileView->setSelectionMode(QAbstractItemView::SingleSelection); // режим выбора одной строки
    TableFileView->setSelectionBehavior(QAbstractItemView::SelectRows);  // выделение всей строки
    TableFileView->setRootIndex(PathIndex);

    ChartView = std::make_unique<QtCharts::QChartView>(this);            // виджет диаграммы

    Splitter = std::make_unique<QSplitter>();

    Splitter->addWidget(TableFileView.get());
    Splitter->addWidget(ChartView.get());

    // Установка размеров виджетов в QSplitter
    QList<int> sizes;
    sizes << 1 << 1;
    Splitter->setSizes(sizes);

    BtnPrintChart = std::make_unique<QPushButton>("Печать", this);              // кнопка печати диаграммы
    BtnPrintChart->setEnabled(false);

    BtnChangeDirectory = std::make_unique<QPushButton>("Выбрать папку", this);  // кнопка смены директории в таблице

    ChkbxBlackWhiteChart = std::make_unique<QCheckBox>("ч/б график", this);     // чекбокс для изменения цветов диаграммы

    ComboboxChartType = std::make_unique<QComboBox>(this);                      // комбобокс для изменения типа диаграммы
    ComboboxChartType->addItem("Столбчатая");
    ComboboxChartType->addItem("Круговая");
    ComboboxChartType->addItem("Линейная");

    std::unique_ptr<QLabel> label = std::make_unique<QLabel>("Выберите тип диаграммы");

    std::unique_ptr<QGridLayout> ChartWidgetLayout = std::make_unique<QGridLayout>();// компоновщик для виджетов диаграммы и кнопки выбора папки
    ChartWidgetLayout->addWidget(BtnChangeDirectory.get(),1,0);
    ChartWidgetLayout->addWidget(label.get(),1,1);
    ChartWidgetLayout->addWidget(ComboboxChartType.get(),1,2);
    ChartWidgetLayout->addWidget(ChkbxBlackWhiteChart.get(),1,3);
    ChartWidgetLayout->addWidget(BtnPrintChart.get(),1,4);

    ChartLayout = std::make_unique<QVBoxLayout>();                                  // главный компоновщик
    ChartLayout->addLayout(ChartWidgetLayout.release());
    ChartLayout->addWidget(Splitter.release(),1);


    std::unique_ptr<QWidget> centralWidget = std::make_unique<QWidget>(this);
    centralWidget->setLayout(ChartLayout.release());
    setCentralWidget(centralWidget.release());

    connect(BtnChangeDirectory.get(), &QPushButton::clicked, this, &MainWindow::changeDirectory);
    connect(TableFileView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::fileSelection);
    connect(TableFileView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::changeChartType);
    connect(ComboboxChartType.get(), &QComboBox::currentTextChanged, this, &MainWindow::changeChartType);
    connect(BtnPrintChart.get(), &QPushButton::clicked, this, &MainWindow::printChart);
    connect(ChkbxBlackWhiteChart.get(), &QCheckBox::toggled, this, &MainWindow::colorSwap);
}

MainWindow::~MainWindow(){}

void MainWindow::changeDirectory()
{
    // Обработчик смены директории
    QString newDirectory = QFileDialog::getExistingDirectory(this, "Выбор папки", DirectoryPath);
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
    filePath = FileModel->filePath(indexes.first());            // Получаем путь к файлу, выбранному в модели

    QFile file(filePath);                                       // Создаем объект файла с указанным путем для проверки ошибок открытия/пустоты

    if (!file.open(QIODevice::ReadOnly)) {                      // Если не удалось открыть файл для чтения
        exceptionCall("Ошибка", "Невозможно открыть файл");     // Вызываем функцию обработки исключения с сообщением об ошибке
        file.close();                                           // Закрытие файла в случае ошибки открытия, чтобы избежать утечки ресурсов.
        return;                                                 // Возвращаемся из функции
    }

    if (file.size() == 0) {                                     // Проверка размера файла
        exceptionCall("Пустой файл", "Выбранный файл пустой");  // Файл пустой, обработка исключения
        isChartActive = false;
        file.close();
        return;
    }


    if (filePath.endsWith(".json"))                                 // если выбран json файл,
        Container.RegisterInstance<IFileReader, JsonFileReader>();  // то регистрируем IFileReader как JsonFileReader

    else if (filePath.endsWith(".sqlite"))                          // если выбран sql файл,
        Container.RegisterInstance<IFileReader, SqlFileReader>();   // то регистрируем IFileReader как SqlFileReader

    else if (filePath.endsWith(".csv"))
        Container.RegisterInstance<IFileReader, CsvFileReader>();

    auto Strategy = Container.GetObject<IFileReader>();             // получили соотвествующую стратегию через иок контейнер
    FileReader fileReader(Strategy);                                // установили эту стратегию для чтения
    fileData = fileReader.getData(filePath);                        // с помощью стратегии читаем данные из выбранного файла
    BtnPrintChart->setEnabled(true);
}

void MainWindow::changeChartType()
{
    // Обработчик изменения типа диаграммы

    // Определяем тип диаграммы на основе выбранного значения в ComboBox
    if (ComboboxChartType->currentText()  == "Столбчатая") {
        Container.RegisterInstance<Chart, BarChart>();
        isChartActive = true;
    }
    else if (ComboboxChartType->currentText() == "Круговая") {
        Container.RegisterInstance<Chart, PieChart>();
        isChartActive = true;
    }
    else if (ComboboxChartType->currentText() == "Линейная") {
        Container.RegisterInstance<Chart, LineChart>();
        isChartActive = true;
    }
    // Рисуем график
   if (isChartActive) {
       Container.GetObject<Chart>()->createChart(fileData,ChartView);
   }
}

void MainWindow::printChart()
{
    // Обработчик печати диаграммы
    if (isChartActive) {
        QString filePath = QFileDialog::getSaveFileName(nullptr, "Сохранить PDF", "", "*.pdf");
        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        // Создание печатного устройства и установка писателя в него
        QPainter painter(&pdfWriter);
        // Отрисовка графика
        ChartView->render(&painter);
        painter.end();

    }else
        exceptionCall("Файл не выбран", "Пожалуйста, выберите файл");
}

void MainWindow::colorSwap()
{
    // Обработчик смены цвета диаграммы
    if (ChkbxBlackWhiteChart->isChecked()) {
        // Установить черно-белый эффект для диаграммы как при печати
        std::unique_ptr<QGraphicsColorizeEffect>colorizeEffect = std::make_unique<QGraphicsColorizeEffect>();
        colorizeEffect->setColor(Qt::black);
        ChartView->chart()->setGraphicsEffect(colorizeEffect.release());
    } else {
        // Отключить эффекты для возврата исходных цветов диаграммы
        ChartView->chart()->setGraphicsEffect(nullptr);
    }

}

void MainWindow::exceptionCall(QString title, QString message)
{
    // Вызов диалогового окна с сообщением об ошибке
    QMessageBox::critical(this, title, message);
}
