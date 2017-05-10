#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QLineEdit>

class NewFileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewFileDialog(QWidget *parent = 0);

    QString fileName() const;

private:
    QLineEdit *m_lineEdit;
};

#endif // NEWFILEDIALOG_H
