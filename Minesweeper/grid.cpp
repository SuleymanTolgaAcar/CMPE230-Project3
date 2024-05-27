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
    countNeighborMines();
}

void Grid::reset() {
    Grid(row, col, mineCount);
}

void Grid::countNeighborMines() {
    for (int r = 0; r < this->row; r++) {
        for (int c = 0; c < this->col; c++) {
            Cell *cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());

            // Check all neighboring cells
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int neighborRow = r + i;
                    int neighborCol = c + j;

                    // Skip the current cell
                    if (i == 0 && j == 0) {
                        continue;
                    }

                    // Check if the neighboring cell is within the grid boundaries
                    if (neighborRow >= 0 && neighborRow < this->row && neighborCol >= 0 && neighborCol < this->col) {
                        Cell *neighborCell = qobject_cast<Cell*>(this->itemAtPosition(neighborRow, neighborCol)->widget());
                        if (neighborCell->mined) {
                            cell->neighborMines++;
                        }
                    }
                }
            }
        }
    }
}

