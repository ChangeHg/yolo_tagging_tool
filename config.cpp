#include "config.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Config::Config(QObject *parent)
    : QObject{parent}
{


}

bool Config::read(QString json_path)
{
    QFile json_file(json_path);
    if (json_file.open(QIODevice::ReadOnly | QIODevice::Text) == true) {
        analysis(json_file.readAll());
        json_file.close();
        return true;
    } else {
        return false;
    }
}

bool Config::save(QString json_path)
{
    QJsonObject root;

    root.insert("train", QJsonValue(train));
    root.insert("val", QJsonValue(val));
    root.insert("nc", QJsonValue(names.size()));
    QJsonArray name_array;
    for (int i = 0; i < names.size(); i++) {
        name_array.append(QJsonValue(names.at(i)));
    }
    root.insert("names", QJsonValue(name_array));
    root.insert("path", QJsonValue(path));


    QFile json_file(json_path);
    if(json_file.open(QIODevice::WriteOnly | QIODevice::Text) == true) {
        QJsonDocument doc(root);
        json_file.write(doc.toJson(QJsonDocument::Compact));
        json_file.close();
        return true;
    } else {
        return false;
    }
}

void Config::analysis(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qDebug() << "json 数据解析出错";
        return;
    }
    QJsonObject root = doc.object();
    train = root.value("train").toString();
    val = root.value("val").toString();
    names.clear();
    QJsonArray name_array= root.value("names").toArray();
    for (int i = 0; i < name_array.size(); i++) {
        names.append(name_array.at(i).toString());
    }
    path = root.value("path").toString();
}
