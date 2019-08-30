#include "uidemo1.h"
#include "ui_uidemo1.h"
#include "quiwidget.h"
#include <QScreen>
#include <QtConcurrent>
#include <QDebug>
#include <QTemporaryFile>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QtMultimedia>
#include <QMutex>
#include "alg.h"
#include "Windows.h"

// 生成结果数据
cv::Mat g_out;
QMutex  g_outMutext;


UIDemo1::UIDemo1(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::UIDemo1), timer(Q_NULLPTR), stop(false), myProcess(new QProcess),
  mPlayer(Q_NULLPTR), lastClickTime(0)
{
  ui->setupUi(this);
  this->initForm();
  QUIWidget::setFormInCenter(this);

  initComobox();
  initTimer();
  readFile();
  initProcess();
  initAudioPlayer();
}

UIDemo1::~UIDemo1()
{
  delete ui;
}

void UIDemo1::initForm()
{
  this->max      = false;
  this->location = this->geometry();
  this->setProperty("form", true);
  this->setProperty("canMove", true);
  this->setWindowFlags(
    Qt::FramelessWindowHint | Qt::WindowSystemMenuHint |
    Qt::WindowMinMaxButtonsHint);

  IconHelper::Instance()->setIcon(ui->labIco, QChar(0xF099), 30);
  IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xF068));
  IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xF067));
  IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xF00d));

  ui->widgetTitle->setProperty("form", "title");
  ui->widgetTop->setProperty("nav", "top");
  ui->labTitle->setText("测试助手");
  ui->labTitle->setFont(QFont("Microsoft Yahei", 20));
  this->setWindowTitle(ui->labTitle->text());

  ui->stackedWidget->setStyleSheet("QLabel{font:12pt;}");

  // 单独设置指示器大小
  int addWidth    = 20;
  int addHeight   = 10;
  int rbtnWidth   = 15;
  int ckWidth     = 13;
  int scrWidth    = 12;
  int borderWidth = 3;

  QStringList qss;
  qss.append(QString(
               "QComboBox::drop-down,QDateEdit::drop-down,QTimeEdit::drop-down,QDateTimeEdit::drop-down{width:%1px;}").arg(
               addWidth));
  qss.append(QString(
               "QComboBox::down-arrow,QDateEdit[calendarPopup=\"true\"]::down-arrow,QTimeEdit[calendarPopup=\"true\"]::down-arrow,"
               "QDateTimeEdit[calendarPopup=\"true\"]::down-arrow{width:%1px;height:%1px;right:2px;}").arg(
               addHeight));
  qss.append(QString("QRadioButton::indicator{width:%1px;height:%1px;}").arg(
               rbtnWidth));
  qss.append(QString(
               "QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:%1px;height:%1px;}").arg(
               ckWidth));
  qss.append(QString(
               "QScrollBar:horizontal{min-height:%1px;border-radius:%2px;}QScrollBar::handle:horizontal{border-radius:%2px;}"
               "QScrollBar:vertical{min-width:%1px;border-radius:%2px;}QScrollBar::handle:vertical{border-radius:%2px;}").arg(
               scrWidth).arg(scrWidth / 2));
  qss.append(QString(
               "QWidget#widget_top>QToolButton:pressed,QWidget#widget_top>QToolButton:hover,"
               "QWidget#widget_top>QToolButton:checked,QWidget#widget_top>QLabel:hover{"
               "border-width:0px 0px %1px 0px;}").arg(borderWidth));
  qss.append(QString(
               "QWidget#widgetleft>QPushButton:checked,QWidget#widgetleft>QToolButton:checked,"
               "QWidget#widgetleft>QPushButton:pressed,QWidget#widgetleft>QToolButton:pressed{"
               "border-width:0px 0px 0px %1px;}").arg(borderWidth));
  this->setStyleSheet(qss.join(""));


  QSize icoSize(32, 32);
  int   icoWidth = 85;

  // 设置顶部导航按钮
  QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
  foreach(QToolButton * btn, tbtns) {
    btn->setIconSize(icoSize);
    btn->setMinimumWidth(icoWidth);
    btn->setCheckable(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
  }

  ui->btnMain->click();


  ui->label_2->hide();
  ui->label_3->hide();

  ui->lineEdit_x->hide();
  ui->lineEdit_y->hide();
}

void UIDemo1::initProcess() {
  connect(myProcess, static_cast<void (QProcess::*)(
                                   int)>(&QProcess::finished),
          this,     [&]() {
    ui->plainTextEdit->appendPlainText("devides 获取完毕");
  });


  connect(myProcess, &QProcess::readyReadStandardOutput,
          this,
          &UIDemo1::getAdbMessage);

  connect(myProcess, &QProcess::readyReadStandardError,
          this,     [&]() {
    QByteArray data = myProcess->readAllStandardError();
    ui->plainTextEdit->appendPlainText("devides error 数据项是-" +
                                       QString::fromLocal8Bit(data));
  });
}

void UIDemo1::initAudioPlayer() {
  mPlayer = new QMediaPlayer;

  QString filePath = QCoreApplication::applicationDirPath() + "/warning.mp3";

  if (QFile(filePath).exists()) {
    mPlayer->setMedia(QUrl::fromLocalFile(filePath));
  } else {
    // 测试播放音乐
    mPlayer->setMedia(QUrl("qrc:/sound/sound/nokia-tune.mp3"));
  }
  mPlayer->setVolume(30);
}

void UIDemo1::buttonClick()
{
  QToolButton *b = (QToolButton *)sender();
  QString name   = b->text();

  QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();

  foreach(QToolButton * btn, tbtns) {
    if (btn == b) {
      btn->setChecked(true);
    } else {
      btn->setChecked(false);
    }
  }

  if (name == "主界面") {
    ui->stackedWidget->setCurrentIndex(0);
  } else if (name == "系统设置") {
    ui->stackedWidget->setCurrentIndex(1);
  } else if (name == "查询") {
    ui->stackedWidget->setCurrentIndex(2);
  } else if (name == "调试帮助") {
    ui->stackedWidget->setCurrentIndex(3);
  } else if (name == "用户退出") {
    exit(0);
  }
}

void UIDemo1::on_btnMenu_Min_clicked()
{
  showMinimized();
}

void UIDemo1::on_btnMenu_Max_clicked()
{
  if (max) {
    this->setGeometry(location);
    this->setProperty("canMove", true);
  } else {
    location = this->geometry();
    this->setGeometry(qApp->desktop()->availableGeometry());
    this->setProperty("canMove", false);
  }

  max = !max;
}

void UIDemo1::on_btnMenu_Close_clicked()
{
  close();
}

void UIDemo1::initComobox()
{
  QList<QScreen *> localScreens = QGuiApplication::screens();

  for (int i = 0; i < localScreens.size(); i++) {
    ui->comboBox_display->addItem(localScreens[i]->name(), i);
  }

  ui->comboBox_alg->addItem("算法1-AKE/AKE",   0);
  ui->comboBox_alg->addItem("算法2-AGAST/ORB", 1);
}

void UIDemo1::initTimer()
{
  timer = new QTimer();
  timer->setInterval(5000);
  timer->setSingleShot(true);
  connect(timer, &QTimer::timeout, this, &UIDemo1::slotTimeout);
}

void UIDemo1::slotTimeout()
{
  qDebug() << "time out";


  ui->plainTextEdit->appendPlainText("Dr.,开始本次计算-" + QString(TIMEMS));


  using namespace cv;

  if (ui->comboBox_type->currentText().compare("adb检测")  == 0) {
    qDebug() << "使用adb检测";
    current =   getDeviceCapScreen();
  } else {
    qDebug() << "使用屏幕检测";
    current = slotGrabFullScreen();
  }


  // 生成结果数据
  QList<cv::Mat> outList;

  for (int i = 0; i < this->matsList.size(); i++) {
    cv::Mat empty;
    outList.append(empty);
  }
  cv::TickMeter meter;
  meter.start();

  QList<QFuture<cv::Point2f> > resultList;

  for (int i = 0; i < this->matsList.size(); i++) {
    QFuture<cv::Point2f> result =
      QtConcurrent::run(this, &UIDemo1::processImage, matsList[i], current,
                        outList[i]);
    resultList.append(result);
  }

  do {
    QApplication::processEvents();
    bool result = true;

    for (int i = 0; i < resultList.size(); i++) {
      if (!resultList[i].isFinished()) {
        result = false;
      }
    }

    if (result) {
      break;
    }
  }
  while (true);

  meter.stop();
  QString info =  "Dr.执行查询" + QString::number(resultList.size()) +
                 "总共花费" +
                 QString::number(meter.getTimeMilli()) + "ms";
  qDebug() << info;
  ui->plainTextEdit->appendPlainText(info);

  for (int i = 0; i < resultList.size(); i++) {
    cv::Point2f localResult = resultList[i].result();
    QString     info        =  "Dr.,匹配到点 --" + QString::number(localResult.x) +
                              "," + QString::number(localResult.y);

    // ui->plainTextEdit->appendPlainText(info);
    qDebug() << info;

    //选择了点击，则进行数据点击
    if (ui->checkBox_clickPic->isChecked()) {
      if (ui->comboBox_type->currentText().compare("adb检测") == 0) {
        adbClickPoint(localResult);
      } else {
        clickPoint(localResult);
      }
    }
  }
  ui->plainTextEdit->appendPlainText("Dr.,本次计算结束-" + QString(TIMEMS));

  if (ui->checkBox_showMatch->isChecked()) {
    QMutexLocker locker(&g_outMutext);


    if (!(g_out.empty())) {
      localImage =  QImage(g_out.data,
                           g_out.cols,
                           g_out.rows,
                           g_out.step,
                           QImage::Format_RGB888);

      localFromImage = QPixmap::fromImage(localImage.rgbSwapped());

      if (!localFromImage.isNull()) {
        ui->lab_result->setPixmap(localFromImage);
      }
    }
  }

  /**检查时间是否正确*/
  if (lastClickTime != 0) {
    if (QDateTime::currentSecsSinceEpoch() - lastClickTime >
        ui->lineEdit_warning->text().toInt()) {
      on_pushButton_5_clicked();
    }
  } else {
    lastClickTime = QDateTime::currentSecsSinceEpoch();
  }

  if (!stop) {
    timer->start();
  }
}

cv::Mat UIDemo1::slotGrabFullScreen()

{
  QString filePathName = "full-";

  filePathName +=
    QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
  filePathName += ".png";

  qDebug() << "tmp file name" << filePathName;

  QList<QScreen *> localScreens = QGuiApplication::screens();

  if (ui->comboBox_display->currentIndex() >= localScreens.size()) {
    ui->plainTextEdit->appendPlainText("Dr.没有找到屏幕");
    cv::Mat empty;
    return empty;
  }

  qDebug() << "抓取第" << ui->comboBox_display->currentIndex() << "个屏幕";
  QScreen *screen =
    localScreens[ui->comboBox_display->currentIndex()];

  QPixmap pixmap = screen->grabWindow(0,
                                      ui->lineEdit_x->text().toInt(),
                                      ui->lineEdit_y->text().toInt(),
                                      1000,
                                      600);

  if (!pixmap.save(filePathName, "png"))
  {
    qDebug() << "save full screen failed" << endl;
  }

  using namespace cv;
  cv::Mat originalMat = imread(filePathName.toStdString());

  if (!originalMat.empty())
  {
    qDebug() << "读取数据成功啦";

    /**删除临时文件*/
    QFile::remove(filePathName);
  } else {
    qDebug() << "读取数据错误啦";
  }

  return originalMat;
}

void UIDemo1::getAdbMessage() {
  QString inf = QString::fromLocal8Bit(myProcess->readAllStandardOutput());

  ui->plainTextEdit->appendPlainText("devides std 数据项是-\n" +
                                     inf);

  inf = inf.replace("\r", " ").replace("\t", " ");

  QString deviceInfo = "List of devices attached";

  if (inf.startsWith(deviceInfo)) {
    QStringList deviceInfos = inf.split("\n");

    if (deviceInfos.size() > 1) {
      qDebug() << deviceInfos;
      ui->comboBox_adb->clear();
      foreach(const QString& data, deviceInfos) {
        if (data.startsWith(deviceInfo)) {
          continue;
        }

        if (data.simplified().isEmpty()) {
          continue;
        }
        ui->comboBox_adb->addItem(data, data.split(" ").at(0));
      }
    }
  }
}

void UIDemo1::getDeivceName()
{
  QString program = ui->lineEdit_adb->text();
  QStringList arguments;

  arguments << "devices";
  myProcess->start(program, arguments);
  ui->plainTextEdit->appendPlainText("开始获取adb设备信息");
}

cv::Mat UIDemo1::getDeviceCapScreen()
{
  QString program = ui->lineEdit_adb->text();
  QStringList arguments;

  arguments << "-s";
  arguments << ui->comboBox_adb->currentData(Qt::UserRole).toString();
  arguments << "shell";
  arguments << "screencap -p /sdcard/screen.png";
  qDebug() << "执行 " << arguments;
  int result = QProcess::execute(program, arguments);
  ui->plainTextEdit->appendPlainText("开始请求adb设备截屏-" + QString::number(result));
  arguments.clear();

  arguments << "-s";
  arguments << ui->comboBox_adb->currentData(Qt::UserRole).toString();
  arguments << "pull";
  arguments << "/sdcard/screen.png";
  arguments <<  QApplication::applicationDirPath() + "/screen.png";

  int localExecute = QProcess::execute(program, arguments);
  qDebug() << "localExecute " << localExecute;
  qDebug() << "执行 " << arguments;
  ui->plainTextEdit->appendPlainText("开始拉取adb设备截屏数据-"  +
                                     QString::number(localExecute));

  QString filePathName =  QApplication::applicationDirPath() + "/screen.png";

  qDebug() << "tmp file name" << filePathName;


  using namespace cv;
  cv::Mat originalMat = imread(filePathName.toStdString());

  if (!originalMat.empty())
  {
    qDebug() << "读取数据成功啦";
  } else {
    qDebug() << "读取数据错误啦";
  }

  return originalMat;
}

// 通过adb获取数据
cv::Mat UIDemo1::slotGrabAdbScreen()
{
  cv::Mat temp;

  return temp;
}

void UIDemo1::changeStyle(const QString& qssFile)
{
  QString fileName = qssFile;

  if (!fileName.isEmpty()) {
    QFile file(fileName);

    if (file.open(QFile::ReadOnly)) {
      QString str = file.readAll();

      static QString qss;

      if (qss == str) {
        return;
      }

      qss = str;
      QString paletteColor = str.mid(20, 7);
      qApp->setPalette(QPalette(QColor(paletteColor)));
      qApp->setStyleSheet(str);
    }
  }
}

cv::Point2f UIDemo1::processImage(const cv::Mat& inputImage,
                                  cv::Mat      & secondImage,
                                  cv::Mat      & outputImage)
{
  return g_alg->processImage(inputImage,
                             secondImage,
                             outputImage,
                             ui->comboBox_alg->currentText(),
                             ui->comboBox_alg->currentData().toInt(),
                             ui->checkBox_showMatch->isChecked());
}

void UIDemo1::readFile()
{
  addedPngFilename.clear();
  matsList.clear();
  QString iniFilePath = QCoreApplication::applicationDirPath() +
                        "\\configure.ini";
  QSettings settings(iniFilePath, QSettings::IniFormat);
  settings.beginGroup("file");
  int size = settings.beginReadArray("files");

  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);

    QString fileName = settings.value("path").toString();


    if (addedPngFilename.contains(fileName))
    {
      qDebug() << ("这个图片已经添加过啦");
    }

    using namespace cv;
    cv::Mat mathFile = imread(fileName.toStdString());

    if (!mathFile.empty()) {
      matsList.append(mathFile);
      addedPngFilename.append(fileName);

      /**添加到页面显示*/
      QListWidgetItem *listItem = new QListWidgetItem(fileName);
      QIcon ico(fileName);
      listItem->setIcon(ico);
      ui->listWidget->addItem(listItem);
    } else {
      qDebug() << ("这个图片读取失败啦");
    }
  }
  settings.endArray();
  settings.endGroup();

  settings.beginGroup("others");
  ui->lineEdit_adb->setText(settings.value("adb").toString());
  settings.endGroup();
}

void UIDemo1::saveFile() {
  QString iniFilePath = QCoreApplication::applicationDirPath() +
                        "\\configure.ini";
  QSettings settings(iniFilePath, QSettings::IniFormat);

  settings.beginGroup("file");
  settings.beginWriteArray("files");

  for (int i = 0; i < addedPngFilename.size(); ++i) {
    settings.setArrayIndex(i);
    settings.setValue("path", addedPngFilename.at(i));
  }
  settings.endArray(); // QString::fromLocal8Bit(this->myEmailConfigure.content.toUtf8().toBase64()));
  settings.endGroup();


  settings.beginGroup("others");
  settings.setValue("adb", ui->lineEdit_adb->text());
  settings.endGroup();
  settings.sync();
}

void UIDemo1::on_pushButton_select_clicked()
{
  /**添加待比较的图片*/
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("打开"),
                                                  "",
                                                  tr("图片 文件(*.png)"));

  if (fileName.isEmpty() && ui->lineEdit_file->text().isEmpty()) {
    QUIWidget::showMessageBoxError("您还没有选择图片文件!", 3);
    return;
  } else {
    if (addedPngFilename.contains(fileName))
    {
      QUIWidget::showMessageBoxError("这个图片已经添加过啦!", 3);
      return;
    }

    using namespace cv;
    cv::Mat mathFile = imread(fileName.toStdString());

    if (!mathFile.empty()) {
      matsList.append(mathFile);
      addedPngFilename.append(fileName);

      /**添加到页面显示*/
      QListWidgetItem *listItem = new QListWidgetItem(fileName);
      QIcon ico(fileName);
      listItem->setIcon(ico);
      ui->listWidget->addItem(listItem);

      saveFile();
    } else {
      QUIWidget::showMessageBoxError("这个图片读取失败啦!", 3);
      return;
    }
  }
}

void UIDemo1::adbClickPoint(cv::Point2f middle)
{
  if (middle.x > 10) {
    QString program = ui->lineEdit_adb->text();
    QStringList arguments;

    arguments << "-s";
    arguments << ui->comboBox_adb->currentData(Qt::UserRole).toString();
    arguments << "shell";
    arguments << "input";
    arguments << "tap";
    arguments << QString::number(middle.x);
    arguments << QString::number(middle.y);
    qDebug() << "执行 " << arguments;
    int result = QProcess::execute(program, arguments);
    ui->plainTextEdit->appendHtml("<red>开始adb点击屏幕-result" + QString::number(
                                    result) + "-at-" +
                                  QString::number(middle.x)
                                  + "," + QString::number(middle.y) + "</red>");

    lastClickTime = QDateTime::currentSecsSinceEpoch();
  }
}

void UIDemo1::clickPoint(cv::Point2f middle)
{
  QPoint old = QCursor::pos();

  if (middle.x > 10) {
    QList<QScreen *> localScreens = QGuiApplication::screens();

    if (ui->comboBox_display->currentIndex() >= localScreens.size()) {
      ui->plainTextEdit->appendPlainText("Dr.没有找到屏幕");
      return;
    }
    qDebug() << "使用第" << ui->comboBox_display->currentIndex() << "个屏幕";
    QScreen *screen =
      localScreens[ui->comboBox_display->currentIndex()];
    QCursor::setPos(screen, QPoint(middle.x + ui->lineEdit_x->text().toInt(),
                                   middle.y + ui->lineEdit_y->text().toInt()));
    Sleep(100);

    for (int i = 0; i < 2; i++) {
      mouse_event(
        MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,
        0,
        0,
        0,
        0);
      Sleep(100);
      mouse_event(
        MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,
        0,
        0,
        0,
        0);
      qDebug() << "模拟点击完毕啦";
    }
    ui->plainTextEdit->appendPlainText(
      "Dr.,点击了坐标(" + QString::number(
        middle.x + ui->lineEdit_x->text().toInt()) + "," + QString::number(
        middle.y + ui->lineEdit_y->text().toInt()) + ")");

    QCursor::setPos(old);

    lastClickTime = QDateTime::currentSecsSinceEpoch();
  } else {}
}

void UIDemo1::on_pushButton_clicked()
{
  if (ui->comboBox_adb->currentText().isEmpty()) {
    QUIWidget::showMessageBoxError("您还没有选择设备!", 3);
    return;
  }

  lastClickTime = QDateTime::currentSecsSinceEpoch();

  if (timer->isActive()) {
    qDebug() << "已经启动啦";
  } else {
    timer->start();
    ui->plainTextEdit->appendPlainText(
      "Dr.开始工作");
  }
  disableButton();
  stop = false;
}

void UIDemo1::on_pushButton_2_clicked()
{
  QListWidgetItem *localCurrentItem = ui->listWidget->currentItem();

  if (!localCurrentItem) {
    return;
  }

  QString string = localCurrentItem->data(Qt::DisplayRole).toString();
  addedPngFilename.removeAll(string);
  saveFile();

  ui->listWidget->clear();
  readFile();
}

void UIDemo1::on_pushButton_3_clicked()
{
  stop = true;
  timer->stop();
  qDebug() << "停止";
  ui->plainTextEdit->appendPlainText("Dr.停止工作");
  enableButton();
}

void UIDemo1::enableOrDisable(bool flag)
{
  QList<QPushButton *> allPButtons = ui->frame->findChildren<QPushButton *>();

  foreach(QPushButton * button, allPButtons) {
    button->setDisabled(flag);
  }

  QList<QLineEdit *> allLineEdit = ui->frame->findChildren<QLineEdit *>();

  foreach(QLineEdit * line, allLineEdit) {
    line->setDisabled(flag);
  }

  QList<QComboBox *> allCom = ui->frame->findChildren<QComboBox *>();

  foreach(QComboBox * combo, allCom) {
    combo->setDisabled(flag);
  }
}

void UIDemo1::enableButton()
{
  enableOrDisable(false);
}

void UIDemo1::disableButton()
{
  enableOrDisable(true);

  ui->pushButton_3->setEnabled(true);
}

void UIDemo1::on_pushButton_4_clicked()
{
  // 点击选择adb路径

  /**添加待比较的图片*/
  QString filePath = QFileDialog::getOpenFileName(this,
                                                  tr("打开"),
                                                  "",
                                                  tr("adb程序(adb.exe)"));

  if (filePath.isEmpty() && ui->lineEdit_adb->text().isEmpty()) {
    QUIWidget::showMessageBoxError("您还没有选择adb.exe!", 3);
    return;
  } else {
    ui->lineEdit_adb->setText(filePath);
    saveFile();
  }
}

void UIDemo1::on_pushButton_queryadb_clicked()
{
  getDeivceName();
}

void UIDemo1::on_pushButton_5_clicked()
{
  if (mPlayer->state() == QMediaPlayer::PlayingState) {
    return;
  }

  mPlayer->play();
}

void UIDemo1::on_pushButton_6_clicked()
{
  if (mPlayer->state() == QMediaPlayer::PlayingState) {
    mPlayer->stop();
  }
}

/////////////////////高级功能开发
///
///
///
// arknights INFO
const QString ArkNights_PACKAGE_NAME          = "com.hypergryph.arknights"; // 这个是官服的设置
const QString ArkNights_PACKAGE_NAME_BILIBILI =
  "com.hypergryph.arknights.bilibili";                                      // 这是b服的设置
const QString ArkNights_ACTIVITY_NAME = "com.u8.sdk.U8UnityContext";
void UIDemo1::on_pushButton_start_clicked()
{
  QString program = ui->lineEdit_adb->text();
  QStringList arguments;

  arguments << "-s";
  arguments << ui->comboBox_adb->currentData(Qt::UserRole).toString();
  arguments << "shell";
  arguments << "am";
  arguments << "start";
  arguments << "-n";
  arguments << QString("%1/%2").arg(ArkNights_PACKAGE_NAME).arg(
    ArkNights_ACTIVITY_NAME);
  qDebug() << "执行 " << arguments;
  int result = QProcess::execute(program, arguments);
  ui->plainTextEdit->appendPlainText("开始请求adb打开程序-" + QString::number(result));
}

void UIDemo1::on_pushButton_stop_clicked()
{
  QString program = ui->lineEdit_adb->text();
  QStringList arguments;

  arguments << "-s";
  arguments << ui->comboBox_adb->currentData(Qt::UserRole).toString();
  arguments << "shell";
  arguments << "am";
  arguments << "force-stop";
  arguments << QString("%1").arg(ArkNights_PACKAGE_NAME);
  qDebug() << "执行 " << arguments;
  int result = QProcess::execute(program, arguments);
  ui->plainTextEdit->appendPlainText("开始请求adb关闭程序-" + QString::number(result));
}
