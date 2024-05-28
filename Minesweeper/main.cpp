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

    int N = 20; int M = 20; int K = 50;

    Grid *gl = new Grid(N, M, K);

    QObject::connect(resetButton, SIGNAL(clicked()), gl, SLOT(reset()));    // Add space between score and buttons
    QObject::connect(hintButton, SIGNAL(clicked()), gl, SLOT(hint()));    // Add space between score and buttons
    QObject::connect(gl, SIGNAL(revealedSignal(QString)), scoreLabel, SLOT(setText(QString)));

    vb->addLayout(gl);
    cw->setFixedSize(cw->sizeHint());
    cw->setWindowTitle("Minesweeper");
    cw->show();
    return app.exec();
}
