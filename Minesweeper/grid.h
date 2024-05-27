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
    int calculateScore();
private:
    void countNeighborMines();
public slots:
    void reset();
    void hint();
};

#endif // GRID_H
