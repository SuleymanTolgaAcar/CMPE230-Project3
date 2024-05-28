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
    // set seed for rand
    srand(time(0));
    this->setSpacing(0);
    this->score = 0;
    this->hintGiven = false;
    emit revealedSignal(QString("Score: 0"));

    // if this is the first time, call the cell constructor and add them to grid
    // if this is called to reset the grid, call the initialize method on all cells to reset them to default
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

    // randomly place mines
    for (int m = 0; m < mineCount; m++){
        Cell *cell;
        // if the current random cell is already mined, skip it and find a new cell to mine
        do {
            int randRow = rand() % this->row;
            int randCol = rand() % this->col;
            cell = qobject_cast<Cell*>(this->itemAtPosition(randRow, randCol)->widget());
        } while (cell->mined);

        cell->mined = true;

        // loop over neighbors and increment their neighbor mines count
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int neighborRow = cell->row + i;
                int neighborCol = cell->col + j;

                // Skip the current cell
                if (i == 0 && j == 0) continue;

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

// reveal the cell on given coordinates
// if the cell has zero neighbor cells, reveal its neighbors too using bfs
void Grid::revealCells(int row, int col){
    Cell* startCell = qobject_cast<Cell*>(this->itemAtPosition(row, col)->widget());

    // initialize a queue for bfs and add the starting cell
    QQueue<Cell*> queue;
    queue.enqueue(startCell);

    // keep a revealed hash set to avoid reprocessing previous cells
    QSet<Cell*> revealed;

    while (!queue.isEmpty()) {
        Cell* cell = queue.dequeue();
        if (revealed.contains(cell)) continue;

        cell->reveal();
        // update the score when revealing cell
        emit revealedSignal(QString("Score: %1").arg(QString::number(this->score)));

        // check if the player won
        if (this->checkGameOver()){
            emit gameOver(true);
        }

        revealed.insert(cell);

        // skip the cells with neighbor cells greater than zero, to stop the bfs at numbered cells
        if (cell->neighborMines > 0 || cell->mined) continue;

        // add neighbor cells to the queue
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;

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

// check whether there is an unrevealed cell or not
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
    // if a hint is already given, find the hinted cell and reveal it
    if (this->hintGiven){
        for (int r = 0; r < this->row; ++r) {
            for (int c = 0; c < this->col; ++c) {
                Cell* cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());
                if (cell->hinted){
                    this->revealCells(cell->row, cell->col);
                    this->hintGiven = false;
                    return;
                }
            }
        }
    }

    // these hashsets kind of "flags" cells as mined
    QSet<Cell*> oldMined;
    QSet<Cell*> mined;

    // continue searching for safe cells that are deducable by the player until there are no changes in the mined hashset
    while (true) {
        for (int r = 0; r < this->row; ++r) {
            for (int c = 0; c < this->col; ++c) {
                Cell* cell = qobject_cast<Cell*>(this->itemAtPosition(r, c)->widget());
                // loop over revealed cells to find a safe cell that can be deduced by the player as well
                if (cell && cell->revealed) {
                    // count unrevealed cells in neighbors
                    int unrevealedCount = 0;
                    // count neighbor cells which are flagged as mines (in the hashset)
                    int mineCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (i == 0 && j == 0) continue;
                            int neighborRow = r + i;
                            int neighborCol = c + j;
                            if (neighborRow >= 0 && neighborRow < this->row && neighborCol >= 0 && neighborCol < this->col) {
                                Cell* neighborCell = qobject_cast<Cell*>(this->itemAtPosition(neighborRow, neighborCol)->widget());
                                if (!neighborCell->revealed) {
                                    unrevealedCount++;
                                }
                                if (mined.contains(neighborCell)){
                                    mineCount++;
                                }
                            }
                        }
                    }
                    // if the value on the current cell is equal to the unrevealed cell count, mark all unrevealed cells as mines (add to hashset)
                    if (cell->neighborMines == unrevealedCount){
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (i == 0 && j == 0) continue;
                                int neighborRow = r + i;
                                int neighborCol = c + j;
                                if (neighborRow >= 0 && neighborRow < this->row && neighborCol >= 0 && neighborCol < this->col) {
                                    Cell* neighborCell = qobject_cast<Cell*>(this->itemAtPosition(neighborRow, neighborCol)->widget());
                                    if (!neighborCell->revealed){
                                        mined.insert(neighborCell);
                                    }
                                }
                            }
                        }
                    }
                    // if the value on the current cell is equal to the mine count, all of the other neighbor cells are safe, so hint the first one
                    if (cell->neighborMines == mineCount){
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (i == 0 && j == 0) continue;
                                int neighborRow = r + i;
                                int neighborCol = c + j;
                                if (neighborRow >= 0 && neighborRow < this->row && neighborCol >= 0 && neighborCol < this->col) {
                                    Cell* neighborCell = qobject_cast<Cell*>(this->itemAtPosition(neighborRow, neighborCol)->widget());
                                    if (!neighborCell->revealed && !neighborCell->mined && !mined.contains(neighborCell)){
                                        this->hintGiven = true;
                                        neighborCell->hinted = true;
                                        neighborCell->setIcon(QIcon(":/hint.png"));
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // check if there is a change in the flagged cells (if the old set contains all of the cells in the new set)
        // if there are no changes, break the loop (no viable hints found)
        bool equal = true;
        for (Cell* minedCell: mined){
            if (!oldMined.contains(minedCell)){
                equal = false;
                oldMined.insert(minedCell);
            }
        }
        if (equal) break;
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
