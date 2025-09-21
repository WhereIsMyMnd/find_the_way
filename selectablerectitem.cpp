//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, переопределяющий QGraphicsRectItem
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "selectablerectitem.h"
#include <QPainter>

SelectableRectItem::SelectableRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{
    emitter = new SelectedItemEmitter;
}

SelectableRectItem::~SelectableRectItem()
{
    delete emitter;
}

// переопределение itemChange для изменения состояния выбранного айтема
QVariant SelectableRectItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemSelectedChange && value.toBool())
        emit emitter->itemSelected();
    return QGraphicsRectItem::itemChange(change, value);
}

// переопределение paint для перекраски выбранного айтема
void SelectableRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("Arial");
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter, text);
}

// установка текста и вызов paint для обновление состояния айтема
void SelectableRectItem::setText(QString text)
{
    this->text = text;
    update();
}

// устновка флага возможности нажатия на айтем
void SelectableRectItem::setSelectable(bool state)
{
    setFlag(QGraphicsItem::ItemIsSelectable, state);
}

// возвращение объекта SelectedItemEmitter для отправки сигнала itemSelected
SelectedItemEmitter* SelectableRectItem::selectedEmitter()
{
    return emitter;
}
