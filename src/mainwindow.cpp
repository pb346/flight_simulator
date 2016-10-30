#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timerThread.h"
#include "plane.h"
#include <QSlider>
#include <QPixmap>
#include <QFile>
#include <QDir>
#include <QtXml>
#include <QXmlStreamReader>
#include <iostream>
#include <stdio.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    procThread = new timerThread(this);
    connect(procThread, SIGNAL(updateGUI(joystick_event*)),this, SLOT(onUpdateGUI(joystick_event*)));
    runningFlag = 0;
    planeState = new Plane();
    debug = new DebugValues();
    previousDebug = new DebugValues();
    previousDebug->gears = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUpdateGUI(joystick_event* event)
{
    updateValues(event);
    updateSliders(event);
    planeState->process_joystick_input(currentModel,event, &debug);

    ui->leftAilVal->setText(QString::number(debug->aileronLeft, 'f', 2));
    ui->rightAilVal->setText(QString::number(debug->aileronRight, 'f', 2));
    ui->throttleValue->setText(QString::number(debug->thrust, 'f', 2));
    ui->rudVal->setText(QString::number(debug->rudder, 'f', 2));
    ui->ElevVal->setText(QString::number(debug->elevator, 'f', 2));
    ui->checkGear->setChecked(planeState->gears_Deployed);

    if(previousDebug->gears == -1)
    {
        printf("First previous");
        debug->copyDebug(previousDebug);
/*        previousDebug->aileronLeft = debug->aileronLeft;
        previousDebug->aileronRight = debug->aileronRight;
        previousDebug->elevator = debug->elevator;
       // previousDebug->flap = this->flap;
        previousDebug->rudder = debug->rudder;
      //  previousDebug->slat = this->slat;
        previousDebug->thrust = debug->thrust;
        previousDebug->gears = 0;
        //debug->copyDebug(previousDebug);*/
        return;
    }
    if(previousDebug->gears == 1 && debug->gears == 0)
    {
        if(ui->checkGear->isChecked() == 1)
        {
            ui->checkGear->setChecked(0);
            planeState->gears_Deployed = 0;
        }
        else
        {
            ui->checkGear->setChecked(1);
            planeState->gears_Deployed = 1;
        }
    }
            debug->copyDebug(previousDebug);
            /*
    previousDebug->aileronLeft = debug->aileronLeft;
    previousDebug->aileronRight = debug->aileronRight;
    previousDebug->elevator = debug->elevator;
   // previousDebug->flap = this->flap;
    previousDebug->rudder = debug->rudder;
  //  previousDebug->slat = this->slat;
    previousDebug->thrust = debug->thrust;
    previousDebug->gears = debug->gears;
    */

}

void MainWindow::updateValues(joystick_event* event)
{
    ui->xAxisValue->setText(QString::number(event->stick_x, 'f', 2 ));
    ui->yAxisValue->setText(QString::number(event->stick_y, 'f', 2 ));
    ui->zAxisValue->setText(QString::number(event->stick_z, 'f', 2 ));
    ui->throttleValue->setText(QString::number(event->throttle, 'f', 2 ));


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
    currentModel = parseXML();
    planeState->update_model_parameters();
}

PlaneModel* parseXML()
{
    QString exe = QCoreApplication::applicationDirPath();
    QString fileName = exe + "/../../flight_simulator/src/input/planeModels.xml";
    printf(fileName.toLatin1());
    QFile* file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
        printf("ERROR: Cannot open XML File\n");
    QXmlStreamReader xmlParser(file);
    PlaneModel* node;
    while(!xmlParser.atEnd() && !xmlParser.hasError())
    {
        QXmlStreamReader::TokenType token = xmlParser.readNext();
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        if(token == QXmlStreamReader::StartElement)
        {
           if(xmlParser.name() == "PLANE")
           {
               node = new PlaneModel;
               printf("New Plane Model Loaded\n");
               continue;
           }
           if(xmlParser.name() == "NAME")
           {
                node->Name = xmlParser.readElementText();
                continue;
           }
           if(xmlParser.name() == "NICKNAME")
           {
                node->Nickname = xmlParser.readElementText();
                continue;
           }
           if(xmlParser.name() == "ENGINE")
                continue;
           if(xmlParser.name() == "MAXNUM")
           {
                node->numEngines = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "MAXTHRUST")
           {
                node->maxThrust = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "SURFACEAREA")
           {
                node->surfaceArea = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "EMPTYMASS")
           {
                node->minWeight = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "MAXMASS")
           {
                node->maxWeight = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "MAXFUEL")
           {
               continue;
           }
           if(xmlParser.name() == "ACTUATOR_SPEED")
           {
                node->maxActuatorSpeed = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "LIFT_SURFACES")
           {
                continue;
           }
           if(xmlParser.name() == "MAX_SLAT_POSANGLE")
           {
                node->maxSlatPOS = xmlParser.readElementText().toInt();
                continue;
           }
           if(xmlParser.name() == "MAX_SLAT_NEGANGLE")
           {
                node->maxSlatNEG = xmlParser.readElementText().toInt();
                continue;
           }
           if(xmlParser.name() == "MAX_FLAP_ANGLE")
           {
                node->maxFlapAngleNEG = xmlParser.readElementText().toInt();
                continue;
           }
           if(xmlParser.name() == "CONTROL_SURFACES")
           {
                continue;
           }
           if(xmlParser.name() == "MAX_ELEVATOR_POSANGLE")
           {
                node->maxElevPOS = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "MAX_ELEVATOR_NEGANGLE")
           {
                node->maxElevNEG = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "MAX_RUDDER_ANGLE")
           {
                node->maxRudderAngle = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "MAX_AILERON_ANGLE")
           {
                node->maxAileronAngle = xmlParser.readElementText().toFloat();
                continue;
           }
           if(xmlParser.name() == "DCOEFFICIENT")
           {
                node->dragCoeff = xmlParser.readElementText().toFloat();
                continue;
           }

        }
    }
    return node;
    //QString content = file->readAll();
   // printf(content.toLatin1());
}
