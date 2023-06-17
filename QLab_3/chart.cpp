#include "chart.h"

void BarChart::setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView){
       ChartView->chart()->setTitle("Столбчатая диаграмма");
}

void BarChart::createSeries(QList<QPair<QString, qfloat16>> list, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck) {
    std::unique_ptr<QtCharts::QBarSeries> series =  std::make_unique<QtCharts::QBarSeries>();
    for (int i = 0; i < list.count(); i++) {
        std::unique_ptr<QtCharts::QBarSet> set = std::make_unique<QtCharts::QBarSet>(list.at(i).first);
        * set << list.at(i).second;
        if (bwCheck)
        {
            // меняем цвет на чб, используя рандомную раскладку оттенков серого от белого к черному
            int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            // присваиваем элементам диаграммы эти цветовые данные
            set->setBrush(QColor(bw, bw, bw));
        }
        series->append(set.release());
    }
    ChartView->chart()->addSeries(series.release());

    ChartView->chart()->createDefaultAxes();
}

void PieChart::setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView){
       ChartView->chart()->setTitle("Круговая диаграмма");
}

void PieChart::createSeries(QList<QPair<QString, qfloat16>> list, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck) {
    std::unique_ptr<QtCharts::QPieSeries> series =  std::make_unique<QtCharts::QPieSeries>();
    for (int i = 0; i < list.count(); i++) {
        QtCharts::QPieSlice* slice = series->append(list.at(i).first, list.at(i).second);
        if (bwCheck)
        {
            // меняем цвет на чб, используя рандомную раскладку оттенков серого от белого к черному
            int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            // присваиваем элементам диаграммы эти цветовые данные
            slice->setBrush(QColor(bw, bw, bw));
        }
    }
    ChartView->chart()->addSeries(series.release());
}

void LineChart::setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView){
    ChartView->chart()->setTitle("Линейная диаграмма");
}

void LineChart::createSeries(QList<QPair<QString, qfloat16>> list, std::unique_ptr<QtCharts::QChartView>& ChartView, bool bwCheck) {
    std::unique_ptr<QtCharts::QLineSeries> series = std::make_unique<QtCharts::QLineSeries>();
    for (int i = 0; i < list.count(); i++) {
        series->append(i, list.at(i).second);
        if (bwCheck) {
            // Генерируем случайный цвет в оттенках серого от белого к черному
            int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            // Устанавливаем цвет серии данных
            series->setColor(QColor(bw, bw, bw));
        }
    }
    ChartView->chart()->addSeries(series.release());
}
