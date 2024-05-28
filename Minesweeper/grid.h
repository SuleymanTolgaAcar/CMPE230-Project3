#ifndef GRID_H
#define GRID_H
#include <QGridLayout>
class Grid : public QGridLayout
{
    Q_OBJECT
public:
    Grid(int row, int col, int mineCount);
    void initialize();
    void revealCells(int row, int col);
    bool checkGameOver();
    int row;
    int col;
    int mineCount;
    int score;
public slots:
    void reset();
    void hint();
    void gameOver(bool won);
signals:
    void revealedSignal(QString);
};

#endif // GRID_H
