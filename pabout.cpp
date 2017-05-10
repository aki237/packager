#include "pabout.h"
#include <QVBoxLayout>

PAbout::PAbout(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    img = new QLabel;
    img->setPixmap(QPixmap(":/icon.png"));
    label = new QLabel("Packager");
    label->setStyleSheet("font : bold large;");
    info = new QLabel("A GUI Packager application for Dibba files.\n"
                      "© 2017 Akilan Elango All rights reserved.");
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(img);
    leftLayout->setAlignment(img,Qt::AlignCenter);
    leftLayout->addWidget(label);
    leftLayout->setAlignment(label,Qt::AlignCenter);
    leftLayout->addWidget(info);
    leftLayout->setAlignment(info,Qt::AlignCenter);
    leftLayout->addStretch();
    setLayout(leftLayout);
    setWindowTitle(tr("About"));
    setStyleSheet("background-color : #222222;color : #888888;padding : 10px;");
    setFixedHeight(sizeHint().height());
}
