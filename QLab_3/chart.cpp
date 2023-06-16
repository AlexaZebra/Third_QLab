#include "chart.h"

void BarChart::setupTitle(QtCharts::QChartView* ChartView){
       ChartView->chart()->setTitle("Столбчатая диаграмма");
}

void BarChart::createSeries(QList<QPair<QString, qfloat16>> list, QtCharts::QChartView* ChartView) {
    std::unique_ptr<QtCharts::QBarSeries> series =  std::make_unique<QtCharts::QBarSeries>();
    for (int i = 0; i < list.count(); i++) {
        std::unique_ptr<QtCharts::QBarSet> set = std::make_unique<QtCharts::QBarSet>(list.at(i).first);
        * set << list.at(i).second;
        series->append(set.release());
    }
    ChartView->chart()->addSeries(series.release());

    ChartView->chart()->createDefaultAxes();
}

void PieChart::setupTitle(QtCharts::QChartView* ChartView){
       ChartView->chart()->setTitle("Круговая диаграмма");
}

void PieChart::createSeries(QList<QPair<QString, qfloat16>> list, QtCharts::QChartView* ChartView) {
    std::unique_ptr<QtCharts::QPieSeries> series =  std::make_unique<QtCharts::QPieSeries>();
    for (int i = 0; i < list.count(); i++) {
        series->append(list.at(i).first, list.at(i).second);
    }
    ChartView->chart()->addSeries(series.release());
}

void LineChart::setupTitle(QtCharts::QChartView* ChartView){
    ChartView->chart()->setTitle("Линейная диаграмма");
}

void LineChart::createSeries(QList<QPair<QString, qfloat16>> list, QtCharts::QChartView* ChartView) {
    std::unique_ptr<QtCharts::QLineSeries> series =  std::make_unique<QtCharts::QLineSeries>();
    for (int i = 0; i < list.count(); i++) {
        series->append(i, list.at(i).second);
    }
    ChartView->chart()->addSeries(series.release());
}
