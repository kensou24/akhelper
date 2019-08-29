#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
# include <QtWidgets>
#endif // if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))

#if defined(_MSC_VER) && _MSC_VER >= 1600
# pragma execution_character_set("utf-8")
#endif // if defined(_MSC_VER) && _MSC_VER >= 1600

#define TIMEMS qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))
#define TIME qPrintable(QTime::currentTime().toString("HH:mm:ss"))
#define QDATE qPrintable(QDate::currentDate().toString("yyyy-MM-dd"))
#define QTIME qPrintable(QTime::currentTime().toString("HH-mm-ss"))
#define LCDTIME qPrintable(QDateTime::currentDateTime().toString( \
                             "MM-dd HH:mm:ss"))
#define DATETIME qPrintable(QDateTime::currentDateTime().toString( \
                              "yyyy-MM-dd HH:mm:ss"))
#define STRDATETIME qPrintable(QDateTime::currentDateTime().toString( \
                                 "yyyy-MM-dd-HH-mm-ss"))
#define STRDATETIMEMS qPrintable(QDateTime::currentDateTime().toString( \
                                   "yyyy-MM-dd-HH-mm-ss-zzz"))

#define AppName "QUI"
#define AppPath qApp->applicationDirPath()
#define AppDeskWidth qApp->desktop()->availableGeometry().width()
#define AppDeskHeight qApp->desktop()->availableGeometry().height()
