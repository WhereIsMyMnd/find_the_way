//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, переопределяющий QGraphicsScene
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QGraphicsScene>
#include <QPointF>

class FtwScene : public QGraphicsScene
{
    Q_OBJECT
public:
    FtwScene(QObject *parent = nullptr);

signals:
    // сигнал, содержащий текущую позицию мыши
    void mouseMoved(QPointF mousePos);

protected:
    // переопределние mouseMoveEvent для отслеживания текущей позиции мыши и отрисовки пути от А до курсора
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};
