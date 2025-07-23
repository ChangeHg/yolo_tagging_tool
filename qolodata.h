#ifndef QOLODATA_H
#define QOLODATA_H

#include <QObject>

struct box_info {
    uint32_t class_id;
    double box[4];
};

struct item_data {
    QString name;
    QString path;
    QString ext;
    QVector<box_info> box;
};

class QoloData : public QObject
{
    Q_OBJECT
public:
    explicit QoloData(QObject *parent = nullptr);
    bool read(QString path);
    bool save(QString path);
    QVector<item_data *> data;
signals:
private:
    bool read_dir(QString path, QString new_path);
    bool data_dupl(QString name, QString path);
    bool read_txt(QString path);
    bool save_txt(QString path);
    QVector<box_info> txt_analysis(QString path, QString dir, QString name);
};

#endif // QOLODATA_H
