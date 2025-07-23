#include "qolodata.h"

#include <QDir>

QoloData::QoloData(QObject *parent)
    : QObject{parent}
{

}

bool QoloData::read(QString path)
{
    if (read_dir(path, "train") == false) {
        qDebug() << "目录不存在: " << __func__;
        return false;
    }
    if (read_dir(path, "val") == false) {
        qDebug() << "目录不存在: " << __func__;
        return false;
    }
    if (read_txt(path) == false) {
        qDebug() << "txt分析失败: " << __func__;
        return false;
    }
    return true;
}

bool QoloData::save(QString path)
{
    if (save_txt(path) == false) {
        return false;
    }
    return true;
}

bool QoloData::read_dir(QString path, QString new_path)
{
    QString img_path = path + "/images/" + new_path;
    QDir dir(img_path);

    if (!dir.exists()) {
        qDebug() << "目录不存在: " << img_path;
        return false;
    }

    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList files = dir.entryList();
    foreach (const QString &file, files) {
        if (file.endsWith(".png", Qt::CaseInsensitive)
            || file.endsWith(".bmp", Qt::CaseInsensitive)
            || file.endsWith(".jpg", Qt::CaseInsensitive)) {
            QString file_name = file.chopped(4);
            QString ext_name = file.mid(file.size() - 3);
            if (data_dupl(file_name, new_path) == false) {
                item_data *new_data = new item_data;

                new_data->name = file_name;
                new_data->path = new_path;
                new_data->ext = ext_name;
                data.append(new_data);
            }
        }
    }

    return true;
}

bool QoloData::data_dupl(QString name, QString path)
{
    foreach (const item_data *t, data) {
        if (t->name == name && t->path == path) {
            return true;
        }
    }
    return false;
}

bool QoloData::read_txt(QString path)
{
    for (int i = 0; i < data.size(); i++) {
        item_data *td = data.at(i);
        td->box.clear();
        td->box = txt_analysis(path, td->path, td->name);
    }
    return true;
}

bool QoloData::save_txt(QString path)
{
    foreach (const item_data *td, data) {
        QFile txt_file(path + "/labels/" + td->path + "/" + td->name + ".txt");
        if (txt_file.open(QIODevice::WriteOnly | QIODevice::Text) == true) {
            foreach (const box_info &tb, td->box) {
                QByteArray line_data;
                line_data = QByteArray::number(tb.class_id);
                for (int i = 0; i < 4; i++) {
                    line_data += " " + QByteArray::number(tb.box[i]);
                }
                line_data += "\n";
                txt_file.write(line_data);
            }
            txt_file.close();
        }
    }
    return true;
}

QVector<box_info> QoloData::txt_analysis(QString path, QString dir, QString name)
{
    QVector<box_info> box;
    QFile txt_file(path + "/labels/" + dir + "/" + name + ".txt");
    if (txt_file.open(QIODevice::ReadOnly | QIODevice::Text) == true) {
        QByteArray txt_data = txt_file.readAll();
        txt_file.close();
        QList<QByteArray> line_datas = txt_data.split('\n');
        foreach (const QByteArray &line_data, line_datas) {
            QList<QByteArray> ld = line_data.split(' ');
            if (ld.size() == 5) {
                box_info tb;
                tb.class_id = ld[0].toUInt();
                tb.box[0] = ld[1].toDouble();
                tb.box[1] = ld[2].toDouble();
                tb.box[2] = ld[3].toDouble();
                tb.box[3] = ld[4].toDouble();
                box.append(tb);
            }
        }
        txt_file.close();
    }
    return box;
}
