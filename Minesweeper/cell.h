#ifndef CELL_H
#define CELL_H
#include <QPushButton>
#include <QIcon>

class Cell : public QPushButton
{
    Q_OBJECT

public:
    Cell(const QIcon& icon, QWidget* parent = 0);
    QIcon icon;
    //bool revealed;
    //bool flagged;
public slots:
    //void put_flag();
    //void reveal();
    //void change_icon();
};

#endif // CELL_H
