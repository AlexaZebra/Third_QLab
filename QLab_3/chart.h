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

// Базовый класс шаблонного метода с инструкцией
class Chart {
public:
    void createChart(QList<QPair<QString, qfloat16>>& data, std::unique_ptr<QtCharts::QChartView>& ChartView, bool bwCheck){
        ChartView->chart()->removeAllSeries();              // Очистка окна рисования

        setupTitle(ChartView);                              // Установка заголовка диаграммы

        createSeries(data, ChartView, bwCheck);             // Создание диаграммы

        ChartView->setRenderHint(QPainter::Antialiasing);   // Рендеринг

        ChartView->update();                                // Обновление окна вывода
    };
    virtual ~Chart() = default;

protected:
    virtual void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)=0;
    virtual void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView, bool bwCheck)=0;

};

// Конкретные классы-диаграммы
class BarChart: public Chart {
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck)override;
    void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)override;
};

class PieChart: public Chart {
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck)override;
    void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)override;
};

class LineChart: public Chart {
protected:
    void createSeries(QList<QPair<QString, qfloat16>> data, std::unique_ptr<QtCharts::QChartView>& ChartView,bool bwCheck)override;
    void setupTitle(std::unique_ptr<QtCharts::QChartView>& ChartView)override;
};
#endif // CHART_H
