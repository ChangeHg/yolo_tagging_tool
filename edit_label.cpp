#include "edit_label.h"
#include "ui_edit_label.h"

#include <QMessageBox>

EditLabel::EditLabel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::edit_label)
{
    ui->setupUi(this);
    cfg = nullptr;
    curr_index = -1;
}

EditLabel::~EditLabel()
{
    delete ui;
}

void EditLabel::set_config(Config *conf)
{
    cfg = conf;
}

void EditLabel::on_add_clicked()
{
    if (cfg == nullptr) return;
    if (ui->label->text().length() == 0) {
        QMessageBox::warning(this, "提示", "请输入内容", QMessageBox::Yes);
        return;
    }
    curr_index = cfg->names.indexOf(ui->label->text());
    if (curr_index < 0) {
        cfg->names.append(ui->label->text());
        curr_index = cfg->names.size() - 1;
    }
    emit new_class_id(curr_index);
    curr_index = -1;
    hide();
}

void EditLabel::closeEvent(QCloseEvent *event)
{
    emit new_class_id(curr_index);
    QWidget::closeEvent(event);
}

void EditLabel::showEvent(QShowEvent *event)
{
    ui->label_list->clear();
    if (cfg) {
        foreach (const QString &str, cfg->names) {
            ui->label_list->addItem(str);
        }
    }
    QWidget::showEvent(event);
}


void EditLabel::on_label_list_itemClicked(QListWidgetItem *item)
{
    ui->label->setText(item->text());
}


void EditLabel::on_label_list_itemDoubleClicked(QListWidgetItem *item)
{
    curr_index = ui->label_list->row(item);
    emit new_class_id(curr_index);
    curr_index = -1;
    hide();
}

