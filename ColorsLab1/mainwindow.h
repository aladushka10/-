#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_pushButton_clicked();

    void on_redEdit_textChanged(const QString &arg1);

    void on_greenEdit_textChanged(const QString &arg1);

    void on_blueEdit_textChanged(const QString &arg1);

    void on_LSlider_sliderReleased();

    void on_LSlider_sliderMoved(int position);

    void on_aSlider_sliderMoved(int position);

    void on_aSlider_sliderReleased();

    void on_bSlider_sliderReleased();

    void on_LEdit_editingFinished();

    void on_aEdit_editingFinished();

    void on_bEdit_editingFinished();

    void on_redSlider_sliderMoved(int position);

    void on_greenSlider_sliderMoved(int position);

    void on_blueSlider_sliderMoved(int position);

    void on_redSlider_sliderReleased();

    void on_greenSlider_sliderReleased();

    void on_blueSlider_sliderReleased();

    void on_cyanSlider_sliderMoved(int position);

    void on_magentaSlider_sliderMoved(int position);

    void on_yellowSlider_sliderMoved(int position);

    void on_keySlider_sliderMoved(int position);

    void on_cyanSlider_sliderReleased();

    void on_magentaSlider_sliderReleased();

    void on_yellowSlider_sliderReleased();

    void on_keySlider_sliderReleased();

    void on_cyanEdit_editingFinished();

    void on_magentaEdit_editingFinished();

    void on_yellowEdit_editingFinished();

    void on_keyEdit_editingFinished();

    void on_bSlider_sliderMoved(int position);

private:
    QColor color;
    void updateColor();
    void updateRGB();
    void updateCMYK();
    void updateLAB();
    void updateLABcolor();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
