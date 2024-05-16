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

    // Create horizontal layout for score and buttons
    QHBoxLayout *navbar = new QHBoxLayout(cw);
    QLabel *score = new QLabel("Score: 0");
    QPushButton *rst = new QPushButton("Restart");
    QPushButton *quit = new QPushButton("Quit");

    // Add space between score and buttons
    navbar->addWidget(score);
    navbar->addStretch();
    navbar->addWidget(rst);
    navbar->addWidget(quit);

    // Put horizontal layout to the vertical box layouts
    vb->addLayout(navbar);

    // Add vertical space between grid and horizontal layout
    vb->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    Grid *gl = new Grid();

    int N, M;
    //std::cin >> N >> M;

    QIcon icon(":/assets/empty.png");
    gl->setSpacing(0);

    // Create 10 x 10 Grid
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 20; col++) {
            Cell *randCell = new Cell(icon);
            gl->addWidget(randCell, row, col, 1, 1);
        }
    }

    vb->addLayout(gl);
    cw->setFixedSize(cw->sizeHint());
    cw->setWindowTitle("Minesweeper");
    cw->show();
    return app.exec();
}
