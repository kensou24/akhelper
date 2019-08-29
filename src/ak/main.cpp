#include "form/quiwidget.h"
#include "form/appinit.h"
#include "form/uidemo1.h"
#include "LesCrashHandler.h"
#include <QApplication>

#pragma comment(lib, "User32.lib")

int main(int argc, char *argv[])
{
  QFileInfo appPath(QString::fromLocal8Bit(argv[0]));
  QString   appDir(appPath.absoluteDir().path());
  QString   crashReporterPath = QString("crashreporter").arg(
    appDir.isEmpty() ? "." : appDir);

  LesCrashHandler::instance()->Init(crashReporterPath,
                                    appPath.absoluteFilePath(),
                                    crashReporterPath);

  QApplication a(argc, argv);

  a.setOrganizationName("les");
  a.setFont(QFont("Microsoft Yahei", 9));
  a.setWindowIcon(QIcon(":/main.ico"));

  QUIWidget::setCode();

  QUIWidget::setTranslator(":/image/qt_zh_CN.qm");
  QUIWidget::setTranslator(":/image/widgets.qm");
  AppInit::Instance()->start();

  // 读取配置文件
  // QUIConfig::ConfigFile =
  // QString("%1/%2").arg(QUIWidget::appPath()).arg("qui_config.ini");
  // QUIConfig::ReadConfig();

  QUIWidget qui;

  // 设置标题
  qui.setTitle("测试界面");

  // 设置标题文本居中
  qui.setAlignment(Qt::AlignCenter);

  // 设置窗体可拖动大小
  qui.setSizeGripEnabled(true);

  // 设置换肤下拉菜单可见
  qui.setVisible(QUIWidget::BtnMenu, true);

  // 设置标题栏高度
  // qui.setTitleHeight(50);

  // 设置按钮宽度
  // qui.setBtnWidth(50);

  // 设置左上角图标-图形字体
  // qui.setIconMain(QChar(0xf099), 11);

  // 设置左上角图标-图片文件
  // qui.setPixmap(QUIWidget::Lab_Ico, ":/main.ico");

  UIDemo1 uide;
  uide.changeStyle(":/qss/black.css");
  uide.show();

  return a.exec();
}
