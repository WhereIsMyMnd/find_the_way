// вспомогательные средства
#pragma once

#include "selectablerectitem.h"

// структура, описывающая айтемы на сцене
typedef struct {
    SelectableRectItem* item;
    int state;
    int index;
} Squares;

// состояние айтемов на сцене
enum SquareState {
    NONACTIVE = -1, // некативный
    BLOCKED,        // заблокированный
    START,          // старт
    FINISH          // финиш
};

// тип вычисляемого пути
enum PathType {
    AB_PATH,    // путь от А до В
    MOUSE_PATH  // путь от А до курсора
};
