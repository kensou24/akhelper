#ifndef UIDEMO1_H
#define UIDEMO1_H

#include <QDialog>
#if defined(_MSC_VER) && _MSC_VER >= 1600
# pragma execution_character_set("utf-8")
#endif // if defined(_MSC_VER) && _MSC_VER >= 1600

#include <QMainWindow>
#include <QTimer>
#include "opencv2/opencv.hpp"
namespace Ui {
class UIDemo1;
}

class QMediaPlayer;

class UIDemo1 : public QDialog {
  Q_OBJECT

public:

  explicit UIDemo1(QWidget *parent = 0);
  ~UIDemo1();

  void enableOrDisable(bool flag);

private:

  Ui::UIDemo1 *ui;
  bool max;
  QRect location;

private slots:

  void initForm();
  void buttonClick();

private slots:

  void on_btnMenu_Min_clicked();
  void on_btnMenu_Max_clicked();
  void on_btnMenu_Close_clicked();

public slots:

  void        initComobox();
  void        initTimer();
  void        initAudioPlayer();
  void        slotTimeout();

  void        getDeivceName();
  cv::Mat     getDeviceCapScreen();


  cv::Mat     slotGrabFullScreen();
  cv::Mat     slotGrabAdbScreen();

  cv::Point2f processImage(const cv::Mat& inputImage,
                           cv::Mat      & secondImage,
                           cv::Mat      & outputImage);

  // 当QUI样式改变时自动应用颜色值
  void changeStyle(const QString& qssFile);

  void getAdbMessage();

protected:

  void readFile();
  void saveFile();

protected slots:

  void initProcess();

private slots:

  void on_pushButton_select_clicked();
  void adbClickPoint(cv::Point2f middle);
  void clickPoint(cv::Point2f middle);

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void enableButton();
  void disableButton();

  void on_pushButton_4_clicked();

  void on_pushButton_queryadb_clicked();

  void on_pushButton_start_clicked();

  void on_pushButton_stop_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_6_clicked();

private:

  QTimer *timer;
  QList<cv::Mat>matsList;
  QStringList addedPngFilename;
  bool stop;
  QProcess *myProcess;
  cv::Mat current;
  QMediaPlayer *mPlayer;
  quint64 lastClickTime;

  QImage localImage;
  QPixmap localFromImage;
};

#endif // UIDEMO1_H
