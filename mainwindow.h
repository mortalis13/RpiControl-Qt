#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QPixmap>

namespace Ui {
class MainWindow;
}
class GetImageThread;
class CommandThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void runKodiAction(QString action);
    void runCommand(QString action);

private:
    void addActions();
    void loadConnectInfo();
    void numClick(int num);


public slots:
  void updateScreenshot(QPixmap img);
  void log(QString msg);
  void getImageThreadFinished();
  
private slots:
  void bNum0Click();
  void bNum1Click();
  void bNum2Click();
  void bNum3Click();
  void bNum4Click();
  void bNum5Click();
  void bNum6Click();
  
  void bScreenClick();
  void bScreenClick_Sync();
  void bBackClick();
  void bVol0Click();
  void bUpClick();
  void bDownClick();
  void bRestartKodiClick();
  void bScreenTakeClick();
  
  void bTestClick();
  
  void execCustomCommand();

public:
    QString host;
    QString pass;
    QString sysType;
private:
    Ui::MainWindow *ui;
    GetImageThread* getImageThread;
    CommandThread* commandThread;
};


class CommandThread : public QThread
{
  Q_OBJECT
  
public:
  explicit CommandThread(QObject *parent = 0);
  void setMainWindow(MainWindow* mainWindow);
  void setHost(QString host);
  void setPass(QString pass);
  void setCommand(QString pass);
  void run();
  void stop();

public:
    QString host;
    QString pass;
    QString command;
    
    bool stopped;

private:
  MainWindow* mainWindow;

signals:
  void updateScreenshot(QPixmap img);
  void log(QString msg);
};

class GetImageThread : public QThread
{
  Q_OBJECT
  
public:
  explicit GetImageThread(QObject *parent = 0);
  void setMainWindow(MainWindow* mainWindow);
  void setHost(QString host);
  void setPass(QString pass);
  void run();
  void stop();

public:
    QString host;
    QString pass;
    
    bool stopped;

private:
  MainWindow* mainWindow;

signals:
  void updateScreenshot(QPixmap img);
  void log(QString msg);
};

#endif // MAINWINDOW_H
