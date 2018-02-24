#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class addWindow;
}

class addWindow : public QWidget
{
    Q_OBJECT

public:
    explicit addWindow(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    ~addWindow();

private:
    Ui::addWindow *ui;
};

#endif // ADDWINDOW_H
