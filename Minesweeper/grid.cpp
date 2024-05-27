#include "grid.h"
#include "cell.h"

Grid::Grid(const int row, const int col, const int mineCount) : QGridLayout() {
    this->row = row;
    this->col = col;
    this->mineCount = mineCount;

    QIcon icon(":/assets/empty.png");
    this->setSpacing(0);

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            Cell *cell = new Cell(row, col, icon);
            this->addWidget(cell, r, c, 1, 1);
        }
    }

    for (int m = 0; m < mineCount; m++){
        Cell *cell;
        do {
            int randRow = rand() % this->row;
            int randCol = rand() % this->col;
            cell = qobject_cast<Cell*>(this->itemAtPosition(randRow, randCol)->widget());
        } while (cell->mined);
        cell->mined = true;
    }
}
