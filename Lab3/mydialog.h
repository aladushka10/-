#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class myDialog: public QDialog
{
private:
    QLineEdit* value;
public:
    myDialog(QWidget* pwgt = 0);
    QString getValue() const;
};

#endif // MYDIALOG_H
