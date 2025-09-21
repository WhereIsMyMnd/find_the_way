//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, переопределяющий QGraphicsRectItem
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QGraphicsRectItem>

// вспомогательный класс для отправки сигнала выбранного айтема на поле
class SelectedItemEmitter : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

signals:
    void itemSelected();
};

class SelectableRectItem : public QGraphicsRectItem
{
public:
    SelectableRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    ~SelectableRectItem();
    // устновка флага возможности нажатия на айтем
    void setSelectable(bool state);
    // возвращение объекта SelectedItemEmitter для отправки сигнала itemSelected
    SelectedItemEmitter* selectedEmitter();
    // установка текста и вызов paint для обновление состояния айтема
    void setText(QString text);

protected:
    // переопределение itemChange для изменения состояния выбранного айтема
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    // переопределение paint для перекраски выбранного айтема
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    SelectedItemEmitter* emitter;
    QString text;
};
