#ifndef CELL_H
#define CELL_H
#include "grid.h"

#include <QPushButton>
#include <QMouseEvent>
#include <QMessageBox>

class Cell : public QPushButton
{
    Q_OBJECT

public:
    Cell(int row, int col, Grid* grid, QWidget* parent = 0);
    void initialize();
    void reveal();
    bool revealed;
    bool flagged;
    bool mined;
    Grid* grid;
    int neighborMines;
    int row;
    int col;
    bool hinted;
public slots:
    void putFlag();
    void revealSlot();
    void mousePressEvent(QMouseEvent *event);
signals:
    void gameOver(bool);
};

#endif // CELL_H
