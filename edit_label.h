#ifndef ENID_LABEL_H
#define ENID_LABEL_H

#include <QListWidget>
#include <QWidget>
#include "config.h"

namespace Ui {
class edit_label;
}

class EditLabel : public QWidget
{
    Q_OBJECT

public:
    explicit EditLabel(QWidget *parent = nullptr);
    ~EditLabel();
    void set_config(Config *conf);

private slots:
    void on_add_clicked();

    void on_label_list_itemClicked(QListWidgetItem *item);

    void on_label_list_itemDoubleClicked(QListWidgetItem *item);

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
signals:
    void new_class_id(int);

private:
    Ui::edit_label *ui;
    Config *cfg;
    int curr_index;
};

#endif // ENID_LABEL_H
