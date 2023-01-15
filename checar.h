#ifndef CHECAR_H
#define CHECAR_H

#include <QWidget>

namespace Ui {
class Checar;
}

class Checar : public QWidget
{
    Q_OBJECT

public:
    explicit Checar(QWidget *parent = nullptr);
    ~Checar();

private:
    Ui::Checar *ui;
};

#endif // CHECAR_H
