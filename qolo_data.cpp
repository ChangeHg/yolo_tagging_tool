#include "qolo_data.h"
#include "ui_qolo_data.h"

#include <QFileDialog>
#include <QMessageBox>

qolo_data::qolo_data(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::qolo_data)
{
    ui->setupUi(this);
    view = new CustomView(this);
    view->setScene(&scene);
    view->setCursor(Qt::CrossCursor);

    connect(view, SIGNAL(press_mouse(QPointF)), this, SLOT(view_press_mouse(QPointF)));
    connect(view, SIGNAL(release_mouse(QPoint)), this, SLOT(view_release_mouse(QPoint)));
    connect(view, SIGNAL(move_mouse(QPointF)), this, SLOT(view_move_mouse(QPointF)));
    ui->g_layout->insertWidget(0, view);



    add_win = new EditLabel(nullptr);
    add_win->set_config(&cfg);
    connect(add_win, SIGNAL(new_class_id(int)), this, SLOT(edit_new_id(int)));


    curr_rect_item = nullptr;
    index_data = nullptr;
}

qolo_data::~qolo_data()
{
    delete ui;
    delete view;
}

void qolo_data::on_open_clicked()
{
    path = QFileDialog::getOpenFileName(this, "选择文件", "", "JSON file (*.json)");
    if (cfg.read(path) == false) {
        QMessageBox::warning(this, "提示", "文件打开失败", QMessageBox::Yes);
        return;
    }
    if (data.read(cfg.path) == false) {
        QMessageBox::warning(this, "提示", "数据读取失败", QMessageBox::Yes);
        return;
    }
    if (data.data.size()) {
        index_update(0);
        view_update();
        list_update_file();
    }
}

void qolo_data::on_save_clicked()
{
    if (cfg.save(path) == false) {
        QMessageBox::warning(this, "提示", "文件保存失败", QMessageBox::Yes);
        return;
    }
    if (data.save(cfg.path) == false) {
        QMessageBox::warning(this, "提示", "数据保存失败", QMessageBox::Yes);
        return;
    }
}


void qolo_data::on_update_clicked()
{

}

void qolo_data::on_up_clicked()
{
    index_update(index - 1);
    view_update();
    ui->file_list->setCurrentRow(index);
}


void qolo_data::on_down_clicked()
{
    index_update(index + 1);
    view_update();
    ui->file_list->setCurrentRow(index);
}


void qolo_data::on_all_label_clicked()
{

}

void qolo_data::view_update_img(void)
{
    QString path = cfg.path + "/images/" + index_data->path + "/" + index_data->name + "." + index_data->ext;
    QPixmap pixmap(path);
    img_size = pixmap.size();
    scene.clear();
    scene.addPixmap(pixmap);
    scene.setSceneRect(scene.itemsBoundingRect());
}

void qolo_data::view_update_box()
{
    ui->curr_label->clear();
    c_item.clear();

    foreach (const box_info &tb, index_data->box) {
        view_add_box(tb);
    }
    select_index = -1;
}

void qolo_data::view_add_box(box_info box)
{
    QString name = cfg.names[box.class_id];
    ui->curr_label->addItem(name);
    int w = box.box[2] * img_size.width();
    int h = box.box[3] * img_size.height();
    int x = box.box[0] * img_size.width();
    int y = box.box[1] * img_size.height() - h / 2;
    QRectF rect(x - w / 2, y, w, h);
    scene_item item = {nullptr, nullptr};
    QPen pen(Qt::red);
    pen.setWidth(2);
    item.rect = scene.addRect(rect, QPen(Qt::red));
    item.text = scene.addText(name);
    QFont tf;
    tf.setPixelSize(16);
    item.text->setFont(tf);
    QRectF br = item.text->boundingRect();
    QPointF txt_pos(x - br.width() / 2, y - br.height());
    item.text->setPos(txt_pos);
    item.text->setDefaultTextColor(Qt::red);
    c_item.append(item);
}

void qolo_data::list_update_file()
{
    ui->file_list->clear();
    foreach (const item_data *t, data.data) {
        ui->file_list->addItem(t->path + "/" + t->name);
    }
    ui->file_list->setCurrentRow(index);
}

void qolo_data::view_update()
{
    view_update_img();
    view_update_box();
}

void qolo_data::view_press_mouse(QPointF point)
{
    int x = point.x();
    int y = point.y();

    if (x < 0) {
        x = 0;
    } else if (x >= img_size.width()) {
        x = img_size.width() - 1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= img_size.height()) {
        y = img_size.height() - 1;
    }
    press_point.setX(x);
    press_point.setY(y);
}

void qolo_data::view_release_mouse(QPoint point)
{
    if (curr_rect_item) {
        add_win->show();
        setEnabled(false);
    }
}

void qolo_data::view_move_mouse(QPointF point)
{
    int x = point.x();
    int y = point.y();

    if (scene.width() == 0 && scene.height() == 0) return;
    if (x < 0) {
        x = 0;
    } else if (x >= img_size.width()) {
        x = img_size.width() - 1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= img_size.height()) {
        y = img_size.height() - 1;
    }
    int w = x - press_point.x();
    int h = y - press_point.y();
    if (w < 0) {
        w = abs(w);
    } else {
        x = press_point.x();
    }
    if (h < 0) {
        h = abs(h);
    } else {
        y = press_point.y();
    }
    curr_rect.setRect(x, y, w, h);
    if (curr_rect_item == nullptr) {
        QPen pen(Qt::red);
        pen.setWidth(2);
        curr_rect_item = scene.addRect(curr_rect, QPen(Qt::red));
    } else {
        curr_rect_item->setRect(curr_rect);
    }
}

void qolo_data::index_update(int i)
{
    index = i;
    if (index < 0) index = data.data.size() - 1;
    else if (index >= data.data.size()) index = 0;
    index_data = data.data.at(index);
}


void qolo_data::on_del_clicked()
{
    if (index_data == nullptr || select_index >= index_data->box.size()) {
        return;
    }
    if (select_index < 0) select_index = ui->curr_label->currentRow();
    if (select_index < 0) return;
    scene_item item = c_item.at(select_index);
    c_item.remove(select_index);
    index_data->box.remove(select_index);
    scene.removeItem(item.rect);
    scene.removeItem(item.text);
    delete ui->curr_label->takeItem(select_index);
    select_index = -1;
}

void qolo_data::edit_new_id(int id)
{
    setEnabled(true);
    if (curr_rect_item) {
        scene.removeItem(curr_rect_item);
        curr_rect_item = nullptr;
    }
    if (id < 0 || id >= cfg.names.size()) {
        return;
    }

    box_info box;
    double w = (double) curr_rect.width();
    double h = (double) curr_rect.height();
    double x = (double)curr_rect.x() + w / 2.0;
    double y = (double)curr_rect.y() + h / 2.0;
    box.class_id = (uint32_t)id,
    box.box[0] = x / img_size.width();
    box.box[1] = y / img_size.height();
    box.box[2] = w / img_size.width();
    box.box[3] = h / img_size.height();
    view_add_box(box);
    index_data->box.append(box);
}

void qolo_data::on_curr_label_itemClicked(QListWidgetItem *item)
{
    int temp_index = ui->curr_label->row(item);
    QPen pen;
    pen.setWidth(2);
    if (select_index != temp_index && select_index >= 0) {
        pen.setColor(Qt::red);
        scene_item s_item = c_item.at(select_index);
        s_item.rect->setPen(pen);
        s_item.text->setDefaultTextColor(Qt::red);
    }
    pen.setColor(Qt::green);
    scene_item s_item = c_item.at(temp_index);
    s_item.rect->setPen(pen);
    s_item.text->setDefaultTextColor(Qt::green);
    select_index = temp_index;
}


void qolo_data::on_file_list_itemClicked(QListWidgetItem *item)
{
    int curr_index = ui->file_list->row(item);
    if (index != curr_index) {
        index_update(curr_index);
        view_update();
    }
}

