#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <customplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_changeScale_clicked();
    void on_segments_clicked();
    void on_polygons_clicked();
    void on_draw_clicked();

private:
    Ui::MainWindow *ui;
    CustomPlot* plot;

    void processSegments();
    void processPolygons();
};
#endif // MAINWINDOW_H
