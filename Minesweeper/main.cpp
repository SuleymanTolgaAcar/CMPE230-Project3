#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "grid.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QWidget *cw = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout(cw);

    // Create horizontal layout for score and buttons
    QHBoxLayout *navbar = new QHBoxLayout(cw);
    QLabel *score = new QLabel("Score: 0");
    QPushButton *rst = new QPushButton("Restart");
    QPushButton *hint = new QPushButton("Hint");
    navbar->addWidget(score);
    navbar->addStretch();
    navbar->addWidget(rst);
    navbar->addWidget(hint);

    // Put horizontal layout to the vertical box layouts
    vb->addLayout(navbar);

    // Add vertical space between grid and horizontal layout
    vb->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    int N = 20; int M = 20; int K = 20;

    Grid *gl = new Grid(N, M, K);

    QObject::connect(rst, SIGNAL(clicked()), gl, SLOT(reset()));    // Add space between score and buttons

    vb->addLayout(gl);
    cw->setFixedSize(cw->sizeHint());
    cw->setWindowTitle("Minesweeper");
    cw->show();
    return app.exec();
}
