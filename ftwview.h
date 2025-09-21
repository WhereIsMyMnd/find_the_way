//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, переопределяющий QGraphicsView
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QGraphicsView>
#include <QObject>
#include <QWheelEvent>

class FtwView : public QGraphicsView
{
    Q_OBJECT
public:
    FtwView(QGraphicsScene* scene = nullptr);

protected:
    // переопределение wheelEvent для масштабирования поля
    void wheelEvent(QWheelEvent* event) override;

private:
    double curScale;
};
