#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "mydialog.h"

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
    void on_openFile_clicked();

    void on_histogram_clicked();
    void on_linearContrast_clicked();

    void on_operationPlus_clicked();

    void on_operationMinus_clicked();

    void on_operationMultiply_clicked();

    void on_operationDivide_clicked();

private:
    Ui::MainWindow *ui;
    QString filePath;
    cv::Mat image;
    void updateImages(const cv::Mat& original, const cv::Mat& transformed);
};
#endif // MAINWINDOW_H
