#ifndef CHART_H
#define CHART_H

#include "readfile.h"

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>

class Chart {
public:
    void createChart(QList<QPair<QString, qfloat16>>& data, std::unique_ptr<QtCharts::QChartView>& ChartView, bool bwCheck){
        // Очистка окна рисования
        ChartView->chart()->removeAllSeries();

        setupTitle(ChartView);

        createSeries(data, ChartView, bwCheck);

        ChartView->setRenderHint(QPainter::Antialiasing);

        // Обновление окна вывода
        ChartView->update();
    };
    virtual ~Chart() = default;

protected:
    virtual void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)=0;
    virtual void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView, bool bwCheck)=0;

};

class BarChart: public Chart {
public:
    ~BarChart() override = default;
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck)override;
    void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)override;
};

class PieChart: public Chart {
public:
    ~PieChart() override = default;
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck)override;
    void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)override;
};

class LineChart: public Chart {
public:
    ~LineChart() override = default;
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck)override;
    void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)override;
};
#endif // CHART_H
