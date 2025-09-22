//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, вычисляющий кратчайшее расстояние между заданными точками. Использует алгоритм поиска в ширину.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include "squares.h"

class FindTheWay : public QObject
{
    Q_OBJECT
public:
    FindTheWay();
    std::atomic_bool stop = false;

signals:
    // сигнал, содержащий найденный путь
    void foundPath(std::vector<int> path, PathType type);

public slots:
    // вычисление пути
    void findTheWay(const std::vector<std::vector<int>> neighbors, int start, int finish, PathType type);
};
