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


void MainWindow::on_pushButton_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);
    QString path = fileDialog->getExistingDirectory(this);
    QDirIterator it(path, {"*.png",  "*.jpg",  "*.bmp", "*.gif",  "*.tiff", "*.pcx"} , QDir::Files);
    info.clear();
    while (it.hasNext())
    {
        QString filePath = it.next();
        pictureInfo curPictureInfo;
        curPictureInfo.fileName = it.fileName();
        QImage image(it.path() + "/" + it.fileName());
        curPictureInfo.size = QString::number(image.size().width()) + "x"
                              + QString::number(image.size().height());
        curPictureInfo.resolution = QString::number(int(image.dotsPerMeterX() * 0.0254)) + "x"
                                    + QString::number(int(image.dotsPerMeterY() * 0.0254));
        curPictureInfo.colorDepth = QString::number(image.depth());
        double mustSize = double(image.size().width()) * image.size().height() * image.depth();
        mustSize /= 8.0;
        QFile file(it.path() + "/" + it.fileName());
        double compression = ((file.size() - mustSize) / mustSize) * 100;
        curPictureInfo.compression = QString::number(compression) + "%";
        info.push_back(curPictureInfo);
    }
    updateTable();
}

void MainWindow::updateTable()
{
    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);
    for (int i = 0; i < info.size(); i++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        QTableWidgetItem *item1 = new QTableWidgetItem(info[i].fileName);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(info[i].size);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem(info[i].resolution);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, item3);
        QTableWidgetItem *item4 = new QTableWidgetItem(info[i].colorDepth);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, item4);
        QTableWidgetItem *item5 = new QTableWidgetItem(info[i].compression);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, item5);
    }
}
