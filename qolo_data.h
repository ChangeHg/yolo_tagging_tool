#ifndef QOLO_DATA_H
#define QOLO_DATA_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QWidget>
#include "config.h"
#include "qolodata.h"
#include <customview.h>
#include <edit_label.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class qolo_data;
}
QT_END_NAMESPACE

struct scene_item {
    QGraphicsRectItem *rect;
    QGraphicsTextItem *text;
};

class qolo_data : public QWidget
{
    Q_OBJECT

public:
    qolo_data(QWidget *parent = nullptr);
    ~qolo_data();

private slots:
    void on_open_clicked();

    void on_save_clicked();

    void on_update_clicked();

    void on_up_clicked();

    void on_down_clicked();

    void on_all_label_clicked();

    void view_press_mouse(QPointF point);

    void view_release_mouse(QPoint point);

    void view_move_mouse(QPointF point);

    void on_del_clicked();

    void edit_new_id(int id);

    void on_curr_label_itemClicked(QListWidgetItem *item);

    void on_file_list_itemClicked(QListWidgetItem *item);

private:
    Ui::qolo_data *ui;
    CustomView *view;
    QString path;
    Config cfg;
    QoloData data;
    QGraphicsScene scene;
    QVector<scene_item> c_item;
    QSize img_size;
    int index;
    item_data *index_data;
    QPoint press_point;
    QGraphicsRectItem *curr_rect_item;
    EditLabel *add_win;
    QRectF curr_rect;

    int select_index;
    void index_update(int i);
    void view_update_img(void);
    void view_update_box(void);
    void view_add_box(box_info box);
    void list_update_file(void);
    void view_update(void);
};
#endif // QOLO_DATA_H
