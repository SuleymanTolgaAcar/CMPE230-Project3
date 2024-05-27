#include "cell.h"

Cell::Cell(int row, int col, const QIcon& icon, QWidget* parent) : QPushButton(parent) {
    this->row = row;
    this->col = col;
    this->icon = icon;
    this->setIcon(icon);
    this->neighborMines = 0; // should be changed
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(icon.actualSize(QSize(15, 15)));
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(reveal()));
}

void Cell::reveal() {
    this->revealed = true;
    QIcon *newIcon;
    // determine the image with respect to neighboring mines.
    if (this->mined){
        newIcon = new QIcon(":/assets/mine.png");

    }
    else {
        newIcon = new QIcon(QString(":assets/%1.png").arg(QString::number(this->neighborMines)));
    }
    this->setIcon(*newIcon);
    QObject::disconnect(this, SIGNAL(clicked()), this, SLOT(reveal()));
}

void Cell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton && !this->revealed) {
        putFlag();
    } else {
        QPushButton::mousePressEvent(event);
    }
}

void Cell::putFlag() {
    QIcon *newIcon;
    if (this->flagged) {
        this->flagged = false;
        newIcon = new QIcon(":/assets/empty.png");
    }
    else {
        this->flagged = true;
        newIcon = new QIcon(":/assets/flag.png");
    }
    this->icon = *newIcon;
    this->setIcon(this->icon);
}
