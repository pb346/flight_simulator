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
#include <QMediaPlayer>
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
    localPath = QDir::currentPath();
    localPath += "../../flight_simulator/src";  //use if running in QT Creator
    //localPath += "deploy/";                     //use if exporting executable
    runningFlag = 0;
    planeState = new Plane();
    debug = new DebugValues();
    previousDebug = new DebugValues();
    currentTime = new ProgramTime(0,0,0,0);
    angularFlip = false;
    previousDebug->afterburnerActive = 0;
    previousDebug->afterburner = 0;
    previousDebug->brakes = 1;
    previousDebug->gears = -1;
    headerTimerCount = 0;
    startFlag = 0;
    statusCount = 0;
    crashCount = 0;
    clockCycles = 0;
    cycleTimer = 0;
    spoolCounter = 0;
    pitch = 0;
    isCrashed = false;
    warningActive = false;
    crashActive = false;
    noFuelActive = false;
    engineActive = false;
    planeSet = false;
    spoolOne = false;
    warning1 = new QMediaPlayer();
    pullUp = new QMediaPlayer();
    noFuel = new QMediaPlayer();
    engine = new QMediaPlayer();
    headingInit();
    altitudeInit();
    speedInit();
    angularInit();
    statusInit();
    //planeState->m_velocity = 0;
    //planeState->z_position = 0;
    //updateSpeed();
    //updateAltitude();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::statusInit()
{
    statusObject = new QImage();
    statusObject->load(":new/prefix1/adjusted F16.png");
    statusImage = QPixmap::fromImage(*statusObject);
    statusScene = new QGraphicsScene(QRect(0,0,0,0));
    statusScene->addPixmap(statusImage);
    ui->graphicsViewStatus->setScene(statusScene);
}

void MainWindow::angularInit()
{
    angularObject = new QImage();
    angularObject->load(":new/prefix1/angularOrientation.png");//":new/prefix1/largerAO.png");
    angularImage = QPixmap::fromImage(*angularObject);
    //angularImage = angularImage.copy(0, -425, angularImage.width(), angularImage.height());
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

void MainWindow::updateAngular()
{
    pitch = planeState->pitch_angle;
    ui->pitch->setText(QString::number(pitch, 'f', 2));
    angularImage = QPixmap::fromImage(*angularObject);
    delete angularScene;
    angularScene = new QGraphicsScene(QRect(0,0,0,0));
    QGraphicsPixmapItem* item;// = angularScene->addPixmap(angularImage);
    QMatrix rm;
    double tempPitch;

    rm.rotate((-1)*planeState->roll_angle);
    angularImage = angularImage.transformed(rm);
    QPixmap original = QPixmap::fromImage(*angularObject);
    int offX = (angularImage.width()- original.width()) / 2;
    int offY = (angularImage.height() - original.height())/2;
    angularImage = angularImage.copy(offX, offY, original.width(), original.height());

    //angularImage = angularImage.copy(0, -425, angularImage.width(), angularImage.height());
    item = angularScene->addPixmap(angularImage);
    item->setPos(-136, -136);

    if(pitch > 45 && pitch <=135 ) // all sky
    {
  //      item = angularScene->addPixmap(angularImage);
        item->setPos(-136, 0);
    }

    else if(pitch > 135 && pitch <=225)//sky to ground
    {
        if(angularFlip == false)
        {
            rm.rotate(180);
            angularFlip = true;
        }
        angularImage = angularImage.transformed(rm);
        tempPitch = 275.0 - (275.0 * (pitch- 135.0)/90.0);
        angularImage = angularImage.copy(137, tempPitch, angularImage.width(), angularImage.height());
        delete angularScene;
        angularScene = new QGraphicsScene(QRect(0,0,0,0));
        item = angularScene->addPixmap(angularImage);
//        angularScene->addPixmap(angularImage);
    }
    else if(pitch > 225 && pitch <=315) //all ground
    {
        if(angularFlip == true)
        {
            rm.rotate(180);
            angularFlip = false;
        }
        angularImage = angularImage.transformed(rm);
        angularImage = angularImage.copy(137, 0, angularImage.width(), angularImage.height());
        delete angularScene;
        angularScene = new QGraphicsScene(QRect(0,0,0,0));
        item = angularScene->addPixmap(angularImage);
    //    angularScene->addPixmap(angularImage);
    }
    else if(pitch > 315 || pitch <= 45)
    {
    //    item = angularScene->addPixmap(angularImage);
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
    ui->graphicsViewAO->setScene(angularScene);
    ui->rollVal->setText(QString::number(planeState->roll_angle, 'f', 2));
}

void MainWindow::updateHeading()
{
    //+90 to get 0 degrees centered
    QMatrix rm;
    rm.rotate(360 - planeState->yaw_angle);
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
    ui->yawVal->setText(QString::number(planeState->yaw_angle, 'f', 2));
    ui->graphicsView1->setScene(scene);
}

void MainWindow::updateSpeed()
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

void MainWindow::updateAltitude()
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

void MainWindow::updateStatus()
{
    statusCount += 1;
    delete statusScene;
    statusScene = new QGraphicsScene(QRect(0,0,0,0));
    statusObject->load(":/new/prefix1/adjusted F16.png");
    statusScene->addPixmap(statusImage);
    statusImage = QPixmap::fromImage(*statusObject);
    ui->graphicsViewStatus->setScene(statusScene);
    if(statusCount <= 10)
    {
        if((planeState->mainFuel == 0 && ui->checkAux->isChecked() == 0) || (planeState->mainFuel == 0 && ui->checkAux->isChecked() == 1 && planeState->auxFuel == 0))
        {
            statusObject->load(":/new/prefix1/noFuel.png");
            statusImage = QPixmap::fromImage(*statusObject);
            QGraphicsPixmapItem* item = statusScene->addPixmap(statusImage);
            ui->graphicsViewStatus->setScene(statusScene);
            //no fuel
        }
    }
    if(statusCount == 20)
    {
        statusCount = 0;
    }
}

void MainWindow::updateFuel()
{
    //double fuelConsumption = 0;
    if(ui->checkAux->isChecked() == 1)
    {
        if(planeState->afterburnerActive == 1)
        {
            planeState->auxFuel -= .22;
            planeState->auxFuel -= (planeState->m_afterburner / 5000.0) * 1.55;
        //    fuelConsumption = .22 + (planeState->m_afterburner / 5000.0) * 1.55;
        }
        else
        {
            if((planeState->m_thrust/250.0) < 5.0)
            {
                planeState->auxFuel -= .011; //idle fuel consumption;
            //    fuelConsumption = .011;
            }
            else
            {
                planeState->auxFuel -= (planeState->m_thrust/25000.0)*.22;
            //    fuelConsumption = (planeState->m_thrust/25000.0)*.22;
            }
        }
    }
    else
    {
        if(planeState->afterburnerActive == 1)
        {
            planeState->mainFuel -= .22;
            planeState->mainFuel -= (planeState->m_afterburner /5000.0) *1.55;
        }
        else
        {
            if((planeState->m_thrust/ 250.0) < 5.0)
            {
                planeState->mainFuel -= .011; //idle fuel consumption;
            }
            else
            {
                planeState->mainFuel -= (planeState->m_thrust/25000.0)*.22;
            }
        }
    }
    if(planeState->mainFuel < 0.0)
    {
        planeState->mainFuel = 0;
    }
    if(planeState->auxFuel < 0.0)
    {
        planeState->auxFuel = 0;
    }
    //printf("FUEL %f\n", fuelConsumption);
}

void MainWindow::printSnappyResponse()
{
    int random = rand() % 5 + 1;
    ui->interfaceDisplay->append("Plane crashed at time: ");
    if(random == 1)
        ui->interfaceDisplay->append("AIR TRAFFIC CONTROL: 'I'm not sure what you planned to accomplish with that manuever.'");
    if(random == 2)
        ui->interfaceDisplay->append("AIR TRAFFIC CONTROL: 'Better luck next time.'");
    if(random == 3)
        ui->interfaceDisplay->append("AIR TRAFFIC CONTROL: 'Well you almost landed. Next time, try to keep the plane in one piece.'");
    if(random == 4)
        ui->interfaceDisplay->append("AIR TRAFFIC CONTROL: 'Splash down!'");
    if(random == 5)
        ui->interfaceDisplay->append("AIR TRAFFIC CONTROL: 'Management isn't going to be happy about this.'");
}
void MainWindow::updateTime()
{
   if(cycleTimer % 10 == 0)
   {
       currentTime->milli = 0;
       cycleTimer = 0;
       currentTime->seconds += 1;
       if(currentTime->seconds > 59)
       {
           currentTime->seconds = 0;
           currentTime->minutes += 1;
           if(currentTime->minutes > 59)
           {
               currentTime->minutes = 0;
               currentTime->hours += 1;
           }
       }
   }
   else
   {
       currentTime->milli = cycleTimer * 100;
   }
}

void MainWindow::onUpdateGUI(joystick_event* event)
{
    if(engineActive == false)
    {
        engine->setMedia(QUrl::fromLocalFile(localPath + "/media/engineSound.mp3"));
        engine->setVolume(50);
        engine->play();
        engineActive = true;
    }
    cycleTimer+=1;
    spoolCounter+=1;
    if(spoolCounter < 50 && spoolOne == false)
    {
        ui->interfaceDisplay->append("Engines are spooling...");
        ui->status->setText("SPOOLING");
        spoolOne = true;

    }
    if(spoolCounter == 50)
    {
        planeSet = true;
        ui->status->setText("RUNNING");
        ui->interfaceDisplay->append("Program Engaged");
    }
    if(planeSet == true)
    {


    if(warningActive == false && planeState->z_position < 1000 && (planeState->pitch_angle < 360 && planeState->pitch_angle > 180) )
    {

        warning1->setMedia(QUrl::fromLocalFile(localPath + "/media/warning.mp3"));
        warning1->setVolume(50);
        pullUp->setMedia(QUrl::fromLocalFile(localPath + "/media/pullUp.mp3"));
        pullUp->setVolume(50);
        pullUp->play();
        warning1->play();
        warningActive = true;
    }
    if(planeState->pitch_angle >= 0 && planeState->pitch_angle <= 180)
    //else if(warningActive == true && (planeState->pitch_angle > 0 || planeState->pitch_angle < 181))
    {
        warning1->stop();
        pullUp->stop();
        warningActive = false;
    }
    if(startFlag < 2)
    {
        event->throttle = -1.0;
        startFlag += 1;
    }
    if(isCrashed == true)
    {
        if(crashActive == false)
        {
        QMediaPlayer* player = new QMediaPlayer();
            ui->interfaceDisplay->append(localPath);
            player->setMedia(QUrl::fromLocalFile(localPath + "/media/crashSound.mp3"));
            player->setVolume(50);
            player->play();
            warning1->stop();
            engine->stop();
            crashActive = true;
        }
        crashCount += 1;
        delete statusScene;
        statusScene = new QGraphicsScene(QRect(0,0,0,0));
        statusObject->load(":/new/prefix1/adjusted F16.png");
        statusScene->addPixmap(statusImage);
        statusImage = QPixmap::fromImage(*statusObject);
        ui->graphicsViewStatus->setScene(statusScene);
        if(crashCount <= 10)
        {
            //if() engine, left ail, right ail, left elev, right elev, rudder, fuel
            statusObject->load(":/new/prefix1/fullDamage.png");
            statusImage = QPixmap::fromImage(*statusObject);
            QGraphicsPixmapItem* item = statusScene->addPixmap(statusImage);
            ui->graphicsViewStatus->setScene(statusScene);
        }
        if(crashCount == 20)
        {
            crashCount = 0;
        }
    return;
    }
    if(noFuelActive == false && (planeState->mainFuel == 0 && ui->checkAux->isChecked()== 0) || (planeState->mainFuel == 0 && ui->checkAux->isChecked()== 1 && planeState->auxFuel == 0))
    {
        noFuel->setMedia(QUrl::fromLocalFile(localPath + "media/noFuel.mp3"));
        noFuel->setVolume(50);
        noFuel->play();
        noFuelActive = true;

    }
    process_joystick_input(currentModel,event, &debug, &planeState);
    planeState->update_plane(debug->thrust, debug->elevator, debug->elevator, debug->aileronLeft, debug->aileronRight, debug->rudder, debug->afterburner, ui->checkAux->isChecked());
    headerTimerCount += 1;

    updateTime();


    updateValues(event);
    updateSliders(event);
    if(planeState->check_for_crash())
    {
    //    this->on_pushButton_clicked();
        isCrashed = true;
        planeState->m_velocity = 0;
        updateSpeed();
        printSnappyResponse();
        ui->status->setText("CRASHED");
    }


    if(headerTimerCount > 0)
    {
        updateHeading();
        updateAltitude();
        updateSpeed();
        updateAngular();
        updateStatus();
        updateFuel();
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
    if(previousDebug->auxActive == 1 && debug->auxActive == 0)
    {
        if(ui->checkAux->isChecked() == 1)
        {
            ui->checkAux->setChecked(0);
            planeState->auxActive = 0;
        }
        else
        {
            ui->checkAux->setChecked(1);
            planeState->auxActive = 1;
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

    debug->copyDebug(previousDebug);
    }
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
    ui->mainFuel->setText(QString::number(planeState->mainFuel, 'f', 2));
    ui->auxFuelValue->setText(QString::number(planeState->auxFuel, 'f', 2));
    ui->xAngleValue->setText(QString::number(planeState->unit_vector_front.x, 'f', 2));
    ui->yAngleValue->setText(QString::number(planeState->unit_vector_front.y, 'f', 2));
    ui->zAngleValue->setText(QString::number(planeState->unit_vector_front.z, 'f', 2));
 //   ui->xAngleValue->setText(QString::number(planeState->unit_vector_up.x, 'f', 2));
 //   ui->yAngleValue->setText(QString::number(planeState->unit_vector_up.y, 'f', 2));
 //   ui->zAngleValue->setText(QString::number(planeState->unit_vector_up.z, 'f', 2));
    QString t_hours = QString::number(currentTime->hours);
    QString t_minutes = QString::number(currentTime->minutes);
    QString t_seconds = QString::number(currentTime->seconds);
    QString t_millisec = QString::number(currentTime->milli);

    QString time =  t_hours+":" +t_minutes +":" + t_seconds +":" + t_millisec ;
    ui->timeVal->setText(time);
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
        //ui->status->setText("READING");
        ui->interfaceDisplay->append("Program has started.");
        //debug->thrust = 0;
        procThread->start();
    }
    else
    {
        runningFlag = 0;
        procThread->Stop = true;
        ui->status->setText("PAUSED");
        ui->interfaceDisplay->append("Program is paused");
    }
}

void MainWindow::onEventLoopStarted()
{
    ui->interfaceDisplay->append("Initializing...");
    currentModel = parseXML();
    planeState->mainFuel = currentModel->mainFuel;
    planeState->auxFuel = currentModel->auxFuel;
}

PlaneModel* MainWindow::parseXML()
{
    QString fileName = localPath + "/input/planeModels.xml";
    ui->interfaceDisplay->append("Loading planeModels.xml");
    printf(fileName.toLatin1());

    QFile* file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->interfaceDisplay->append("ERROR: Cannot open XML File\n");
    }
    QXmlStreamReader xmlParser(file);
    PlaneModel* node;
    while(!xmlParser.atEnd() && !xmlParser.hasError())
    {
        QXmlStreamReader::TokenType token = xmlParser.readNext();
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        if(xmlParser.name() == "MODEL_FILE")
        {
            continue;
        }
        if(token == QXmlStreamReader::StartElement)
        {
           if(xmlParser.name() == "PLANE")
           {
               node = new PlaneModel;
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
                QString loadedModel = "Loading " + node->Name + ": " + node->Nickname;
                ui->interfaceDisplay->append(loadedModel);
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
           if(xmlParser.name() == "MAXAFTER")
           {
               continue; //need to add
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
               node->maxFuel = xmlParser.readElementText().toFloat();
               continue;
           }
           if(xmlParser.name() == "MAINFUEL")
           {
               node->mainFuel = xmlParser.readElementText().toFloat();
               continue;
           }
           if(xmlParser.name() == "AUXFUEL")
           {
               node->auxFuel = xmlParser.readElementText().toFloat();
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
    ui->interfaceDisplay->append("Using " + node->Name + ": " + node->Nickname);
    return node;
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
    //delete previousState; //causes a crash
    delete currentModel;
    QApplication::quit();
}

void MainWindow::on_pushButton_quit_clicked()
{
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
    rerun = false;
    QApplication::quit();
}
