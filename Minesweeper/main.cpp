#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "grid.h"
#include "cell.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QWidget *cw = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout(cw);
    Grid *gl = new Grid();

    int N, M;
    //std::cin >> N >> M;

    QIcon icon("assets/0.png");
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {

            Cell *randCell = new Cell(icon);
            gl->addWidget(randCell, row,col, 1, 1);
        }
    }
    vb->addLayout(gl);

    cw->setWindowTitle("Minesweeper");
    cw->resize(640, 480);
    cw->show();

    return app.exec();
}
