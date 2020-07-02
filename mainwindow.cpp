#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QProcess>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  getImageThread = nullptr;
  addActions();
  
  QString def_host = "192.168.20.20";
  QString def_pass = "libreelec";
  QString def_type = "kodi";
  
  ui->leHost->setText(def_host);
  ui->lePass->setText(def_pass);
  ui->leSysType->setText(def_type);
}

MainWindow::~MainWindow() {
  delete ui;
}


void MainWindow::addActions() {
  connect( ui->bNum0, SIGNAL(clicked()), this, SLOT(bNum0Click()) );
  connect( ui->bNum1, SIGNAL(clicked()), this, SLOT(bNum1Click()) );
  connect( ui->bNum2, SIGNAL(clicked()), this, SLOT(bNum2Click()) );
  connect( ui->bNum3, SIGNAL(clicked()), this, SLOT(bNum3Click()) );
  connect( ui->bNum4, SIGNAL(clicked()), this, SLOT(bNum4Click()) );
  connect( ui->bNum5, SIGNAL(clicked()), this, SLOT(bNum5Click()) );
  connect( ui->bNum6, SIGNAL(clicked()), this, SLOT(bNum6Click()) );
  
  connect( ui->bScreen, SIGNAL(clicked()), this, SLOT(bScreenClick()) );
  connect( ui->bScreenTake, SIGNAL(clicked()), this, SLOT(bScreenTakeClick()) );
  connect( ui->bBack, SIGNAL(clicked()), this, SLOT(bBackClick()) );
  connect( ui->bVol0, SIGNAL(clicked()), this, SLOT(bVol0Click()) );
  connect( ui->bUp, SIGNAL(clicked()), this, SLOT(bUpClick()) );
  connect( ui->bDown, SIGNAL(clicked()), this, SLOT(bDownClick()) );
  connect( ui->bRestartKodi, SIGNAL(clicked()), this, SLOT(bRestartKodiClick()) );
  
  connect( ui->bTest, SIGNAL(clicked()), this, SLOT(bTestClick()) );
  connect( ui->leCustomCommand, SIGNAL(returnPressed()), this, SLOT(execCustomCommand()) );
}


// ------ Buttons
void MainWindow::bTestClick() {
  qDebug() << "== bTestClick()";
}

void MainWindow::bScreenClick() {
  loadConnectInfo();
  
  ui->lbImage->clear();
  if (getImageThread) {
    getImageThread->stop();
  }
  
  getImageThread = new GetImageThread();
  getImageThread->setMainWindow(this);
  getImageThread->setHost(this->host);
  getImageThread->setPass(this->pass);
  
  connect( getImageThread, SIGNAL(finished()), this, SLOT(getImageThreadFinished()) );
  connect( getImageThread, SIGNAL(updateScreenshot(QPixmap)), this, SLOT(updateScreenshot(QPixmap)) );
  connect( getImageThread, SIGNAL(log(QString)), this, SLOT(log(QString)) );
  
  getImageThread->start();
}

void MainWindow::bNum0Click() {
  numClick(0);
}
void MainWindow::bNum1Click() {
  numClick(1);
}
void MainWindow::bNum2Click() {
  numClick(2);
}
void MainWindow::bNum3Click() {
  numClick(3);
}
void MainWindow::bNum4Click() {
  numClick(4);
}
void MainWindow::bNum5Click() {
  numClick(5);
}
void MainWindow::bNum6Click() {
  numClick(6);
}

void MainWindow::bBackClick() {
  QString action = "Action(Back)";
  runKodiAction(action);
  bScreenClick();
}

void MainWindow::bVol0Click() {
  QString action = "SetVolume(0)";
  runKodiAction(action);
}

void MainWindow::bUpClick() {
  QString action = "Action(Up)";
  runKodiAction(action);
  bScreenClick();
}

void MainWindow::bDownClick() {
  QString action = "Action(Down)";
  runKodiAction(action);
  bScreenClick();
}

void MainWindow::bScreenTakeClick() {
  QString action = "TakeScreenshot";
  runKodiAction(action);
}

void MainWindow::bRestartKodiClick() {
  QString cmd = "killall -9 " + this->sysType + ".bin";
  runCommand(cmd);
}


// ------ Actions
void MainWindow::numClick(int num) {
  QString action = "Action(Number" + QString::number(num) + ")";
  runKodiAction(action);
  bScreenClick();
}

void MainWindow::runCommand(QString cmd) {
  qDebug() << "== runCommand()";
  loadConnectInfo();

  QString command = "plink -pw " + this->pass + " root@" + this->host + " " + cmd;
  log("command: " + command);
  
  QProcess proc(this);
  proc.start(command);
  proc.waitForFinished();
  proc.close();
}

void MainWindow::runKodiAction(QString action) {
  qDebug() << "== runKodiAction()";
  loadConnectInfo();

  QString command = "plink -pw " + this->pass + " root@" + this->host + " " + this->sysType + "-send --action='" + action + "'";
  log("command: " + command);
  
  QProcess proc(this);
  proc.start(command);
  proc.waitForFinished();
  proc.close();
}

void MainWindow::execCustomCommand() {
  loadConnectInfo();
  
  QString command = ui->leCustomCommand->text();
  log(command);
  
  QProcess proc(this);
  proc.start(command);
  proc.waitForFinished();
  
  QByteArray resBA = proc.readAllStandardOutput();
  QString res = QString::fromUtf8(resBA);
  res = res.replace("\r\r", "\r");
  res = res.trimmed();
  log("---RESPONSE---\n" + res + "\n-----");
  
  proc.close();
}

void MainWindow::getImageThreadFinished() {
  if (getImageThread != NULL) {
    // todo: delete thread when it's finished completely
    // delete getImageThread;
  }
}


// ------ Utils
void MainWindow::loadConnectInfo() {
  this->host = ui->leHost->text();
  this->pass = ui->lePass->text();
  this->sysType = ui->leSysType->text();
}

void MainWindow::updateScreenshot(QPixmap img) {
  this->log("updateScreenshot()");
  ui->lbImage->setPixmap(img);
  ui->lbImage->setScaledContents(true);
}

void MainWindow::log(QString msg) {
  qDebug() << msg;
  ui->teOutput->append(msg);
}


// -- GetImageThread
GetImageThread::GetImageThread(QObject *parent) :
  QThread(parent)
{
  stopped = false;
}

void GetImageThread::setMainWindow(MainWindow* mainWindow) {
  this->mainWindow = mainWindow;
}

void GetImageThread::setHost(QString host) {
  this->host = host;
}

void GetImageThread::setPass(QString pass) {
  this->pass = pass;
}

void GetImageThread::stop() {
  stopped = true;
}

void GetImageThread::run() {
  stopped = false;
  qDebug() << "GetImageThread.run()";
  
  // create screenshot on the device, wait until it's created, copy the image to a local folder, assign it to the component
  
  QString src_img = "/storage/screenshots";
  QString img_name = "scr.png";
  QString dir_path = "./";
  
  QString command0 = QString("plink -pw %2 root@%1 " + this->mainWindow->sysType + "-send --action=\'TakeScreenshot(%3/%4)\'").arg(this->host, this->pass, src_img, img_name);
  emit log("command0: " + command0);
  QProcess proc0;
  proc0.start(command0);
  proc0.waitForFinished();
  proc0.close();
  
  if (stopped) return;
  
  bool img_downloaded = false;
  while (!img_downloaded) {
    QString command = QString("plink -pw %2 root@%1 ls -l %3/%4").arg(this->host, this->pass, src_img, img_name);
    emit log("command: " + command);
    QProcess proc;
    proc.start(command);
    proc.waitForFinished();
    
    if (stopped) return;
    
    QByteArray resBA = proc.readAllStandardOutput();
    QString res = QString::fromUtf8(resBA);
    proc.close();
    
    res = res.replace("\r\r", "\r");
    res = res.trimmed();
    
    emit log(res);
    
    QStringList attrs = res.split(QRegExp("\\s+"));
    if (attrs.length() > 5) {
      QString img_size = attrs[4].trimmed();
      emit log("img_size: " + img_size);
      
      if (img_size != "0") {
        img_downloaded = true;
        break;
      }
    }
  }
  emit log("img_downloaded");
  
  // ---
  QString command = QString("pscp -pw %2 root@%1:%3/%4 %5").arg(this->host, this->pass, src_img, img_name, dir_path);
  emit log("command: " + command);
  QProcess proc;
  proc.start(command);
  proc.waitForFinished();
  if (stopped) return;
  proc.close();
  // ---
  
  QString dst_img_path = QString("%1%2").arg(dir_path, img_name);
  emit log("dst_img_path: " + dst_img_path);
  QPixmap img(dst_img_path);
  if (stopped) return;
  emit updateScreenshot(img);
}
