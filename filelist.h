#ifndef FILELIST_H
#define FILELIST_H

#include <QObject>
#include <QAbstractListModel>

typedef struct {
    std::string filename, contents;
} File;

class FileList : public QAbstractListModel {
    Q_OBJECT
public:
    explicit FileList(const std::vector<File>& fs, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override ;
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool Remove(QModelIndex&  index);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool addFile(File);
    File* at(int index);
private:
    std::vector<File> Files;
signals:
    void editCompleted(const QString &);
};

#endif // FILELIST_H
