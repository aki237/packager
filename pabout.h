#ifndef PABOUT_H
#define PABOUT_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QDialog>

class PAbout : public QDialog
{
    Q_OBJECT
public:
    explicit PAbout(QWidget *parent = 0);
signals:

public slots:
private:
    QLabel* img;
    QLabel* label;
    QLabel* info;
};

#endif // PABOUT_H
