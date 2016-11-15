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
    headerTimerCount = 0;
    headingInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::headingInit()
{
    imageObject = new QImage();
    imageObject->load(":/new/prefix1/heading.bmp");
    image = QPixmap::fromImage(*imageObject);
    rotateImage = QPixmap::fromImage(*imageObject);
    scene = new QGraphicsScene(QRect(0,0,0,0));
    QMatrix rm;
    rm.rotate(90);
    headingAngle = 0;
    image = image.transformed(rm);
    image = image.scaled(400,400); //300 300
    //image = image.scaled(300, 300);
    rotateImage = image.copy(0, -220, image.width(), image.height());
    //rotateImage = image.copy(0, -120, image.width(), image.height() );
    scene->addPixmap(rotateImage);
    scene->addPixmap(rotateImage);
    ui->graphicsView1->setScene(scene);
}
void MainWindow::updateHeading(joystick_event* event)
{
    float rudder = event->stick_z;
    rudder = rudder*5;
    headingAngle += rudder;
    QMatrix rm;
    rm.rotate(headingAngle);
    rotateImage = image.transformed(rm);
    int offX = (rotateImage.width()- image.width()) / 2;
    int offY = (rotateImage.height() - image.height())/2;
    rotateImage = rotateImage.copy(offX, offY, image.width(), image.height());

    delete scene;
    scene = new QGraphicsScene(QRect(0,0,0,0));
    QGraphicsPixmapItem* item =scene->addPixmap(rotateImage);
    item->setPos(-50, 0);
    item =scene->addPixmap(rotateImage);
    item->setPos(-50, 0);
    ui->graphicsView1->setScene(scene);
}

void MainWindow::onUpdateGUI(joystick_event* event)
{
    //printf("%i %i %i\n", (int)planeState->pitch_angle, (int)planeState->yaw_angle, (int)planeState->roll_angle);
    headerTimerCount += 1;
    updateValues(event);
    updateSliders(event);
    planeState->process_joystick_input(currentModel,event, &debug);
    if(headerTimerCount > 50)
    {
        updateHeading(event);
        headerTimerCount = 0;
    }
    //ui->leftAilVal->setText(QString::number(debug->aileronLeft, 'f', 2));
    ui->rightAilVal->setText(QString::number(debug->aileronRight, 'f', 2));
    ui->leftAilVal->setText(QString::number(debug->aileronLeft, 'f', 2));

    ui->throttleValue->setText(QString::number(debug->thrust, 'f', 2));
    ui->rudVal->setText(QString::number(debug->rudder, 'f', 2));
    ui->ElevVal->setText(QString::number(debug->elevator, 'f', 2));
    ui->checkGear->setChecked(planeState->gears_Deployed);
    ui->afterburnVal->setText(QString::number(debug->afterburner, 'f', 2));
    ui->checkAfterburner->setChecked(planeState->afterburnerActive);
    ui->flapVal->setText(QString::number(planeState->flap, 'f', 2));

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
    if(previousDebug->afterburnerActive == 1 && debug->afterburnerActive == 0)
    {
        if(ui->checkAfterburner->isChecked() == 1)
        {
            ui->checkAfterburner->setChecked(0);
            planeState->afterburnerActive = 0;
        }
        else
        {
            ui->checkAfterburner->setChecked(1);
            planeState->afterburnerActive = 1;
        }
    }

    if(previousDebug->flapDown == 1 && debug->flapDown == 0)
    {
        if(planeState->flap > ((-1.0)*currentModel->maxFlapAngleNEG))
        {
            planeState->flap -= 1;//.5;
            //planeState->right_aileron_angle -= 1;
        }
    }
    if(previousDebug->flapUp == 1 && debug->flapUp == 0)
    {
        if(planeState->flap < 0.0 )
        {
            planeState->flap += 1;//.5;
            //planeState->left_aileron_angle -= 1;
        }
    }
    debug->copyDebug(previousDebug);
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
