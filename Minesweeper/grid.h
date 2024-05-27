#ifndef GRID_H
#define GRID_H
#include <QGridLayout>
class Grid : public QGridLayout
{
    Q_OBJECT
public:
    Grid(int row, int col, int mineCount);
    int row;
    int col;
    int mineCount;
    int score;
private:
    void countNeighborMines();
public slots:
    void reset();
};

#endif // GRID_H
