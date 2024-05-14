#include "cell.h"

Cell::Cell(const QIcon& icon, QWidget* parent) : QPushButton(parent) {
    this->icon = icon;
    this->setIcon(icon);
}
