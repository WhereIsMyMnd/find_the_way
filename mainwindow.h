#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QThread>
#include <QSettings>
#include "squares.h"
#include "findtheway.h"
#include "ftwscene.h"
#include "ftwview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void findTheWay(const std::vector<std::vector<int>> squareNeighbors, int start, int finish, PathType type);

private:
    Ui::MainWindow *ui;
    int rows = 7, cols = 7, squareSize = 25;
    FtwScene* scene;
    FtwView* view;
    QSpinBox *widthSpBox, *heightSpBox;
    std::vector<Squares> squares;
    std::vector<std::vector<int>> neighbors;
    QPushButton *pointABtn, *pointBBtn;
    FindTheWay* ftw;
    int start = -1, finish = -1;
    int mousePosIdx;
    std::vector<QGraphicsItem*> mainLine, mouseLine;
    QThread* ftwThread;
    QSettings settings{ "Company", "App" };

private slots:
    // генерация поля
    void generateScene();
    // действия, происходящие при установке точки А
    void setPointA(bool state);
    // действия, происходящие при установке точки В
    void setPointB(bool state);
    // вычисление пути для установленных точек
    void setPoints(int idx);
    // вычисление центра квадрата для отрисовки пути
    QPointF getSquareCenter(int idx);
    // вычисление пути от точки А до курсора мыши
    void setMousePath(QPointF pos);
    // отрисовка выбранного пути
    void paintLine(std::vector<int> path, PathType type);
    // сохранение настроек
    void saveSettings();
    // восстановление настроек
    void restoreSettings();
};
