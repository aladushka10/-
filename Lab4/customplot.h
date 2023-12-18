#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H
#include <QWidget>
#include <QPainter>
#include <vector>

struct Pixel
{
    int x;
    int y;
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
    void clearPixels();
    void addPixel(const Pixel& pixel);
private:
    int plotWidth;
    int plotHeight;
    int cellSize;
    QColor plotColor = Qt::white;
    QColor axisColor = Qt::black;
    std::vector<Pixel> pixels;

    void drawPixels(QPainter& painter);
    void drawPlot(QPainter& painter);
    void paintEvent(QPaintEvent* event) override;
};

#endif // CUSTOMPLOT_H
