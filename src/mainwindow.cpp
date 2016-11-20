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
#include <QCloseEvent>
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317675238467481846766940513200056812714526356082778577134275778960917363717872146844090122495343014654958537105079227968925892354201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859502445945534690830264252230825334468503526193118817101000313783875288658753320838142061717766914730359825349042875546873115956286388235378759375195778185778053217122680661300192787661119590921642019893809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913151557485724245415069595082953311686172785588907509838175463746493931925506040092770167113900984882401285836160356370766010471018194295559619894676783744944825537977472684710404753464620804668425906949129331367702898915210475216205696602405803815019351125338243003558764024749647326391419927260426992279678235478163600934172164121992458631503028618297455570674983850549458858692699569092721079750930295532116534498720275596023648066549911988183479775356636980742654252786255181841757467289097777279380008164706001614524919217321721477235014144197356854816136115735255213347574184946843852332390739414333454776241686251898356948556209921922218427255025425688767179049460165346680498862723279178608578438382796797668145410095388378636095068006422512520511739298489608412848862
extern bool rerun;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //rerun;
    ui->setupUi(this);
    procThread = new timerThread(this);
    connect(procThread, SIGNAL(updateGUI(joystick_event*)),this, SLOT(onUpdateGUI(joystick_event*)));
    ui->checkBrake->setChecked(true);
    runningFlag = 0;
    planeState = new Plane();
    debug = new DebugValues();
    previousDebug = new DebugValues();
    previousDebug->afterburnerActive = 0;
    previousDebug->afterburner = 0;
    previousDebug->brakes = 1;
    previousDebug->gears = -1;
    headerTimerCount = 0;
    startFlag = 0;
    clockCycles = 0;
    pitch = 0;
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
  //  pitch += 5; //planeState->pitch_angle;
    pitch = planeState->pitch_angle;



    ui->pitch->setText(QString::number(pitch, 'f', 2));
    angularImage = QPixmap::fromImage(*angularObject);
    delete angularScene;
    angularScene = new QGraphicsScene(QRect(0,0,0,0));
    QGraphicsPixmapItem* item;// = angularScene->addPixmap(angularImage);
    QMatrix rm;
    double tempPitch;

    double z = planeState->unit_vector_up.z;
    double y = planeState->unit_vector_up.y;
    double x = planeState->unit_vector_up.x;
    double xy = sqrt(pow(y,2)+pow(x,2));
    double rollAngle = 0.0;
    if(z > 0 && xy > 0) {
        rollAngle = atan(-xy/z)*180/PI;
    }
    else if(z < 0 && xy > 0) {
        rollAngle = 180+atan(-xy/z)*180/PI;
    }
    else if(z > 0 && xy < 0) {
        rollAngle = 360+atan(-xy/z)*180/PI;
    }
    else if(z < 0 && xy < 0) {
        rollAngle = 180+atan(-xy/z)*180/PI;
    }
    else if(z == 0 && xy > 0) {
        rollAngle = 90;
    }
    else if(z == 1 && xy == 0) {
        rollAngle = 0;
    }
    else if(z == 0 && xy < 0) {
        rollAngle = 270;
    }
    else if(z == -1 && xy == 0) {
        rollAngle = 180;
    }/*
    rm.rotate(rollAngle);*/
    //angularImage = angularImage.transformed(rm);
    //angularImage = angularImage.copy(137, 137, angularImage.width(), angularImage.height());
    //angularScene->addPixmap(angularImage);

    if(pitch > 45 && pitch <=135 ) // all sky
    {
        item = angularScene->addPixmap(angularImage);
        item->setPos(-136, 0);
    }
    else if(pitch > 135 && pitch <=225)//sky to ground
    {
        rm.rotate(180);
        angularImage = angularImage.transformed(rm);
        tempPitch = 275.0 - (275.0 * (pitch- 135.0)/90.0);
        angularImage = angularImage.copy(137, tempPitch, angularImage.width(), angularImage.height());
        angularScene->addPixmap(angularImage);
    }
    else if(pitch > 225 && pitch <=315) //all ground
    {
        rm.rotate(180);
        angularImage = angularImage.transformed(rm);
        angularImage = angularImage.copy(137, 0, angularImage.width(), angularImage.height());
        angularScene->addPixmap(angularImage);
    }
    else if(pitch > 315 || pitch <= 45)
    {
        item = angularScene->addPixmap(angularImage);
        if(pitch > 315)
        {
            item->setPos(-136, -275 + ((pitch -315.0)/90.0 )* 275.0);
        }
        else
        {
            item->setPos(-136, -137 + (137.0 * pitch/45.0));
        }
    }
    else
    {
        angularScene->addPixmap(angularImage); //hopefully stops ocassional crash
    }
    //planeState->pitch_angle = pitch;
    ui->graphicsViewAO->setScene(angularScene);
    ui->rollVal->setText(QString::number(rollAngle, 'f', 2));
}

void MainWindow::updateHeading(joystick_event* event)
{
    //+90 to get 0 degrees centered
    double x = planeState->unit_vector_front.x/sqrt(pow(planeState->unit_vector_front.x,2) + pow(planeState->unit_vector_front.y,2));
    double y = planeState->unit_vector_front.y/sqrt(pow(planeState->unit_vector_front.x,2) + pow(planeState->unit_vector_front.y,2));
    if(x > 0 && y > 0) {
        headingAngle = atan(y/x)*180/PI;
    }
    else if(x < 0 && y > 0) {
        headingAngle = 180+atan(y/x)*180/PI;
    }
    else if(x > 0 && y < 0) {
        headingAngle = 360+atan(y/x)*180/PI;
    }
    else if(x < 0 && y < 0) {
        headingAngle = 180+atan(y/x)*180/PI;
    }
    else if(x == 0 && y == 1) {
        headingAngle = 90;
    }
    else if(x == 1 && y == 0) {
        headingAngle = 0;
    }
    else if(x == 0 && y == -1) {
        headingAngle = 270;
    }
    else if(x == -1 && y == 0) {
        headingAngle = 180;
    }
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
    ui->yawVal->setText(QString::number(headingAngle, 'f', 2));
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
    if(startFlag < 2)
    {
        event->throttle = -1.0;
        startFlag += 1;
    }

    process_joystick_input(currentModel,event, &debug, &planeState);
    planeState->update_plane(debug->thrust, debug->elevator, debug->elevator, debug->aileronLeft, debug->aileronRight, debug->rudder, debug->afterburner);
    headerTimerCount += 1;
    updateValues(event);
    updateSliders(event);
    if(planeState->check_for_crash())
    {
        this->on_pushButton_clicked();
        ui->status->setText("CRASHED");
    }


    if(headerTimerCount > 0)
    {
        updateHeading(event);
        updateAltitude(event);
        updateSpeed(event);
        updateAngular(event);
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

    /*
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
    */
/*
    if(previousDebug->brakes == 1 && debug->brakes == 0)
    {
        if(planeState->brakes == 0)
        {
         //   planeState->brakes = 1;
         //   ui->checkBrake->setChecked(true);
        }
        else
        {
        //    planeState->brakes = 0;
        //    ui->checkBrake->setChecked(false);
        }
    }
    if(planeState->brakes == 1)
    {
        if(planeState->m_velocity < 15.0)
        {
         //   planeState->m_velocity = 0;
        }
        else
        {
         //   planeState->m_velocity -= 15;
        }
    }
*/
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
    //ui->xAngleValue->setText(QString::number(planeState->unit_vector_front.x, 'f', 2));
    //ui->yAngleValue->setText(QString::number(planeState->unit_vector_front.y, 'f', 2));
    //ui->zAngleValue->setText(QString::number(planeState->unit_vector_front.z, 'f', 2));
    ui->xAngleValue->setText(QString::number(planeState->unit_vector_up.x, 'f', 2));
    ui->yAngleValue->setText(QString::number(planeState->unit_vector_up.y, 'f', 2));
    ui->zAngleValue->setText(QString::number(planeState->unit_vector_up.z, 'f', 2));

    }
void MainWindow::updateSliders(joystick_event* event)
{
    ui->xSlider->setSliderPosition(((event->stick_x + 1)/2) *100);
    ui->ySlider->setSliderPosition(((event->stick_y + 1)/2) *100);
    ui->zSlider->setSliderPosition(((event->stick_z + 1)/2) *100);
    ui->throttleSlider->setSliderPosition(((event->throttle + 1)/2) *100);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    rerun = false;
    event->accept();
}

void MainWindow::on_pushButton_clicked()
{
    if(!runningFlag)
    {
        procThread->Stop = false;
        runningFlag = 1;
        ui->status->setText("READING");
        //debug->thrust = 0;
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

void MainWindow::on_pushButton_reset_clicked()
{
    if(runningFlag)
    {
        this->on_pushButton_clicked();
    }
    procThread->exit(0);
    delete procThread;
    delete scene;
    delete altScene;
    delete speedScene;
    delete angularScene;
    delete imageObject;
    delete altObject;
    delete speedObject;
    delete angularObject;
    delete planeState;
    delete debug;
    delete previousDebug;
    delete previousState;
    delete currentModel;
    QApplication::quit();
}
