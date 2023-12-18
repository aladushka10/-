#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <fstream>
#include <vector>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->draw->setVisible(false);
    plot = new CustomPlot(this->width(), this->height(), this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_changeScale_clicked()
{
    bool ok;
    int newSize = QInputDialog::getInt(this, "Изменение параметров",
                   "Введите новый размер клетки", 25, 10, width() / 10, 1, &ok);
    if (ok)
        plot->setCellSize(newSize);
}

void MainWindow::on_segments_clicked()
{
    ui->draw->setVisible(true);
}

void MainWindow::on_polygons_clicked()
{
    ui->draw->setVisible(true);
}

void MainWindow::on_draw_clicked()
{
    plot->clearLines();
    if (ui->segments->isChecked())
    {
        processSegments();
    }
    else
    {
        processPolygons();
    }
    QWidget::update();
}

int getCode(QPointF winLeft, QPointF winRight, QPointF point)
{
    int code = 0;
    if (point.y() > winRight.y())
        code += 8;
    if (point.y() < winLeft.y())
        code += 4;
    if (point.x() > winRight.x())
        code += 2;
    if (point.x() < winLeft.x())
        code += 1;
    return code;
}

void MainWindow::processSegments()
{
    std::ifstream fin("/Users/test/Downloads/Lab5/segments.txt");
    int n;
    if (!(fin >> n))
        return ;
    std::vector <std::pair<QPointF, QPointF> > segments;
    for (int i = 0; i < n; ++i)
    {
        double x1, y1, x2, y2;
        fin >> x1 >> y1 >> x2 >> y2;
        segments.push_back( {QPointF(x1, y1), QPointF(x2, y2)} );
    }
    double winX1, winY1, winX2, winY2;
    fin >> winX1 >> winY1 >> winX2 >> winY2;
    QPointF winLeft(winX1, winY1);
    QPointF winRight(winX2, winY2);
    plot->addLine( {QPointF(winX1, winY1), QPointF(winX2, winY1), QColor::fromRgb(0, 128, 0)} );
    plot->addLine( {QPointF(winX1, winY1), QPointF(winX1, winY2), QColor::fromRgb(0, 128, 0)} );
    plot->addLine( {QPointF(winX2, winY2), QPointF(winX1, winY2), QColor::fromRgb(0, 128, 0)} );
    plot->addLine( {QPointF(winX2, winY2), QPointF(winX2, winY1), QColor::fromRgb(0, 128, 0)} );
    for (int i = 0; i < n; ++i)
    {
        QPointF first = segments[i].first;
        QPointF second = segments[i].second;
        int firstCode = getCode(winLeft, winRight, first);
        int secondCode = getCode(winLeft, winRight, second);
        while (firstCode | secondCode)
        {
            QPointF cur;
            int code;
            if (firstCode & secondCode)
            {
                plot->addLine( {first, second, QColor::fromRgb(238, 130, 238)} );
                break;
            }
            if (firstCode)
            {
                code = firstCode;
                cur = first;
            }
            else
            {
                code = secondCode;
                cur = second;
            }
            if (code & 1)
            {
                cur.setY(cur.y() + (first.y() - second.y()) *
                        (winX1 - cur.x()) / (first.x() - second.x()));
                cur.setX(winX1);
            }
            else
                if (code & 2)
                {
                    cur.setY(cur.y() + (first.y() - second.y()) *
                            (winX2 - cur.x()) / (first.x() - second.x()));
                    cur.setX(winX2);
                }
                else
                    if (code & 4)
                    {
                        cur.setX(cur.x() + (first.x() - second.x()) *
                                (winY1 - cur.y()) / (first.y() - second.y()));
                        cur.setY(winY1);
                    }
                    else
                        if (code & 8)
                        {
                            cur.setX(cur.x() + (first.x() - second.x()) *
                                    (winY2 - cur.y()) / (first.y() - second.y()));
                            cur.setY(winY2);
                        }
            if (code == firstCode)
            {
                plot->addLine( {first, cur, QColor::fromRgb(238, 130, 238)} );
                first = cur;
                firstCode = getCode(winLeft, winRight, first);
            }
            else
            {
                plot->addLine( {second, cur, QColor::fromRgb(238, 130, 238)} );
                second = cur;
                secondCode = getCode(winLeft, winRight, second);
            }
        }
        if (!(firstCode & secondCode))
            plot->addLine( {first, second, QColor::fromRgb(255, 215, 0) } );
    }
    fin.close();
}

bool inside(QPointF p, QPointF q, int x, int y)
{
    return (q.x() - p.x()) * (y - p.y()) - (q.y() - p.y()) * (x - p.x()) >= 0;
}

QPointF intersect(QPointF first, QPointF second, QPointF third, QPointF fourth)
{
    double numX = (first.x() * second.y() - first.y() * second.x()) * (third.x() - fourth.x()) -
               (first.x() - second.x()) * (third.x() * fourth.y() - third.y() * fourth.x());
    double den = (first.x() - second.x()) * (third.y() - fourth.y()) -
              (first.y() - second.y()) * (third.x() - fourth.x());
    double numY = (first.x() * second.y() - first.y() * second.x()) * (third.y() - fourth.y()) -
               (first.y() - second.y()) * (third.x() * fourth.y() - third.y() * fourth.x());
    return QPointF(numX / den, numY / den);
}
//bool isInside(QPointF winLeft, QPointF winRight, QPointF )

void clip(std::vector <QPointF>& visible, QPointF first, QPointF second)
{
    std::vector <QPointF> temp;
    for (int i = 0; i < visible.size(); ++i)
    {
        QPointF curPoint = visible[i];
        QPointF nextPoint = visible[(i + 1) % visible.size()];
        ///позиции относительно отсекающей линии
        double curPos = (second.x() - first.x()) * (curPoint.y() - first.y()) -
                     (second.y() - first.y()) * (curPoint.x() - first.x());
        double nextPos = (second.x() - first.x()) * (nextPoint.y() - first.y()) -
                (second.y() - first.y()) * (nextPoint.x() - first.x());
        if (curPos < 0 && nextPos < 0)
        {
            ///обе внутри
            temp.push_back(nextPoint);
        }
        else
            if (curPos >= 0 && nextPos < 0)
            {
                ///первая снаружи
                temp.push_back(intersect(first, second, curPoint, nextPoint));
                temp.push_back(nextPoint);
            }
            else
                if (curPos < 0 && nextPos >= 0)
                {
                    ///вторая снаружи
                    temp.push_back(intersect(first, second, curPoint, nextPoint));
                }
    }
    visible = temp;
}

void MainWindow::processPolygons()
{
    std::ifstream fin("/Users/test/Downloads/Lab5/polygons.txt");
    int n;
    if (!(fin >> n))
        return ;
    std::vector <QPointF> points;
    for (int i = 0; i < n; ++i)
    {
        double x, y;
        fin >> x >> y;
        points.push_back(QPointF(x, y));
    }
    double winX1, winY1, winX2, winY2;
    fin >> winX1 >> winY1 >> winX2 >> winY2;
    QPointF winLeft(winX1, winY1);
    QPointF winRight(winX2, winY2);
    std::vector <QPointF> window;
    window.push_back(QPointF(winX1, winY1)); //Левая нижняя
    window.push_back(QPointF(winX1, winY2)); //Правая нижняя
    window.push_back(QPointF(winX2, winY2)); //Правая верхняя
    window.push_back(QPointF(winX2, winY1)); //Левая верхняя
    plot->addLine( {QPointF(winX1, winY1), QPointF(winX2, winY1),  QColor::fromRgb(0, 128, 0)} );
    plot->addLine( {QPointF(winX1, winY1), QPointF(winX1, winY2),  QColor::fromRgb(0, 128, 0)} );
    plot->addLine( {QPointF(winX2, winY2), QPointF(winX1, winY2),  QColor::fromRgb(0, 128, 0)} );
    plot->addLine( {QPointF(winX2, winY2), QPointF(winX2, winY1),  QColor::fromRgb(0, 128, 0)} );
    std::vector <QPointF> visible = points;

    for (int i = 0; i < 4; ++i)
    {
        clip(visible, window[i], window[(i + 1) % 4]);
    }

    for (int i = 0; i < points.size(); ++i)
    {
        plot->addLine({points[i], points[(i + 1) % points.size()], QColor::fromRgb(238, 130, 238)});
    }
    for (int i = 0; i < visible.size(); ++i)
    {
        plot->addLine({visible[i], visible[(i + 1) % visible.size()], QColor::fromRgb(255, 215, 0)});
    }
    fin.close();
}
