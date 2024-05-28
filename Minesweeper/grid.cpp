#include "grid.h"
#include "cell.h"

#include <QQueue>

Grid::Grid(const int row, const int col, const int mineCount) : QGridLayout() {
    this->row = row;
    this->col = col;
    this->mineCount = mineCount;

    this->initialize();
}

void Grid::initialize() {
    srand(time(0));
    this->setSpacing(0);
    this->score = 0;
    emit revealedSignal(QString("Score: 0"));

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            if (!this->itemAtPosition(r, c)) {
                Cell* cell = new Cell(r, c, this);
                this->addWidget(cell, r, c, 1, 1);
                QObject::connect(cell, SIGNAL(gameOver(bool)), this, SLOT(gameOver(bool)));
            } else {
                Cell* cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());
                cell->initialize();
            }
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

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int neighborRow = cell->row + i;
                int neighborCol = cell->col + j;

                // Skip the current cell
                if (i == 0 && j == 0) {
                    continue;
                }

                if (neighborRow >= 0 && neighborRow < this->row && neighborCol >= 0 && neighborCol < this->col){
                    Cell *neighborCell = qobject_cast<Cell*>(this->itemAtPosition(neighborRow, neighborCol)->widget());
                    neighborCell->neighborMines++;
                }
            }
        }
    }
}

void Grid::reset() {
    this->initialize();
}

void Grid::revealCells(int row, int col){
    Cell* startCell = qobject_cast<Cell*>(this->itemAtPosition(row, col)->widget());

    QQueue<Cell*> queue;
    queue.enqueue(startCell);

    QSet<Cell*> revealed;

    while (!queue.isEmpty()) {
        Cell* cell = queue.dequeue();
        if (revealed.contains(cell)) {
            continue;
        }

        cell->reveal();
        emit revealedSignal(QString("Score: %1").arg(QString::number(this->score)));
        if (this->checkGameOver()){
            emit gameOver(true);
        }
        revealed.insert(cell);

        if (cell->neighborMines > 0 || cell->mined) {
            continue;
        }

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }

                int neighborRow = cell->row + i;
                int neighborCol = cell->col + j;

                if (neighborRow >= 0 && neighborRow < this->row && neighborCol >= 0 && neighborCol < this->col) {
                    Cell* neighborCell = qobject_cast<Cell*>(this->itemAtPosition(neighborRow, neighborCol)->widget());
                    if (neighborCell && !revealed.contains(neighborCell) && !neighborCell->revealed) {
                        queue.enqueue(neighborCell);
                    }
                }
            }
        }
    }
}

bool Grid::checkGameOver(){
    bool won = true;
    for (int r = 0; r < this->row; r++) {
        for (int c = 0; c < this->col; c++) {
            Cell *cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());
            if (!cell->mined && !cell->revealed){
                won = false;
            }
        }
    }
    return won;
}

void Grid::hint() {
    for (int r = 0; r < this->row; r++) {
        for (int c = 0; c < this->col; c++) {
            Cell *cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());
            if (cell->mined || cell->revealed) {
                continue;
            }
            else {
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
                            if (neighborCell->revealed) {
                                cell->setIcon(QIcon(":/hint.png"));
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Grid::gameOver(bool won) {
    for (int r = 0; r < this->row; r++){
        for (int c = 0; c < this->col; c++){
            Cell* cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());
            QObject::disconnect(cell, SIGNAL(clicked()), cell, SLOT(revealSlot()));

            if (cell->mined){
                cell->setIcon(QIcon(":/mine.png"));
            }
        }
    }

    if (won) {
        QMessageBox msgBox;
        msgBox.setText("You win!");
        msgBox.exec();
        msgBox.setStandardButtons(QMessageBox::Ok);
    } else {
        QMessageBox msgBox;
        msgBox.setText("You lose!");
        msgBox.exec();
        msgBox.setStandardButtons(QMessageBox::Ok);
    }
}
