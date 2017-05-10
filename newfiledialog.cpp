#include "newfiledialog.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent)
{
    setStyleSheet("background-color : #222222;color : #888888;");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    m_lineEdit = new QLineEdit(this);

    QPushButton *searchButton = new QPushButton(tr("Create"));
    searchButton->setDefault(true);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(searchButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *lt = new QVBoxLayout;
    lt->addWidget(m_lineEdit);
    lt->addWidget(buttonBox);

    setLayout(lt);
}

QString NewFileDialog::fileName() const
{
    return m_lineEdit->text();
}
