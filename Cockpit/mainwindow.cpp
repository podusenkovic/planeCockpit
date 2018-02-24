#define PI 3.1415
#include "mainwindow.h"
#include "ui_mainwindow.h"

double speed = 0; // kph
int dir = 0; //degrees
int horAccel = 0;
double altitude = 0; // km
double accel = 0; //

bool flaps = false;
QTimer *timerAlti;
QTimer *timerSpeed;
QTimer *timerDir;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("SAMALETIK"));
    anotherWin = new addWindow();
    anotherWin->show();
    anotherWin->setWindowTitle("COMPAS");
    QPoint po = this->pos();
    po.setX(po.x() + 375);
    anotherWin->move(po);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.setBrush(Qt::yellow);
    painter.drawEllipse(30,30, 150, 150); // center = (105,105)
    QPoint Center1 = QPoint(105,105);

    painter.setBrush(Qt::green);
    painter.drawEllipse(30,210, 150, 150); // center = (105,285)
    QPoint Center2 = QPoint(105,285);

    painter.setPen(QPen(Qt::black, 10));
    painter.drawPoint(Center1);
    painter.drawPoint(Center2);

    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int rad = 60;
    double x, y;
    int k = 0;
    double degOfVec1 = 3*PI/4 + (speed / 220) * 3 * PI / 2;
    double degOfVec2 = 3*PI/4 + (altitude / 10000) * 3 * PI / 2;

    painter.setPen(QPen(Qt::black, 3));
    double d = 3*PI/4;
    while(d <= 9*PI/4 + 0.01){
        x = rad * cos(d) + 100;
        y = rad * sin(d) + 110;
        painter.drawText(QPoint(x,y),QString::number((k/60)*22));
        k += 100;
        d = d + PI/4;
    }
    QPoint speedP(rad * cos(degOfVec1) + 100,\
                   rad * sin(degOfVec1) + 100);
    painter.drawLine(Center1, speedP);

    d = 3*PI/4; k = 0;
    while(d <= 9*PI/4 + 0.01){
        x = rad * cos(d) + 92;
        y = rad * sin(d) + 290;
        painter.drawText(QPoint(x,y),QString::number((k/6)*100));
        k += 100;
        d = d + PI/4;
    }
    QPoint altiP(rad * cos(degOfVec2) + 100,\
                  rad * sin(degOfVec2) + 280);
    painter.drawLine(Center2, altiP);


}


//300x400

void MainWindow::on_verticalSlider_actionTriggered(int action)
{
    double data = ui->verticalSlider->value() * 1.0;
    ui->lcdNumber->display(data);
    accel = data/3;
    this->update();
}

void MainWindow::growAlti(){
    if (altitude < 10000){
        altitude += 300;
        this->update();
    }
    else disconnect(timerAlti, SIGNAL(timeout()), this, SLOT(growAlti()));
}

void MainWindow::decreaseAlti(){
    if (altitude > 0){
        altitude -= 300;
        this->update();
    }
    else
        disconnect(timerAlti, SIGNAL(timeout()), this, SLOT(decreaseAlti()));
}

void MainWindow::on_pushButton_clicked()
{
    try {
        disconnect(timerAlti, SIGNAL(timeout()), this, SLOT(decreaseAlti()));
        disconnect(timerAlti, SIGNAL(timeout()), this, SLOT(growAlti()));
    }
    catch (std::exception& e){}
    if(speed > 200 && flaps == false)
    {
        flaps = true;
        timerAlti = new QTimer(this);
        connect(timerAlti, SIGNAL(timeout()), this, SLOT(growAlti()));
        timerAlti->start(1000);
    }
    if (speed < 50 && flaps == true)
    {
        flaps = false;
        timerAlti = new QTimer(this);
        connect(timerAlti, SIGNAL(timeout()), this, SLOT(decreaseAlti()));
        timerAlti->start(1000);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::changeSpeed(){
    //speed += accel;
    if (speed <= 220)
        speed += 3*accel;
    this->update();
}


void MainWindow::on_pushButton_3_clicked()
{
    disconnect(timerSpeed, SIGNAL(timeout()), this, SLOT(changeSpeed()));
    disconnect(timerDir, SIGNAL(timeout()), this, SLOT(changeDir()));

    timerSpeed = new QTimer(this);
    connect(timerSpeed, SIGNAL(timeout()), this, SLOT(changeSpeed()));
    timerSpeed->start(1000);

    timerDir = new QTimer(this);
    connect(timerDir, SIGNAL(timeout()), this, SLOT(changeDir()));
    timerDir->start(100);
}

void MainWindow::changeDir(){
    dir = ((dir + horAccel) % 360) * 1.0;
    ui->lcdNumber_2->display(dir);
}

void addWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    double y = 50;
    double x = 0;

    QFont font = painter.font();
    font.setPointSize(18);
    painter.setFont(font);

    for (int i = dir - 5; i <= dir + 6; i++)
    {
        x = 60*(i - dir) + 295;
        painter.drawText(QPoint(x,y),QString::number((i + 360)%360));
    }
    this->update();
}


void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    double data = ui->horizontalSlider->value() * 1.0;
    horAccel = data/3;
}
