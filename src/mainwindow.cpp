#include "mainwindow.h"0
#include "ui_mainwindow.h"
#include "timerThread.h"
#include "QSlider"
#include "QPixmap"
#include "QFile"
#include "QDir"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    procThread = new timerThread(this);
    connect(procThread, SIGNAL(updateGUI(joystick_event*)),this, SLOT(onUpdateGUI(joystick_event*)));
    runningFlag = 0;
    planeState = new Plane;
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
    ui->xAxisValue->setText(QString::number(event->stick_x, 'f', 2 ));
    ui->yAxisValue->setText(QString::number(event->stick_y, 'f', 2 ));
    ui->zAxisValue->setText(QString::number(event->stick_z, 'f', 2 ));
    ui->throttleValue->setText(QString::number(event->throttle, 'f', 2 ));
    ui->afterburner_box->setChecked(event->button[1]);


}
void MainWindow::updateSliders(joystick_event* event)
{
    ui->xSlider->setSliderPosition(((event->stick_x + 1)/2) *100);
    ui->ySlider->setSliderPosition(((event->stick_y + 1)/2) *100);
    ui->zSlider->setSliderPosition(((event->stick_z + 1)/2) *100);
    ui->throttleSlider->setSliderPosition(((event->throttle + 1)/2) *100);
}

void MainWindow::on_pushButton_clicked()
{

    if(!runningFlag)
    {
        procThread->Stop = false;
        runningFlag = 1;
        ui->status->setText("READING");
        procThread->start();
    }
    else
    {
        runningFlag = 0;
        procThread->Stop = true;
        ui->status->setText("STOPPED");
    }
}

void MainWindow::onEventLoopStarted()
{
    parseXML();
    planeState->update_model_parameters();
}

void parseXML()
{
    QString exe = QCoreApplication::applicationDirPath();
    QString fileName = exe + "/../../flight_simulator/src/input/planeModels.xml";
    printf(fileName.toLatin1());
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        printf("ERROR\n");
    QString content = file.readAll();
    printf(content.toLatin1());
}
