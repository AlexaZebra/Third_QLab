#ifndef CHART_H
#define CHART_H

#include "readfile.h"

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>



class Chart {
public:
    void createChart(QList<QPair<QString, qfloat16>>& data, QtCharts::QChartView* ChartView){
        // Очистка окна рисования
        ChartView->chart()->removeAllSeries();

        setupTitle(ChartView);

        createSeries(data, ChartView);

        ChartView->setRenderHint(QPainter::Antialiasing);

        // Обновление окна вывода
        ChartView->update();
    };
    virtual ~Chart() = default;

protected:
    virtual void setupTitle(QtCharts::QChartView* ChartView)=0;
    virtual void createSeries(QList<QPair<QString, qfloat16>> data, QtCharts::QChartView* ChartView)=0;

};

class BarChart: public Chart {
public:
    ~BarChart() override = default;
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, QtCharts::QChartView* ChartView)override;
    void setupTitle(QtCharts::QChartView* ChartView)override;
};

class PieChart: public Chart {
public:
    ~PieChart() override = default;
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, QtCharts::QChartView* ChartView)override;
    void setupTitle(QtCharts::QChartView* ChartView)override;
};

#endif // CHART_H
