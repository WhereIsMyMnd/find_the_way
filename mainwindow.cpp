#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ftwscene.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new FtwScene;
    connect(scene, &FtwScene::mouseMoved, this, &MainWindow::setMousePath);
    view = new FtwView(scene);
    view->setMouseTracking(true);

    ftwThread = new QThread;
    ftw = new FindTheWay;
    ftw->moveToThread(ftwThread);
    connect(ftwThread, &QThread::finished, ftw, &FindTheWay::deleteLater);
    connect(this, &MainWindow::findTheWay, ftw, &FindTheWay::findTheWay);
    connect(ftw, &FindTheWay::foundPath, this, &MainWindow::paintLine);
    ftwThread->start();

    widthSpBox = new QSpinBox;
    widthSpBox->setRange(4, 30);
    widthSpBox->setValue(cols);
    widthSpBox->setMinimumWidth(45);
    heightSpBox = new QSpinBox;
    heightSpBox->setRange(4, 30);
    heightSpBox->setValue(rows);
    heightSpBox->setMinimumWidth(45);
    QHBoxLayout* sceneSizeLayout = new QHBoxLayout;
    sceneSizeLayout->addStretch(2);
    sceneSizeLayout->addWidget(new QLabel("Ширина:"));
    sceneSizeLayout->addWidget(widthSpBox);
    sceneSizeLayout->addStretch(1);
    sceneSizeLayout->addWidget(new QLabel("Высота:"));
    sceneSizeLayout->addWidget(heightSpBox);
    sceneSizeLayout->addStretch(2);

    QPushButton* generateBtn = new QPushButton("Генерировать");
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generateScene);

    pointABtn = new QPushButton("A");
    pointABtn->setFixedWidth(30);
    pointABtn->setCheckable(true);
    connect(pointABtn, &QPushButton::toggled, this, &MainWindow::setPointA);
    pointBBtn = new QPushButton("B");
    pointBBtn->setFixedWidth(30);
    pointBBtn->setCheckable(true);
    connect(pointBBtn, &QPushButton::toggled, this, &MainWindow::setPointB);

    QVBoxLayout* ctrlLayout = new QVBoxLayout;
    ctrlLayout->addWidget(generateBtn);
    ctrlLayout->addStretch(2);
    ctrlLayout->addWidget(new QLabel("Точки:"), 0, Qt::AlignCenter);
    ctrlLayout->addWidget(pointABtn, 0, Qt::AlignCenter);
    ctrlLayout->addWidget(pointBBtn, 0, Qt::AlignCenter);
    ctrlLayout->addStretch(1);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(view, 0, 0);
    mainLayout->addLayout(ctrlLayout, 0, 1);
    mainLayout->addLayout(sceneSizeLayout, 1, 0);
    ui->centralwidget->setLayout(mainLayout);

    generateScene();
    restoreSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    ftwThread->quit();
    ftwThread->wait();
    delete ui;
}

// сохранение настроек
void MainWindow::saveSettings()
{
    settings.beginGroup("settings");
    settings.setValue("pos", pos());
    settings.endGroup();
}


// восстановление настроек
void MainWindow::restoreSettings()
{
    settings.beginGroup("settings");
    move(settings.value("pos", QPoint(300, 100)).toPoint());
    settings.endGroup();
}

// генерация поля
void MainWindow::generateScene()
{
    scene->clear();                       // сброс сцены и сопутствующих параметров
    squares.clear();
    mainLine.clear();
    mouseLine.clear();
    cols = widthSpBox->value();
    rows = heightSpBox->value();
    squares.resize(rows * cols);
    start = -1;
    finish = -1;
    ftw->stop = true;                     // флаг остановки вычисления пути, если путь не был вычислен до перегенерации поля

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            SelectableRectItem* square = new SelectableRectItem(j * squareSize, i * squareSize, squareSize, squareSize);
            square->setBrush(QBrush(Qt::white));
            square->setPen(QPen(Qt::black));
            scene->addItem(square);
            connect(square->selectedEmitter(), &SelectedItemEmitter::itemSelected, [this, idx]() { setPoints(idx); });

            squares[idx].item = square;                                   // добавление точек (вершин) в массив
            squares[idx].index = idx;                                     // добавление индекса каждой точке
            squares[idx].state = NONACTIVE;

            if (QRandomGenerator::global()->bounded(0, 100) < 15) {       // установка состояния точки (заблокирована или нет, шанс блокировки = 15%)
                square->setBrush(QBrush(Qt::black));
                squares[idx].state = BLOCKED;
            }
        }
    }

    for (auto &square : squares) {                    // установка соседей для точки
        if (square.state != BLOCKED) {
            bool conditions[] = {                     // условия добавления соседей
                (square.index % cols > 0),
                (square.index % cols < cols - 1),
                (square.index / cols > 0),
                (square.index / cols < rows - 1)
            };
            int offset[] = { -1, 1, -cols, cols };    // расположение соседей
            for (int i = 0; i < 4; i++) {
                if (conditions[i] && squares[square.index + offset[i]].state != BLOCKED)
                    square.neighbors.push_back(square.index + offset[i]);
            }
        }
    }

    view->setSceneRect(0, 0, squareSize * cols, squareSize * rows);
    view->setMinimumSize(squareSize * cols + 10, squareSize * rows + 10);
    view->resetTransform();
    ui->centralwidget->adjustSize();
    adjustSize();
}

// действия, происходящие при установке точки А
void MainWindow::setPointA(bool state)
{
    pointBBtn->setChecked(false);
    for (auto &square : squares) if (square.state != BLOCKED) square.item->setSelectable(state);
}

// действия, происходящие при установке точки В
void MainWindow::setPointB(bool state)
{
    pointABtn->setChecked(false);
    for (auto &square : squares) if (square.state != BLOCKED) square.item->setSelectable(state);
}

// вычисление пути для установленных точек
void MainWindow::setPoints(int idx)
{
    if (pointABtn->isChecked()) {                 // установка точки А
        for (auto &square : squares) {
            if (square.state == START) {
                square.state = NONACTIVE;
                square.item->setText("");
                for (auto &line : mainLine) {
                    scene->removeItem(line);
                    delete line;
                }
                mainLine.clear();
                break;
            }
        }
        squares[idx].item->setText("A");
        squares[idx].state = START;
        start = idx;
        pointABtn->setChecked(false);
    }

    if (pointBBtn->isChecked()) {                 // установка точки В
        for (auto &square : squares) {
            if (square.state == FINISH) {
                square.state = NONACTIVE;
                square.item->setText("");
                for (auto &line : mainLine) {
                    scene->removeItem(line);
                    delete line;
                }
                mainLine.clear();
                break;
            }
        }
        squares[idx].item->setText("B");
        squares[idx].state = FINISH;
        finish = idx;
        pointBBtn->setChecked(false);
    }

    if (start != -1 && finish != -1) {
        emit findTheWay(squares, start, finish, AB_PATH);
    }
}

// вычисление пути от точки А до курсора мыши
void MainWindow::setMousePath(QPointF pos)
{
    if (pos.x() >= cols * squareSize || pos.y() >= rows *squareSize) return;

    if (pos.x() == -1 && pos.y() == -1) {
        for (auto &line : mouseLine) {
            scene->removeItem(line);
            delete line;
        }
        mouseLine.clear();
        return;
    }

    int posX = static_cast<int>(pos.x()) / squareSize;
    int posY = static_cast<int>(pos.y()) / squareSize;
    int idx = posY * cols + posX;
    if (mousePosIdx == idx) return;
    mousePosIdx = idx;

    if (start != -1) {
        for (auto &line : mouseLine) {
            scene->removeItem(line);
            delete line;
        }
        mouseLine.clear();

        emit findTheWay(squares, start, mousePosIdx, MOUSE_PATH);
    }
}

// отрисовка выбранного пути
void MainWindow::paintLine(std::vector<int> path, PathType type)
{
    if (path.empty() && type == AB_PATH) {                             // окно с сообщение, в случае невозможности найти путь (А->В)
        QMessageBox message;
        message.setIcon(QMessageBox::Warning);
        message.setText("Невозможно дойти из А до В");
        message.exec();
    }

    for (int i = 1; i < path.size(); i++) {                            // отрисовка пути
        QPointF p1 = getSquareCenter(path[i - 1]);
        QPointF p2 = getSquareCenter(path[i]);

        QPointF dir = p2 - p1;
        qreal len = std::hypot(dir.x(), dir.y());
        QPointF unit = dir / len;
        qreal offset = squareSize / 2.0;
        if (i == 1) p1 = p1 + unit * offset;
        if (i == path.size() - 1) p2 = p2 - unit * offset;

        if (type == AB_PATH) {
            QGraphicsItem* lineItem = scene->addLine(QLineF(p1, p2), QPen(Qt::red, 2));
            mainLine.push_back(lineItem);
        }
        else if (type == MOUSE_PATH) {
            QGraphicsItem* lineItem = scene->addLine(QLineF(p1, p2), QPen(Qt::green, 2));
            mouseLine.push_back(lineItem);
        }
    }
}

// вычисление центра квадрата для отрисовки пути
QPointF MainWindow::getSquareCenter(int idx)
{
    int row = idx / cols;
    int col = idx % cols;
    return QPointF(col * squareSize + squareSize / 2.0, row * squareSize + squareSize / 2.0);
}
