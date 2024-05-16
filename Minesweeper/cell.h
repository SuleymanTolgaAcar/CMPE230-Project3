#ifndef CELL_H
#define CELL_H
#include <QPushButton>
#include <QMouseEvent>

class Cell : public QPushButton
{
    Q_OBJECT

public:
    Cell(int row, int col, const QIcon& icon, QWidget* parent = 0);
    QIcon icon;
    bool revealed;
    bool flagged;
    bool mined;
    int neighborMines;
    int row;
    int col;
public slots:
    void putFlag();
    void reveal();
    void mousePressEvent(QMouseEvent *event);
};

#endif // CELL_H
