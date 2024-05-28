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

    // create horizontal layout for score and buttons
    QHBoxLayout *navbar = new QHBoxLayout(cw);
    QLabel *scoreLabel = new QLabel(QString("Score: 0"));
    QPushButton *resetButton = new QPushButton("Restart");
    QPushButton *hintButton = new QPushButton("Hint");

    navbar->addWidget(scoreLabel);
    navbar->addStretch();
    navbar->addWidget(resetButton);
    navbar->addWidget(hintButton);

    // Put horizontal layout to the vertical box layouts
    vb->addLayout(navbar);

    // Add vertical space between grid and horizontal layout
    vb->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    // N = row count, M = column count, K = mine count
    int N = 15; int M = 15; int K = 20;

    // initialize the grid
    Grid *gl = new Grid(N, M, K);

    // connect signals for buttons
    QObject::connect(resetButton, SIGNAL(clicked()), gl, SLOT(reset()));
    QObject::connect(hintButton, SIGNAL(clicked()), gl, SLOT(hint()));
    // connect signal for updating score when a cell is revealed
    QObject::connect(gl, SIGNAL(revealedSignal(QString)), scoreLabel, SLOT(setText(QString)));

    // fix the size and display the app
    vb->addLayout(gl);
    cw->setFixedSize(cw->sizeHint());
    cw->setWindowTitle("Minesweeper");
    cw->show();
    return app.exec();
}
