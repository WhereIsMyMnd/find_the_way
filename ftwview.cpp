//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, переопределяющий QGraphicsView
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ftwview.h"

FtwView::FtwView(QGraphicsScene* scene)
    : QGraphicsView(scene)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

// переопределение wheelEvent для масштабирования поля
void FtwView::wheelEvent(QWheelEvent* event)
{
    const double scaleFactor = 1.1;
    if (event->angleDelta().y() > 0) scale (scaleFactor, scaleFactor);
    else scale (1.0 / scaleFactor, 1.0 / scaleFactor);
    curScale = transform().m11();
}
