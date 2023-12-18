#include "mydialog.h"

myDialog::myDialog(QWidget* pwgt): QDialog(pwgt, Qt::WindowTitleHint |
                                           Qt::WindowSystemMenuHint)
{
   value = new QLineEdit;
   QLabel* valueLabel = new QLabel("&Значение");
   valueLabel->setBuddy(value);
   QPushButton* ok = new QPushButton("&Oк");
   QPushButton* cancel = new QPushButton("&Отмена");
   connect(ok, SIGNAL(clicked()), SLOT(accept()));
   connect(cancel, SIGNAL(clicked()), SLOT(reject()));
   QGridLayout* ptopLayout = new QGridLayout;
   ptopLayout->addWidget(valueLabel, 0, 0);
   ptopLayout->addWidget(value, 0, 1);
   ptopLayout->addWidget(ok, 1,0);
   ptopLayout->addWidget(cancel, 1, 1);
   setLayout(ptopLayout);
}

QString myDialog::getValue() const
{
    return value->text();
}
