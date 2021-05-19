#include "gpio_pin.h"

#include<QFileSystemWatcher>
#include <QDebug>
#include <QFile>
#include<QTime>

GPIO_pin::GPIO_pin(int a_pinNumber, QString a_pinType, int a_initVal, QString edge, QObject *parent) : QObject(parent)
{
    FW = new QFileSystemWatcher;

    if( !setPinNumber(a_pinNumber) )    //1. assign GPIO pin number
        return;

    configurePinFilePaths();  //2. configure all gpio pin file path strings

    if( !exportPin() )    //3. export pin to file system
        return;

    if( !setPinDirection(a_pinType)) //4. set pin type (in/out)
        return;

    setPinValue( a_initVal);    //5. set initial value(OUTPUT ONLY, input is "0" by default

    setEdge(edge);  //configure edge detection

    FW->addPath(pathValue);  //watch for changes in value file

    lastInterrupt = QTime::currentTime();

    connect(this->FW, SIGNAL(fileChanged(QString)), this, SLOT(PinValueFileRead()));   //connect pin value file wathcher
}

GPIO_pin::GPIO_pin(QObject *parent) : QObject(parent)   //default constructor
{

}
GPIO_pin::~GPIO_pin(){
    delete FW;

    if(exportStatus == true)
           unexportPin();
}

bool GPIO_pin::setPinValue(int new_value)
{
    //check for invalid "new value"
    if( new_value != LOW && new_value != HIGH){
        qDebug()<<"Invalid value "<<new_value<<" for pin number: "<<this->pinNumber;
        resetFileWatcher();
        return false;
    }

    if( this->pinType == "out"){
        //doesn't write if the same value is already set on the physical pin
        if( new_value == this->pinVal){
            qDebug()<<"No change in pin value!";
            resetFileWatcher();
            return false;
        }

        QFile fp(this->pathValue);

        if(!fp.exists()){
            qDebug()<<"Unable to open file: "<<this->pathValue;
            return false;
        }

        fp.open(QIODevice::WriteOnly);
        QTextStream stream(&fp);

        stream<<new_value;

        this->pinVal = new_value;   //record change in virtual pin

        //emit signalPinValChange(this->pinNumber, new_value);

        qInfo()<<"Pin "<<pinNumber<<" value set to: "<<this->pinVal;
        fp.close();
    }

    resetFileWatcher();    //(file watcher expires after file change)
    return true;
}

void GPIO_pin::resetFileWatcher()
{
    if (QFile::exists(this->pathValue)) {
      this->FW->addPath(this->pathValue);
    }
    else
        qDebug()<<"File "<<this->pathValue<<" not found(not available for file watch)";
}

bool GPIO_pin::setPinNumber(int pinNumber)
{
    if( pinNumber >= 0 && pinNumber <= 27)
        this->pinNumber = pinNumber;  //set pin number
    else{
       qDebug()<<"Invalid pin number: "<<pinNumber<<" Aborting pin initialization";
       return false;
    }

    return true;
}

bool GPIO_pin::setPinDirection(QString direction)   //static function
 {
     if(direction != "in" && direction != "out"){
        qDebug()<<"Invalid direction set for pin: "<<this->pinNumber;
        return false;
     }

     QFile fp(this->pathDirection);
     fp.open(QIODevice::WriteOnly);
     QTextStream stream(&fp);
     if(!fp.exists()){
         qDebug()<<"Unable to open file: "<<this->pathDirection;
         return false;
     }
     stream<<direction;

     this->pinType = direction;   //record pin direction change
     fp.close();

     return true;
 }

bool GPIO_pin::exportPin()
{
    QFile fp(this->pathExport);
    fp.open(QIODevice::WriteOnly);
    QTextStream stream(&fp);
    if(!fp.exists()){
        qDebug()<<"Error opening file: "<<pathExport<<" Aborting pin "<<this->pinNumber<<" initialization...";
        return false;
    }
    stream<<this->pinNumber;
    this->exportStatus = true;
    fp.close();

    return true;
}

void GPIO_pin::setEdge(QString edge)
{
    QFile fp(this->pathEdge);
    fp.open(QIODevice::WriteOnly);

    if(!fp.exists()){
        qDebug()<<"Error opening file: "<<pathEdge<<" Aborting pin "<<this->pinNumber<<" initialization...";
        return;
    }

    QTextStream stream(&fp);
    stream<<edge;
    this->exportStatus = true;
    fp.close();

    return;
}

void GPIO_pin::unexportPin()
{
    QFile fp(this->pathUnexport);

    if(!fp.exists()){
        qDebug()<<"Error opening file: "<<this->pathUnexport;
        return;
    }

    fp.open(QIODevice::WriteOnly);
    QTextStream stream(&fp);

    stream<<this->pinNumber;
    fp.close();

    this->exportStatus = false;
}

 void GPIO_pin::configurePinFilePaths()
 {
     this->pathGPIO  = "/sys/class/gpio/gpio" + QString::number(this->pinNumber) + "/";
     this->pathDirection = this->pathGPIO + "direction";
     this->pathValue  = this->pathGPIO + "value";
     this->pathExport  = "/sys/class/gpio/export";
     this->pathUnexport  =  "/sys/class/gpio/unexport";;
     this->pathEdge = this->pathGPIO + "edge";

     //TEST
    /*
     this->pathDirection = "/home/andrija/GPIO_test/direction.txt";
     this->pathValue  = "/home/andrija/GPIO_test/value.txt";
     this->pathExport  ="/home/andrija/GPIO_test/export.txt";
     this->pathUnexport  = "/home/andrija/GPIO_test/unexport.txt";
    */
 }

 void GPIO_pin::PinValueFileRead()   //slot(called when the value file has changed)
 {
     //input detection delay(used to handle voltage bouncing)
     QTime currInterupt = QTime::currentTime();
     int delta = this->lastInterrupt.msecsTo(currInterupt);
          if(delta < INPUT_DELAY_MS){
              return;
          }
     this->lastInterrupt = currInterupt;

     QFile fp(this->pathValue);
     fp.open(QIODevice::ReadOnly);

     if(!fp.exists()){
         qDebug()<<"Error opening file: "<<this->pathValue;
         return;
     }
     QTextStream stream(&fp);
     QString temp = stream.read(1);
     int read_val = temp.toInt();

     emit signalPinValChange(this->pinNumber,read_val); //signal for QML
 }
