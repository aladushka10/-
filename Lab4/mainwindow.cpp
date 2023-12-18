#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QValidator>
#include <QInputDialog>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIntValidator* validator = new QIntValidator(-100, 100, this);
    ui->firstX->setValidator(validator);
    ui->secondX->setValidator(validator);
    ui->firstY->setValidator(validator);
    ui->secondY->setValidator(validator);
    plot = new CustomPlot(this->width(), this->height(), this);
    ui->firstX->setVisible(false);
    ui->firstXLabel->setVisible(false);
    ui->firstY->setVisible(false);
    ui->firstYLabel->setVisible(false);
    ui->radius->setVisible(false);
    ui->radiusLabel->setVisible(false);
    ui->draw->setVisible(false);
    ui->secondX->setVisible(false);
    ui->secondXLabel->setVisible(false);
    ui->secondY->setVisible(false);
    ui->secondYLabel->setVisible(false);
}

void MainWindow::showFirstTypeButtons()
{
    ui->firstX->setVisible(true);
    ui->firstXLabel->setVisible(true);
    ui->firstY->setVisible(true);
    ui->firstYLabel->setVisible(true);
    ui->secondX->setVisible(true);
    ui->secondXLabel->setVisible(true);
    ui->secondY->setVisible(true);
    ui->secondYLabel->setVisible(true);
    ui->draw->setVisible(true);
    ui->radius->setVisible(false);
    ui->radiusLabel->setVisible(false);
}

void MainWindow::showSecondTypeButtons()
{
    ui->firstX->setVisible(true);
    ui->firstXLabel->setVisible(true);
    ui->firstY->setVisible(true);
    ui->firstYLabel->setVisible(true);
    ui->radius->setVisible(true);
    ui->radiusLabel->setVisible(true);
    ui->draw->setVisible(true);
    ui->secondX->setVisible(false);
    ui->secondXLabel->setVisible(false);
    ui->secondY->setVisible(false);
    ui->secondYLabel->setVisible(false);
}

void MainWindow::on_bline_clicked() { algo = ChosenAlgo::BresenhamLine; showFirstTypeButtons(); }

void MainWindow::on_ddaline_clicked() { algo = ChosenAlgo::DDALine; showFirstTypeButtons(); }

void MainWindow::on_bcircle_clicked() { algo = ChosenAlgo::BresenhamCircle; showSecondTypeButtons(); }

void MainWindow::on_nline_clicked() { algo = ChosenAlgo::NativeLine; showFirstTypeButtons(); }

void MainWindow::on_wuline_clicked() { algo = ChosenAlgo::WuLine; showFirstTypeButtons(); }

void MainWindow::on_changeScale_clicked()
{
    bool ok;
    int newSize = QInputDialog::getInt(this, "Изменение параметров",
                   "Введите новый размер клетки", 25, 10, width() / 10, 1, &ok);
    if (ok)
        plot->setCellSize(newSize);
}

void MainWindow::BresenhamLine(int x0, int y0, int x1, int y1)
{
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int deltaX = abs(x1 - x0);
    int deltaY = abs(y1 - y0);
    int error = 0;
    int deltaerr = (deltaY + 1);
    int y = y0;
    int diry = (y1 - y0) / abs(y1 - y0);
    for (int x = x0; x <= x1; ++x)
    {
        plot->addPixel({x, y, Qt::black});
        error += deltaerr;
        if (error >= deltaX + 1)
        {
            y += diry;
            error -= deltaX + 1;
        }
    }
}

void MainWindow::BresenhamCircle(int x0, int y0, int r)
{
    int x = 0;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;
    while (y >= x)
    {
        plot->addPixel({x0 + x, y0 + y, Qt::black});
        plot->addPixel({x0 + x, y0 - y, Qt::black});
        plot->addPixel({x0 - x, y0 + y, Qt::black});
        plot->addPixel({x0 - x, y0 - y, Qt::black});
        plot->addPixel({x0 + y, y0 + x, Qt::black});
        plot->addPixel({x0 + y, y0 - x, Qt::black});
        plot->addPixel({x0 - y, y0 + x, Qt::black});
        plot->addPixel({x0 - y, y0 - x, Qt::black});
        error = 2 * (delta + y) - 1;
        if (delta < 0 && error <= 0)
            delta += 2 * ++x + 1;
        else
            if (delta > 0 && error > 0)
                delta -= 2 * --y + 1;
            else
                delta += 2 * (++x - --y);
    }
}

void MainWindow::DDALine(int x0, int y0, int x1, int y1)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    int step = std::max(abs(dx), abs(dy));
    dx /= step;
    dy /= step;
    double x = x0;
    double y = y0;
    int i = 0;
    while (i <= step)
    {
        plot->addPixel({int(round(x)), int(round(y)), Qt::black});
        x += dx;
        y += dy;
        ++i;
    }
}

void MainWindow::NativeLine(int x0, int y0, int x1, int y1)
{
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    double dx = x1 - x0;
    double dy = y1 - y0;
    if(dx >= 1)
        for (int x = x0; x <= x1; ++x)
        {
            int y = round(y0 + dy * (x - x0) / dx);
            plot->addPixel({x, y, Qt::black});
        }
    if(dy >= 1)
        for (int y = y0; y <= y1; ++y)
        {
            int x = round(x0 + dx * (y - y0) / dy);
            plot->addPixel({x, y, Qt::black});
        }
}

void MainWindow::WuLine(int x0, int y0, int x1, int y1)
{
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    double dx = x1 - x0;
    double dy = y1 - y0;
    double gradient;
    if (dx == 0)
        gradient = 1;
    else
        gradient = dy / dx;
    double xend = x0;
    double yend = y0 + gradient * (xend - x0);
    double xgap = 0.5;
    int xpxl1 = xend;
    int ypxl1 = int(yend);
    if (steep)
    {
        double coeff = ((yend - int(yend)) * xgap);
        plot->addPixel({ypxl1, xpxl1, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
        coeff = 1 - ((yend - int(yend)) * xgap);
        plot->addPixel({ypxl1 + 1, xpxl1, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
    }
    else
    {
        double coeff = ((yend - int(yend)) * xgap);
        plot->addPixel({xpxl1, ypxl1, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
        coeff = 1 - ((yend - int(yend)) * xgap);
        plot->addPixel({xpxl1 + 1, ypxl1, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
    }
    double intery = yend + gradient;
    xend = x1;
    yend = y1 + gradient * (xend - x1);
    xgap = 0.5;
    int xpxl2 = xend;
    int ypxl2 = int(yend);
    if (steep)
    {
        double coeff = ((yend - int(yend)) * xgap);
        plot->addPixel({ypxl2, xpxl2, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
        coeff = 1 - ((yend - int(yend)) * xgap);
        plot->addPixel({ypxl2 + 1, xpxl2, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
    }
    else
    {
        double coeff = ((yend - int(yend)) * xgap);
        plot->addPixel({xpxl2, ypxl2, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
        coeff = 1 - ((yend - int(yend)) * xgap);
        plot->addPixel({xpxl2 + 1, ypxl2, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
    }
    if (steep)
    {
        for (int x = xpxl1 + 1; x < xpxl2; ++x)
        {
            double coeff = (intery - int(intery));
            plot->addPixel({int(intery), x, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
            coeff = 1 - intery - int(intery);
            plot->addPixel({int(intery) + 1, x, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
            intery += gradient;
        }
    }
    else
    {
        for (int x = xpxl1 + 1; x < xpxl2; ++x)
        {
            double coeff = (intery - int(intery));
            plot->addPixel({x, int(intery), qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
            coeff = 1 - intery - int(intery);
            plot->addPixel({x, int(intery) + 1, qRgb(255 * coeff, 255 * coeff, 255 * coeff)});
            intery += gradient;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_draw_clicked()
{
    plot->clearPixels();
    switch(algo)
    {
    case ChosenAlgo::None:
        break;
    case ChosenAlgo::BresenhamCircle:
        BresenhamCircle(ui->firstX->text().toInt(), ui->firstY->text().toInt(),
                        ui->radius->text().toInt());
        break;
    case ChosenAlgo::BresenhamLine:
        BresenhamLine(ui->firstX->text().toInt(), ui->firstY->text().toInt(),
                      ui->secondX->text().toInt(), ui->secondY->text().toInt());
        break;
    case ChosenAlgo::DDALine:
        DDALine(ui->firstX->text().toInt(), ui->firstY->text().toInt(),
                ui->secondX->text().toInt(), ui->secondY->text().toInt());
        break;
    case ChosenAlgo::NativeLine:
        NativeLine(ui->firstX->text().toInt(), ui->firstY->text().toInt(),
                   ui->secondX->text().toInt(), ui->secondY->text().toInt());
        break;
    case ChosenAlgo::WuLine:
        WuLine(ui->firstX->text().toInt(), ui->firstY->text().toInt(),
               ui->secondX->text().toInt(), ui->secondY->text().toInt());
        break;
    default:
        break;
    }
    QWidget::update();
}

