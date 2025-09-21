//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, переопределяющий QGraphicsScene
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ftwscene.h"
#include <QGraphicsSceneMouseEvent>

FtwScene::FtwScene(QObject *parent)
    : QGraphicsScene{parent}
{}

// переопределние mouseMoveEvent для отслеживания текущей позиции мыши и отрисовки пути от А до курсора
void FtwScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePos = event->scenePos();
    if (sceneRect().contains(mousePos)) emit mouseMoved(mousePos);
    else emit mouseMoved(QPointF(-1, -1));
    QGraphicsScene::mouseMoveEvent(event);
}
