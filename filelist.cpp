#include "filelist.h"
#include <iostream>

FileList::FileList(const std::vector<File> &fs, QObject *parent)
    : QAbstractListModel(parent)
{
    Files = fs;
}

int FileList::rowCount(const QModelIndex &parent) const
{
    return Files.size();
}

QVariant FileList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= int(Files.size())) return QVariant();

    if (role == Qt::DisplayRole) {
        return QVariant(QString::fromStdString(Files.at(index.row()).filename));
    } else {
        return QVariant();
    }
}

bool FileList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;
    if (index.row() >= int(Files.size())) return false;
    if (role == Qt::EditRole) {
        for (auto const& val : Files) {
            if (val.filename == value.toString().toStdString()) {
                return false;
            }
        }
        if (value.toString().toStdString() == "") {
            return false;
        }
        Files.at(index.row()).filename = value.toString().toStdString();
        emit editCompleted(QString::fromStdString(Files.at(index.row()).filename));
    }
    return true;
}

Qt::ItemFlags FileList::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractListModel::flags(index);
}

bool FileList::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count);
    File f;
    auto RI = Files.begin();
    RI += row;
    Files.insert(RI,uint(count), f);
    endInsertRows();
    return true;
}

bool FileList::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count);
    if (row + count > Files.size()) {
        endRemoveRows();
        return false;
    }
    if (row < 0) {
        endRemoveRows();
        return false;
    }
    auto RI = Files.begin();
    RI += row;
    Files.erase(RI, RI+count);
    endRemoveRows();
    return true;
}

bool FileList::Remove(QModelIndex &index)
{
    if (index.row() >= Files.size()) {
        endRemoveRows();
        return false;
    }
    auto RI = Files.begin();
    RI += index.row();
    Files.erase(RI);
}

QVariant FileList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(QString("Files"));
    return QVariant();
}

bool FileList::addFile(File f)
{
    for (auto const& val : Files) {
        if (val.filename == f.filename) {
            std::cout << val.filename << " " << f.filename << std::endl;
            return false;
        }
    }
    Files.push_back(f);
    QModelIndex tl = index(0,0), lr = index(0, Files.size()-1);
    emit dataChanged(tl,lr);
    return true;
}

File* FileList::at(int index) {
    if (index >= Files.size()) {
        return nullptr;
    }
    return &Files.at(index);
}
