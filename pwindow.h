#ifndef PWINDOW_H
#define PWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "filelist.h"
#include "Dibba.hpp"

namespace Ui {
class PWindow;
}

class PWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PWindow(QWidget *parent = 0);
    ~PWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionAdd_File_triggered();

    void on_listView_clicked(const QModelIndex &index);

    void on_actionRemove_File_triggered();

    void on_listView_activated(const QModelIndex &index);

    void on_textEdit_textChanged();

    void on_actionQuit_triggered();

    void on_actionAdd_File_from_Disk_triggered();

    void on_actionAbout_triggered();

    void on_actionSet_Monospaced_Font_toggled(bool checked);

private:
    Ui::PWindow *ui;
    FileList* qslm;
    bool issaved;
    Dibba::Option<std::string> dibfile;
    void setTitle();
};

#endif // PWINDOW_H
