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
    startFlag = 0;
    clockCycles = 0;
    headingInit();
    altitudeInit();
    speedInit();
    angularInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::angularInit()
{
    angularObject = new QImage();
    angularObject->load(":new/prefix1/angularOrientation.png");
    angularImage = QPixmap::fromImage(*angularObject);
    angularScene = new QGraphicsScene(QRect(0,0,0,0));
    angularScene->addPixmap(angularImage);
    ui->graphicsViewAO->setScene(angularScene);
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
    rotateImage = image.copy(0, -280, image.width(), image.height());
    scene->addPixmap(rotateImage);  //need double draw to compensate for compression loss
    scene->addPixmap(rotateImage);
    ui->graphicsView1->setScene(scene);
}

void MainWindow::altitudeInit()
{
    altObject = new QImage();
    altObject->load(":new/prefix1/altitude50Kft.png");
    altImage = QPixmap::fromImage(*altObject);
    altScene = new QGraphicsScene(QRect(0,0,0,0));
    QGraphicsPixmapItem* item = altScene->addPixmap(altImage);
  //  item->setPos(-18, -7560);
    ui->graphicsViewAlt->setScene(altScene);
}

void MainWindow::speedInit()
{
    speedObject = new QImage();
    speedObject->load(":new/prefix1/speed2K.png");
    speedImage = QPixmap::fromImage(*speedObject);
    speedScene = new QGraphicsScene(QRect(0,0,0,0));
    speedScene->addPixmap(speedImage);
    ui->graphicsViewSpeed->setScene(speedScene);
}

void MainWindow::updateAngular(joystick_event* event)
{


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

void MainWindow::updateSpeed(joystick_event* event)
{
    double speed = (planeState->m_velocity * 3600.0)/(5280.0);
    speedImage = QPixmap::fromImage(*speedObject);
    delete speedScene;
    speedScene = new QGraphicsScene(QRect(0,0,0,0));
    QGraphicsPixmapItem* item = speedScene->addPixmap(speedImage);
    float tempSpeed = speed * 1.65;
    item->setPos(-18, (-3110 + tempSpeed));
    ui->graphicsViewSpeed->setScene(speedScene);
    ui->speedVal->setText(QString::number(speed, 'f', 1));
}

void MainWindow::updateAltitude(joystick_event* event)
{
    int altitude = (planeState->z_position);
    altImage = QPixmap::fromImage(*altObject);
    delete altScene;
    altScene = new QGraphicsScene(QRect(0,0,0,0));
    QGraphicsPixmapItem* item = altScene->addPixmap(altImage);
    float tempAlt = altitude * 1.65/10.0;
    item->setPos(-18, (-7565 + tempAlt));
    ui->graphicsViewAlt->setScene(altScene);
    ui->altValue->setText(QString::number( altitude ));
}

void MainWindow::onUpdateGUI(joystick_event* event)
{
    //clockCycles++;
    //printf("Cycle %i\n", clockCycles);
    //printf("%i %i %i\n", (int)planeState->pitch_angle, (int)planeState->yaw_angle, (int)planeState->roll_angle);
    if(startFlag == 0)
    {
        event->throttle = -1.0;
        startFlag = 1;
    }

    process_joystick_input(currentModel,event, &debug, &planeState);
    planeState->update_plane(debug->thrust, debug->elevator, debug->elevator, debug->aileronLeft, debug->aileronRight, debug->rudder, debug->afterburner);
    headerTimerCount += 1;
    updateValues(event);
    updateSliders(event);
    if(headerTimerCount > 0)
    {
        updateHeading(event);
        updateAltitude(event);
        updateSpeed(event);
        headerTimerCount = 0;
    }
    //ui->leftAilVal->setText(QString::number(debug->aileronLeft, 'f', 2));
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
    ui->rightAilVal->setText(QString::number(debug->aileronRight, 'f', 2));
    ui->leftAilVal->setText(QString::number(debug->aileronLeft, 'f', 2));
    ui->throttleValue->setText(QString::number(debug->thrust, 'f', 2));
    ui->rudVal->setText(QString::number(debug->rudder, 'f', 2));
    ui->ElevVal->setText(QString::number(debug->elevator, 'f', 2));
    ui->checkGear->setChecked(planeState->gears_Deployed);
    ui->afterburnVal->setText(QString::number(debug->afterburner, 'f', 2));
    ui->checkAfterburner->setChecked(planeState->afterburnerActive);
    ui->liftVal->setText(QString::number(planeState->m_lift_left + planeState->m_lift_right, 'f', 2));
    ui->flapVal->setText(QString::number(planeState->flap, 'f', 2));
    ui->xAxisValue->setText(QString::number(planeState->x_position, 'f', 2 ));
    ui->yAxisValue->setText(QString::number(planeState->y_position, 'f', 2 ));
    ui->zAxisValue->setText(QString::number(planeState->z_position, 'f', 2 ));
    ui->speedValue->setText(QString::number(planeState->m_velocity, 'f', 2 ));
    ui->accelValue->setText(QString::number(planeState->m_acceleration, 'f', 2 ));
    ui->dragValue->setText(QString::number((planeState->m_drag_left + planeState->m_drag_right), 'f', 2 ));
    ui->thrustValue->setText(QString::number(planeState->m_thrust, 'f', 2 ));
    ui->airDensity->setText(QString::number(planeState->air_pressure, 'f', 2));
    ui->xAngleValue->setText(QString::number(planeState->unit_vector_front.x, 'f', 2));
    ui->yAngleValue->setText(QString::number(planeState->unit_vector_front.y, 'f', 2));
    ui->zAngleValue->setText(QString::number(planeState->unit_vector_front.z, 'f', 2));

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
