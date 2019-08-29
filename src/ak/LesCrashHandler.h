#pragma once

#include <QtCore/QString>

class CrashHandlerPrivate;
class  LesCrashHandler {
public:

  static LesCrashHandler* instance();
  void                    Init(const QString& reportPath,
                               const QString& appPath,
                               const QString& crashReporterFullPath);

  void setReportCrashesToSystem(bool report);
  bool writeMinidump();

private:

  LesCrashHandler();
  ~LesCrashHandler();
  Q_DISABLE_COPY(LesCrashHandler)
  CrashHandlerPrivate *d;
};
