#include "cell.h"

#include "grid.h"

Cell::Cell(int row, int col, Grid* grid, QWidget* parent) : QPushButton(parent) {
    this->row = row;
    this->col = col;
    this->grid = grid;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->initialize();
}

void Cell::initialize() {
    this->neighborMines = 0;
    this->mined = false;
    this->revealed = false;
    this->flagged = false;
    this->setIcon(QIcon(":/empty.png"));
    this->setFixedSize(QSize(15, 15));
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(revealSlot()));
}

void Cell::revealSlot() {
    this->grid->revealCells(this->row, this->col);
}

void Cell::reveal() {
    this->revealed = true;

    if (this->mined){
        this->setIcon(QIcon(":/mine.png"));
        emit gameOver(false);
    } else {
        this->grid->score++;
        this->setIcon(QIcon(QString(":/%1.png").arg(QString::number(this->neighborMines))));
        QObject::disconnect(this, SIGNAL(clicked()), this, SLOT(revealSlot()));
    }
}

void Cell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton && !this->revealed) {
        putFlag();
    } else {
        QPushButton::mousePressEvent(event);
    }
}

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
