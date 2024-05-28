#include "cell.h"

#include "grid.h"

Cell::Cell(int row, int col, Grid* grid, QWidget* parent) : QPushButton(parent) {
    this->row = row;
    this->col = col;
    this->grid = grid;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->initialize();
}

// reset all values to  defaults
void Cell::initialize() {
    this->neighborMines = 0;
    this->mined = false;
    this->revealed = false;
    this->flagged = false;
    this->hinted = false;
    this->setIcon(QIcon(":/empty.png"));
    this->setFixedSize(QSize(15, 15));
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(revealSlot()));
}

// slot function to call the reveal cells function from grid
void Cell::revealSlot() {
    this->grid->revealCells(this->row, this->col);
}

// reveal the cell
void Cell::reveal() {
    this->revealed = true;
    this->hinted = false;

    // if the cell is mined, emit game over signal
    if (this->mined){
        this->setIcon(QIcon(":/mine.png"));
        emit gameOver(false);
    } else {
        // update the score and display appropriate icon for the cell
        this->grid->score++;
        this->setIcon(QIcon(QString(":/%1.png").arg(QString::number(this->neighborMines))));
        // disconnect signal to prevent clicking again
        QObject::disconnect(this, SIGNAL(clicked()), this, SLOT(revealSlot()));
    }
}

// right click functionality
void Cell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton && !this->revealed) {
        putFlag();
    } else {
        QPushButton::mousePressEvent(event);
    }
}

// mark the cell as flagged and display appropriate icon
void Cell::putFlag() {
    if (this->flagged) {
        this->flagged = false;
        this->setIcon(QIcon(":/empty.png"));
    }
    else {
        this->flagged = true;
        this->setIcon(QIcon(":/flag.png"));
    }
}
