#include "cell.h"

Cell::Cell(const QIcon& icon, QWidget* parent) : QPushButton(parent) {
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
    switch (this->neighborMines) {
    case 0:
        newIcon = new QIcon(":/assets/0.png");
        break;
    case 1:
        newIcon = new QIcon(":/assets/1.png");
        break;
    case 2:
        newIcon = new QIcon(":/assets/2.png");
        break;
    case 3:
        newIcon = new QIcon(":/assets/3.png");
        break;
    case 4:
        newIcon = new QIcon(":/assets/4.png");
        break;
    case 5:
        newIcon = new QIcon(":/assets/5.png");
        break;
    case 6:
        newIcon = new QIcon(":/assets/6.png");
        break;
    case 7:
        newIcon = new QIcon(":/assets/7.png");
        break;
    }
    this->icon = *newIcon;
    this->setIcon(this->icon);
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
