#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H
#include <QWidget>
#include <QPainter>
#include <vector>
#include <QPointF>

struct Line
{
    QPointF first;
    QPointF second;
    QColor color;
};

class CustomPlot : public QWidget
{
    Q_OBJECT
public:
    CustomPlot(int w, int h, QWidget *parent = nullptr);
    void setWidth(double w);
    void setHeight(int h);
    void setCellSize(int size);
    void clearLines();
    void addLine(const Line& pixel);
private:
    int plotWidth;
    int plotHeight;
    int cellSize;
    QColor plotColor = Qt::white;
    QColor axisColor = Qt::black;
    std::vector<Line> lines;

    void drawLines(QPainter& painter);
    void drawPlot(QPainter& painter);
    void paintEvent(QPaintEvent* event) override;
};

#endif // CUSTOMPLOT_H
