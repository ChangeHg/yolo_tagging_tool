#include "customview.h"

#include <QMouseEvent>

CustomView::CustomView(QWidget *parent)
    : QGraphicsView(parent)
{
    mouse_state = false;
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    mouse_state = true;
    emit press_mouse(mapToScene(event->pos()));
    QGraphicsView::mousePressEvent(event);
}

void CustomView::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_state = false;
    emit release_mouse(event->pos());
    QGraphicsView::mouseReleaseEvent(event);
}

void CustomView::mouseMoveEvent(QMouseEvent *event)
{
    if (mouse_state) emit move_mouse(mapToScene(event->pos()));
    QGraphicsView::mouseMoveEvent(event);
}
