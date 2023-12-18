#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImages(const cv::Mat& original, const cv::Mat& transformed)
{
    QImage originalImage((uchar*) original.data, original.cols, original.rows,
                         original.step, QImage::Format_Grayscale8);
    QPixmap originalPixmap;
    originalPixmap.convertFromImage(originalImage.scaled(500, 400));
    ui->original->setPixmap(originalPixmap);
    QImage transformedImage((uchar*) transformed.data, transformed.cols, transformed.rows,
                            transformed.step, QImage::Format_Grayscale8);
    QPixmap transformedPixmap;
    transformedPixmap.convertFromImage(transformedImage.scaled(500, 400));
    ui->transformed->setPixmap(transformedPixmap);
}

void MainWindow::on_openFile_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    QString fileName = fileDialog->getOpenFileName(this, tr("Open Image"),
                        "C:/", tr("Image Files (*.png *.jpg *.bmp *.gif *.tiff *.pcx)"));
    ui->curFile->setText("Cur File: " + fileName);
    image = cv::imread(fileName.toStdString(), cv::IMREAD_ANYDEPTH);
    updateImages(image, cv::Mat());
}

void MainWindow::on_histogram_clicked() // выравнивание гистограммы
{
    cv::Mat equalizedImage;
    cv::equalizeHist(image, equalizedImage);
    updateImages(image, equalizedImage);
}


void MainWindow::on_linearContrast_clicked() // линейное контрастирование
{
    double alpha = 1.5; // коэффициент контрастности
    int beta = 50; // смещение яркости
    cv::Mat contrastedImage = alpha * image + beta;
    updateImages(image, contrastedImage);
}


void MainWindow::on_operationPlus_clicked()
{
    int alpha = 70;
    myDialog* dialog = new myDialog;
    if (dialog->exec() == QDialog::Accepted)
    {
        QString value = dialog->getValue();
        alpha = value.toInt();
    }
    cv::Mat transformed = image + alpha;
    updateImages(image, transformed);
}


void MainWindow::on_operationMinus_clicked()
{
    int alpha = 40;
    myDialog* dialog = new myDialog;
    if (dialog->exec() == QDialog::Accepted)
    {
        QString value = dialog->getValue();
        alpha = value.toInt();
    }
    cv::Mat transformed = image - alpha;
    updateImages(image, transformed);
}


void MainWindow::on_operationMultiply_clicked()
{
    double alpha = 1.5;
    myDialog* dialog = new myDialog;
    if (dialog->exec() == QDialog::Accepted)
    {
        QString value = dialog->getValue();
        alpha = value.toDouble();
    }
    cv::Mat transformed = alpha * image;
    updateImages(image, transformed);
}


void MainWindow::on_operationDivide_clicked()
{
    double alpha = 1.3;
    myDialog* dialog = new myDialog;
    if (dialog->exec() == QDialog::Accepted)
    {
        QString value = dialog->getValue();
        alpha = value.toDouble();
    }
    cv::Mat transformed = image / alpha;
    updateImages(image, transformed);
}

