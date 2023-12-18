#include "customplot.h"

CustomPlot::CustomPlot(int w, int h, QWidget* parent) : QWidget(parent)
{
    resize(w - 400, h);
    plotWidth = w - 400;
    plotHeight = h;
    cellSize = 25;
}

void CustomPlot::setWidth(double w) { plotWidth = w; }

void CustomPlot::setHeight(int h) { plotHeight = h; }

void CustomPlot::setCellSize(int size) { cellSize = size; }

void CustomPlot::clearPixels() { pixels.clear(); }

void CustomPlot::addPixel(const Pixel& pixel) { pixels.push_back(pixel); }

void CustomPlot::drawPlot(QPainter& painter)
{
    QPen pen(axisColor);
    pen.setWidth(0.7);
    painter.setPen(pen);
    painter.setBrush(plotColor);
    int x = 0, y = 0;
    while (y <= plotHeight)
    {
        x = 0;
        while (x <= plotWidth)
        {
            painter.drawRect(x, y, cellSize, cellSize);
            x += cellSize;
        }
        y += cellSize;
    }
    int centerX = cellSize * (plotWidth / cellSize / 2);
    int centerY = cellSize * (plotHeight / cellSize / 2);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    ///y
    painter.drawLine(centerX, 0, centerX, plotHeight);
    painter.drawLine(centerX - cellSize / 2, cellSize / 2, centerX, 0);
    painter.drawLine(centerX + cellSize / 2, cellSize / 2, centerX, 0);
    painter.drawText(QRect(centerX + cellSize / 2, cellSize / 2, 10, 10), "y");
    y = 0;
    int i = (plotHeight / cellSize / 2);
    while(y <= plotHeight)
    {
        painter.drawLine(centerX - cellSize / 3, y, centerX + cellSize / 3, y);
        painter.drawText(QRect(centerX + 5, y - cellSize + 5, cellSize - 5, cellSize - 5), QString::number(i));
        y += cellSize;
        --i;
    }
    ///x
    painter.drawLine(0, centerY, plotWidth, centerY);
    painter.drawLine(plotWidth, centerY, plotWidth - cellSize / 2, centerY - cellSize / 2);
    painter.drawLine(plotWidth, centerY, plotWidth - cellSize / 2, centerY + cellSize / 2);
    painter.drawText(QRect(plotWidth - cellSize / 2, centerY + cellSize / 2, 10, 10), "x");
    x = 0;
    i = -(plotWidth / cellSize / 2);
    while(x <= plotWidth)
    {
        painter.drawLine(x, centerY - cellSize / 3, x, centerY + cellSize / 3);
        painter.drawText(QRect(x + 5, centerY, cellSize - 5, cellSize), QString::number(i));
        x += cellSize;
        ++i;
    }
}

void CustomPlot::drawPixels(QPainter& painter)
{
    QPen pen(axisColor);
    pen.setWidth(1.0);
    painter.setPen(pen);
    for (int i = 0; i < pixels.size(); ++i)
    {
        painter.setBrush(pixels[i].color);
        int curX = cellSize * (pixels[i].x + (plotWidth / cellSize / 2));
        int curY = cellSize * ((plotHeight / cellSize / 2) - pixels[i].y - 1);
        painter.drawRect(curX, curY, cellSize, cellSize);
    }
}

void CustomPlot::paintEvent(QPaintEvent*)
{
   // resize(width() - 420, height());
    QFont font;
    font.setBold(true);
    font.setPointSize(7);
    QPainter painter(this);
    painter.setFont(font);
    drawPlot(painter);
    drawPixels(painter);
}
