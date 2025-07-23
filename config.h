#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    bool read(QString path);
    bool save(QString path);
    QString train;
    QString val;
    QVector<QString> names;
    QString path;


signals:
private:
    void analysis(QByteArray data);
};

#endif // CONFIG_H
