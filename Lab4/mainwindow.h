#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <customplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class ChosenAlgo
{
    None,
    BresenhamLine,
    BresenhamCircle,
    NativeLine,
    DDALine,
    WuLine,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bline_clicked();
    void on_ddaline_clicked();
    void on_bcircle_clicked();
    void on_nline_clicked();
    void on_wuline_clicked();
    void on_changeScale_clicked();
    void on_draw_clicked();

private:
    Ui::MainWindow *ui;
    ChosenAlgo algo = ChosenAlgo::None;
    CustomPlot* plot;

    void showFirstTypeButtons();
    void showSecondTypeButtons();
    void draw();
    void BresenhamLine(int x0, int y0, int x1, int y1);
    void BresenhamCircle(int x0, int y0, int r);
    void DDALine(int x0, int y0, int x1, int y1);
    void NativeLine(int x0, int y0, int x1, int y1);
    void WuLine(int x0, int y0, int x1, int y1);
};
#endif // MAINWINDOW_H
