#ifndef GPIO_PIN_H
#define GPIO_PIN_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFile>
#include <QTime>

#define LOW 0
#define HIGH 1
#define INPUT_DELAY_MS 400

class GPIO_pin : public QObject
{
   Q_OBJECT
public:
   explicit GPIO_pin(QObject *parent = nullptr);
   explicit GPIO_pin(int a_pinNumber, QString a_pinType, int a_initVal, QString edge, QObject *parent = nullptr);
   ~GPIO_pin();

   int pinNumber;
   int pinVal;
   bool exportStatus;
   QString pinType;
   QTime lastInterrupt;

   //GPIO PIN file paths
   QString pathGPIO; //base directory
   QString pathValue;
   QString pathDirection;
   QString pathExport;
   QString pathUnexport;
   QString pathEdge;

   QFileSystemWatcher* FW;  //watches for changes in the pin value file

   bool setPinNumber(int);
   Q_INVOKABLE bool setPinValue(int);   //for output pins, called from QML
   bool setPinDirection(QString);   //set pin as output/input
   bool exportPin();    //export pin to file system
   void unexportPin();
   void configurePinFilePaths();
   void resetFileWatcher();
   void setEdge(QString edge);

signals:
   void signalPinValChange(int, int);   //can be used to communicate with QML
public slots:
   void PinValueFileRead(); //called when there is a change in the sysfile
};

#endif // GPIO_PIN_H
