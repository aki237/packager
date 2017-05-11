#include "pwindow.h"
#include "ui_pwindow.h"
#include "Dibba.hpp"
#include "newfiledialog.h"
#include "basename.hpp"
#include "pabout.h"
#include "file_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <QFileDialog>
#include <QMessageBox>

std::string base64_encode(const std::string &in) {
    std::string out;

    int val=0, valb=-6;
    for (uchar c : in) {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb-=6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

int RunDialog(QString text, QString more) {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setInformativeText(more);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setStyleSheet("background-color : #222222;color : #888888;");
    return msgBox.exec();
}

void PWindow::setTitle () {
    std::string title = "Packager - ";
    if (dibfile.IsSome())
        title += Basename::basename(dibfile.Get());
    else
        title += "Untitled";
    if (!issaved)
        title += "*";
    setWindowTitle(QString::fromStdString(title));
}

PWindow::PWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PWindow)
{
    ui->setupUi(this);
    ui->scrollArea->hide();
    ui->menuBar->setStyleSheet("background-color : #222222;color : #888888;");
    ui->textEdit->undoAvailable(false);
    ui->listView->setStyleSheet("QListView::item {padding : 10px;}"
                                "QListView::item:hover {background-color : #FEEDDC;}");
    ui->listView->setFixedWidth(150);
    ui->menuBar->setStyleSheet("QMenuBar::item{background-color : #222222;color : #888888;}"
                               "QMenuBar::item:selected {background-color : #666666;}");
    ui->menuFile->setStyleSheet("QMenu::item :selected {background-color : #666666;}");
    std::vector<File> files;
    qslm = new FileList(files,this);
    File author, doc;
    author.filename = "AUTHOR";
    doc.filename = "doc.md";
    qslm->addFile(author);
    qslm->addFile(doc);
    ui->listView->setModel(qslm);
    QModelIndex index = qslm->index( 0, 0);
    ui->listView->setCurrentIndex(index);
    issaved = true;
    setTitle();
}

PWindow::~PWindow()
{
    delete ui;
}

void PWindow::on_actionOpen_triggered()
{
    if (!issaved) {
        int ret = RunDialog("Some files are not saved",
                            "Are you sure you want to open a new one.");
        switch (ret) {
          case QMessageBox::Save:
            on_actionSave_triggered();
            break;
          case QMessageBox::Discard:
            break;
          case QMessageBox::Cancel:
            return;
        }
    }
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Dibba File"), "",
            tr("Dibba Files (*.dib);;All Files (*)"));
    if (fileName == "")
        return;
    std::cout << fileName.toStdString() << std::endl;
    std::vector<File> files;
    std::fstream inFile;
    inFile.open(fileName.toStdString(),std::ios_base::in);
    Dibba::DibbaReader rd(inFile);
    Dibba::Option<Dibba::Error> err = rd.Parse();
    if (err.IsSome()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error",QString::fromStdString(err.Get().What()));
        return;
    }
    std::vector<std::string> fileList = rd.FileList();
    for (std::string const& filename : fileList) {
        File tfile;
        tfile.filename = filename;
        Dibba::Option<std::string> opt = rd.Read(filename);
        if (!opt.IsSome()) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Unable to get the content from the Dibba file.");
            return;
        }
        tfile.contents = opt.Get();
        files.push_back(tfile);
    }
    qslm = new FileList(files,this);
    ui->listView->setModel(qslm);
    QModelIndex index = qslm->index(0, 0);
    ui->listView->setCurrentIndex(index);
    on_listView_activated(index);
    issaved = true;
    dibfile.Set(fileName.toStdString());
    setTitle();
}

void PWindow::on_actionSave_triggered()
{
    if (!issaved) {
        if (!dibfile.IsSome()){
            QString fileName =
                    QFileDialog::getSaveFileName(this,
                                                 tr("Save Dibba File"),
                                                 "",
                                                 tr("Dibba Files(*.dib);;All Files (*)"));
            if (fileName == "")
                return;
            dibfile.Set(fileName.toStdString());
            setTitle();
        }
        std::fstream outFile;
        outFile.open(dibfile.Get(),std::ios_base::out);
        Dibba::DibbaWriter dr(outFile);
        dr.Init();
        int i = 0;
        File* flst = qslm->at(i);
        while (flst != nullptr) {
            std::stringstream ss;
            ss << flst->contents;
            Dibba::File tempdFile(flst->filename, ss);
            dr.Add(tempdFile);
            i++;
            flst = qslm->at(i);
        }
        dr.Commit();
        issaved = true;
        setTitle();
    }
}

void PWindow::on_actionNew_triggered()
{
    if (!issaved) {
        int ret = RunDialog("Some files are not saved",
                            "Are you sure you want to open a new one.");
        switch (ret) {
          case QMessageBox::Save:
            on_actionSave_triggered();
            break;
          case QMessageBox::Discard:
            break;
          case QMessageBox::Cancel:
            return;
        }
    }
    std::vector<File> files;
    qslm = new FileList(files,this);
    File author, doc;
    author.filename = "AUTHOR";
    doc.filename = "doc.md";
    qslm->addFile(author);
    qslm->addFile(doc);
    ui->listView->setModel(qslm);
    QModelIndex index = qslm->index(0, 0);
    ui->listView->setCurrentIndex(index);
    std::cout << "Clearing the text view..." <<  std::endl;
    ui->textEdit->clear();
    std::cout << "Cleared the text view..." <<  std::endl;
    issaved = false;
}

void PWindow::on_actionAdd_File_triggered()
{
    File newFile;
    NewFileDialog *dialog = new NewFileDialog();

    if (dialog->exec()) {
        newFile.filename = dialog->fileName().toStdString();
        if (!qslm->addFile(newFile)) {
            std::cout << "Unable to add the file : " << newFile.filename << std::endl;
        }
    }
}

void PWindow::on_listView_activated(const QModelIndex &index)
{
    std::string op = qslm->at(index.row())->contents;
    if (op == "" || get_encoding(op.c_str(),op.length())){
        ui->scrollArea->hide();
        ui->textEdit->show();
        ui->textEdit->setPlainText(QString::fromStdString(op));
    } else {
        auto mime = get_mime_type(op.c_str(), op.length());
        if (mime.length() >= 5 && mime.substr(0,5) == "image"){
            ui->scrollArea->show();
            ui->textEdit->hide();
            QPixmap qpm;
            qpm.loadFromData((uchar*)(op.c_str()),uint(op.length()));
            ui->label->setPixmap(qpm);
            ui->label->setAlignment(Qt::AlignCenter);
        } else {
            ui->scrollArea->hide();
            ui->textEdit->show();
            ui->textEdit->setPlainText("Unable to show the content");
        }
    }
}

void PWindow::on_listView_clicked(const QModelIndex &index)
{
    on_listView_activated(index);
}

void PWindow::on_actionRemove_File_triggered()
{
    auto index = ui->listView->currentIndex();
    qslm->Remove(index);
}

void PWindow::on_textEdit_textChanged()
{
    issaved = false;
    setTitle();
    auto index = ui->listView->currentIndex();
    auto op = qslm->at(index.row())->contents;
    qslm->at(index.row())->contents = ui->textEdit->toPlainText()
            .toStdString();
    std::string lfle = qslm->at(index.row())->filename;
    if (lfle.length() >= 3 && lfle.substr(lfle.length() - 3) == ".md") {
        ui->textEdit->setHighlightingEnabled(true);
    }
}

void PWindow::on_actionQuit_triggered()
{
    if (!issaved) {
        int ret = RunDialog("Some files are not saved",
                            "Are you sure you want to open a new one.");
        switch (ret) {
          case QMessageBox::Save:
            on_actionSave_triggered();
            break;
          case QMessageBox::Discard:
            break;
          case QMessageBox::Cancel:
            return;
        }
    }
    this->close();
}

void PWindow::on_actionAdd_File_from_Disk_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File to add"), "",
            tr("All Files (*)"));
    if (fileName == "") {
        return;
    }
    std::string content = "";
    std::ifstream is(fileName.toStdString());

    char c;
    while (is.get(c)){
        content += c;
    }
    File tfle;
    tfle.filename = Basename::basename(fileName.toStdString());
    tfle.contents = content;
    qslm->addFile(tfle);
}

void PWindow::on_actionAbout_triggered()
{
    PAbout pabt;
    pabt.exec();
}

void PWindow::on_actionSet_Monospaced_Font_toggled(bool checked)
{
    if (checked) {
        ui->textEdit->setFont(QFont("monospace"));
    } else {
        ui->textEdit->setFont(QFont());
    }
}
