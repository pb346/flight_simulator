#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timerThread.h"
#include "QSlider"
#include "QPixmap"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    procThread = new timerThread(this);
    connect(procThread, SIGNAL(updateGUI(joystick_event*)),this, SLOT(onUpdateGUI(joystick_event*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUpdateGUI(joystick_event* event)
{
    updateValues(event);
    updateSliders(event);
   // QCoreApplication::processEvents();
}

void MainWindow::updateValues(joystick_event* event)
{
    ui->throttleValue->setText(QString::number(event->throttle, 'f', 2 ));

}
void MainWindow::updateSliders(joystick_event* event)
{
    ui->throttleSlider->setSliderPosition(((event->throttle + 1)/2) *100);
}

void MainWindow::on_pushButton_clicked()
{
    procThread->Stop = false;
    ui->status->setText("READING");
    procThread->start();
}

void MainWindow::onEventLoopStarted()
{

}
