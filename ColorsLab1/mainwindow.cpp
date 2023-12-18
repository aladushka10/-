#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QValidator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->redEdit->setValidator(new QIntValidator(0, 255));
    ui->greenEdit->setValidator(new QIntValidator(0, 255));
    ui->blueEdit->setValidator(new QIntValidator(0, 255));
    ui->cyanEdit->setValidator(new QIntValidator(0, 255));
    ui->magentaEdit->setValidator(new QIntValidator(0, 255));
    ui->yellowEdit->setValidator(new QIntValidator(0, 255));
    ui->keyEdit->setValidator(new QIntValidator(0, 255));
    ui->LEdit->setValidator(new QDoubleValidator(0, 100, 6));
    ui->aEdit->setValidator(new QDoubleValidator(-127, 128, 6));
    ui->bEdit->setValidator(new QDoubleValidator(-127, 128, 6));
    color = Qt::white;
    updateRGB();
    updateCMYK();
    updateLAB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateColor()
{
    ui->color->setStyleSheet("background-color: rgb(" + QString::number(color.red()) + ", " + QString::number(color.green()) +
                             ", " + QString::number(color.blue()) + ");");
}

void MainWindow::updateRGB()//из картинка в линия и слайдера
{
    ui->redEdit->setText(QString::number(color.red()));
    ui->greenEdit->setText(QString::number(color.green()));
    ui->blueEdit->setText(QString::number(color.blue()));
    ui->redSlider->setValue(color.red());
    ui->greenSlider->setValue(color.green());
    ui->blueSlider->setValue(color.blue());
}

void MainWindow::updateCMYK()//из картинка в линия и слайдера
{
    ui->cyanEdit->setText(QString::number(color.cyan()));
    ui->magentaEdit->setText(QString::number(color.magenta()));
    ui->yellowEdit->setText(QString::number(color.yellow()));
    ui->keyEdit->setText(QString::number(color.black()));
    ui->cyanSlider->setValue(color.cyan());
    ui->magentaSlider->setValue(color.magenta());
    ui->yellowSlider->setValue(color.yellow());
    ui->keySlider->setValue(color.black());
}

void MainWindow::updateLABcolor()//из линия в цвет
{
    double L = (ui->LEdit->text()).toDouble();
    double a = (ui->aEdit->text()).toDouble();
    double b = (ui->bEdit->text()).toDouble();
    double Xw = 95.047;
    double Yw = 100;
    double Zw = 108.883;
    double x1 = (L + 16.) / 116.;
    double x2 = a / 500 + x1;
    double x3 = x1 - b / 200;
    double X, Y, Z;
    if(x1 * x1 * x1 >= 0.008856)
        Y = x1 * x1 * x1 * Yw;
    else
        Y = ((x1 - 16./116) / 7.787) * Yw;
    if(x2 * x2 * x2 >= 0.008856)
        X = x2 * x2 * x2 * Xw;
    else
        X = ((x2 - 16./116) / 7.787) * Xw;
    if(x3 * x3 * x3 >= 0.008856)
        Z = x3 * x3 * x3 * Zw;
    else
        Z = ((x3 - 16./116) / 7.787) * Zw;
    double Rn, Gn, Bn;
    X /= 100;
    Y /= 100;
    Z /= 100;
    Rn = 3.2406 * X - 1.5372 * Y - 0.4986 * Z;
    Gn = -0.9689 * X + 1.8758 * Y + 0.0415 * Z;
    Bn = 0.0557 * X - 0.2040 * Y + 1.0570 * Z;
    double R = 0, G = 0, B = 0;
    if(Rn >= 0.0031308)
        R = (1.055 * qPow(Rn, 0.4166) - 0.055) * 255;
    else
        R = 12.92 * Rn * 255;
    if(Gn >= 0.0031308)
        G = (1.055 * qPow(Gn, 1./2.4) - 0.055) * 255;
    else
        G = 12.92 * Gn * 255;
    if(Bn >= 0.0031308)
        B = (1.055 * qPow(Bn, 1./2.4) - 0.055) * 255;
    else
        B = 12.92 * Bn * 255;
    color.setRed(int(R));
    color.setGreen(int(G));
    color.setBlue(int(B));
    updateColor();
    ui->redEdit->setText(QString::number(int(R)));
    ui->greenEdit->setText(QString::number(int(G)));
    ui->blueEdit->setText(QString::number(int(B)));
}

void MainWindow::updateLAB()//из картинка в линия
{
    double rn, gn, bn;
    if (1. * color.red() / 255 >= 0.04045)
        rn = qPow((1. * color.red() / 255 + 0.055) / 1.055, 2.4);
    else
        rn = (1. * color.red() / 255) / 12.52;
    rn *= 100;
    if (1. * color.green() / 255 >= 0.04045)
        gn = qPow((1. * color.green() / 255 + 0.055) / 1.055, 2.4);
    else
        gn = (1. * color.green() / 255) / 12.52;
    gn *= 100;
    if (1. * color.blue() / 255 >= 0.04045)
        bn = qPow((1. * color.blue() / 255 + 0.055) / 1.055, 2.4);
    else
        bn = (1. * color.blue() / 255) / 12.52;
    bn *= 100;
    double x = 0.412453 * rn + 0.35758 * gn + 0.180423 * bn;
    double y = 0.212671 * rn + 0.715160 * gn + 0.072169 * bn;
    double z = 0.019334 * rn + 0.119193 * gn + 0.950227 * bn;

    double Xw = 95.047;
    double Yw = 100;
    double Zw = 108.883;
    double L, a, b;
    double Fx, Fy, Fz;
    if(x / Xw >= 0.008856)
        Fx = cbrt(x / Xw);
    else
        Fx = 7.787 * (x / Xw) + 16. / 116;
    if(y / Yw >= 0.008856)
        Fy = cbrt(y / Yw);
    else
        Fy = 7.787 * (y / Yw) + 16. / 116;
    if(z / Zw >= 0.008856)
        Fz = cbrt(z / Zw);
    else
        Fz = 7.787 * (z / Zw) + 16. / 116;
    L = 116 * Fy - 16;
    a = 500 * (Fx - Fy);
    b = 200 * (Fy - Fz);
    ui->LEdit->setText(QString::number(L));
    ui->aEdit->setText(QString::number(a));
    ui->bEdit->setText(QString::number(b));
    ui->LSlider->setValue(L);
    ui->aSlider->setValue(a);
    ui->bSlider->setValue(b);
}

void MainWindow::on_pushButton_clicked()//выбрать цвет
{
    QColorDialog* dialog = new QColorDialog;
    color = dialog->getColor();
    updateColor();
    updateRGB();
    updateCMYK();
    updateLAB();
}


void MainWindow::on_redEdit_textChanged(const QString &arg1)//из линия в картинка и слайжера
{
    if (ui->redEdit->text().toInt() > 255)
        ui->redEdit->setText(QString::number(255));
    color.setRed((ui->redEdit->text()).toInt());
    updateColor();
    updateCMYK();
    updateLAB();
    ui->redSlider->setValue(color.red());
}


void MainWindow::on_greenEdit_textChanged(const QString &arg1)
{
    if (ui->greenEdit->text().toInt() > 255)
        ui->greenEdit->setText(QString::number(255));
    color.setGreen((ui->greenEdit->text()).toInt());
    updateColor();
    updateCMYK();
    updateLAB();
    ui->greenSlider->setValue(color.green());
}


void MainWindow::on_blueEdit_textChanged(const QString &arg1)
{
    if (ui->blueEdit->text().toInt() > 255)
        ui->blueEdit->setText(QString::number(255));
    color.setBlue((ui->blueEdit->text()).toInt());
    updateColor();
    updateCMYK();
    updateLAB();
    ui->blueSlider->setValue(color.blue());
}



void MainWindow::on_LSlider_sliderReleased()//из слайдера в текст и цвет при остановке
{
    ui->LEdit->setText(QString::number(ui->LSlider->value()));
    updateLABcolor();
    updateRGB();
    updateCMYK();
}


void MainWindow::on_LSlider_sliderMoved(int position)//из слайдера в текст при сдвиге
{
    ui->LEdit->setText(QString::number(ui->LSlider->value()));
}


void MainWindow::on_aSlider_sliderReleased()
{
    ui->aEdit->setText(QString::number(ui->aSlider->value()));
    updateLABcolor();
    updateRGB();
    updateCMYK();
}


void MainWindow::on_aSlider_sliderMoved(int position)
{
    ui->aEdit->setText(QString::number(ui->aSlider->value()));
}


void MainWindow::on_bSlider_sliderReleased()
{
    ui->bEdit->setText(QString::number(ui->bSlider->value()));
    updateLABcolor();
    updateRGB();
    updateCMYK();
}

void MainWindow::on_bSlider_sliderMoved(int position)
{
    ui->bEdit->setText(QString::number(ui->bSlider->value()));
}


void MainWindow::on_LEdit_editingFinished()//из текста в цвет и слайдер
{
    if (ui->LEdit->text().toDouble() > 100)
            ui->LEdit->setText(QString::number(100));
    updateLABcolor();
    updateRGB();
    updateCMYK();
    ui->LSlider->setValue(ui->LEdit->text().toDouble());
}


void MainWindow::on_aEdit_editingFinished()
{
    if (ui->aEdit->text().toDouble() < -128)
            ui->aEdit->setText(QString::number(-128));
    if (ui->aEdit->text().toDouble() > 127)
        ui->aEdit->setText(QString::number(127));
    updateLABcolor();
    updateRGB();
    updateCMYK();
    ui->aSlider->setValue(ui->aEdit->text().toDouble());
}


void MainWindow::on_bEdit_editingFinished()
{
    if (ui->bEdit->text().toDouble() < -128)
        ui->bEdit->setText(QString::number(-128));
    if (ui->bEdit->text().toDouble() > 127)
        ui->bEdit->setText(QString::number(127));
    updateLABcolor();
    updateRGB();
    updateCMYK();
    ui->bSlider->setValue(ui->bEdit->text().toDouble());
}


void MainWindow::on_redSlider_sliderMoved(int position)//когда сдвинули
{
    ui->redEdit->setText(QString::number(ui->redSlider->value()));
}


void MainWindow::on_greenSlider_sliderMoved(int position)
{
    ui->greenEdit->setText(QString::number(ui->greenSlider->value()));
}


void MainWindow::on_blueSlider_sliderMoved(int position)
{
    ui->blueEdit->setText(QString::number(ui->blueSlider->value()));
}


void MainWindow::on_redSlider_sliderReleased()//когда остановили
{
    ui->redEdit->setText(QString::number(ui->redSlider->value()));
    color.setRed(ui->redSlider->value());
    updateColor();
    updateCMYK();
    updateLAB();
}


void MainWindow::on_greenSlider_sliderReleased()
{
    ui->greenEdit->setText(QString::number(ui->greenSlider->value()));
    color.setGreen(ui->greenSlider->value());
    updateColor();
    updateCMYK();
    updateLAB();
}


void MainWindow::on_blueSlider_sliderReleased()
{
    ui->blueEdit->setText(QString::number(ui->blueSlider->value()));
    color.setBlue(ui->blueSlider->value());
    updateColor();
    updateCMYK();
    updateLAB();
}

void MainWindow::on_cyanSlider_sliderMoved(int position)
{
    ui->cyanEdit->setText(QString::number(ui->cyanSlider->value()));
}


void MainWindow::on_magentaSlider_sliderMoved(int position)
{
    ui->magentaEdit->setText(QString::number(ui->magentaSlider->value()));
}


void MainWindow::on_yellowSlider_sliderMoved(int position)
{
    ui->yellowEdit->setText(QString::number(ui->yellowSlider->value()));
}


void MainWindow::on_keySlider_sliderMoved(int position)
{
    ui->keyEdit->setText(QString::number(ui->keySlider->value()));
}


void MainWindow::on_cyanSlider_sliderReleased()
{
    ui->cyanEdit->setText(QString::number(color.cyan()));
    color.setCmyk(ui->cyanSlider->value(), color.magenta(), color.yellow(), color.black());
    updateColor();
    updateRGB();
    updateLAB();
}


void MainWindow::on_magentaSlider_sliderReleased()
{
    ui->magentaEdit->setText(QString::number(color.magenta()));
    color.setCmyk(color.cyan(), ui->magentaSlider->value(), color.yellow(), color.black());
    updateColor();
    updateRGB();
    updateLAB();
}


void MainWindow::on_yellowSlider_sliderReleased()
{
    ui->yellowEdit->setText(QString::number(color.yellow()));
    color.setCmyk(color.cyan(), color.magenta(), ui->yellowSlider->value(), color.black());
    updateColor();
    updateRGB();
    updateLAB();
}


void MainWindow::on_keySlider_sliderReleased()
{
    ui->keyEdit->setText(QString::number(color.black()));
    color.setCmyk(color.cyan(), color.magenta(), color.yellow(), ui->keySlider->value());
    updateColor();
    updateRGB();
    updateLAB();
}


void MainWindow::on_cyanEdit_editingFinished()
{
    if (ui->cyanEdit->text().toInt() > 255)
        ui->cyanEdit->setText(QString::number(255));
    color.setCmyk(ui->cyanEdit->text().toInt(), color.magenta(), color.yellow(), color.black());
    updateColor();
    updateRGB();
    updateLAB();
    ui->cyanSlider->setValue(color.cyan());
}


void MainWindow::on_magentaEdit_editingFinished()
{
    if (ui->magentaEdit->text().toInt() > 255)
        ui->magentaEdit->setText(QString::number(255));
    color.setCmyk(color.cyan(), ui->magentaEdit->text().toInt(), color.yellow(), color.black());
    updateColor();
    updateRGB();
    updateLAB();
    ui->magentaSlider->setValue(color.magenta());
}


void MainWindow::on_yellowEdit_editingFinished()
{
    if (ui->yellowEdit->text().toInt() > 255)
        ui->yellowEdit->setText(QString::number(255));
    color.setCmyk(color.cyan(), color.magenta(), ui->yellowEdit->text().toInt(), color.black());
    updateColor();
    updateRGB();
    updateLAB();
    ui->yellowSlider->setValue(color.yellow());
}


void MainWindow::on_keyEdit_editingFinished()
{
    if (ui->keyEdit->text().toInt() > 255)
        ui->keyEdit->setText(QString::number(255));
    color.setCmyk(color.cyan(), color.magenta(), color.yellow(), ui->keyEdit->text().toInt());
    updateColor();
    updateRGB();
    updateLAB();
    ui->keySlider->setValue(color.black());
}


