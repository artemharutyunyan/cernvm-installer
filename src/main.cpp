/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "CVMIDialog.h"
#include "CVMIConfig.h"

#include <iostream>

void log (QtMsgType type, const char* msg)
{
  QString txt(msg);

  if (type == QtFatalMsg)
    abort();

  QFile logFile(QDir::tempPath() + QDir::separator() + "debug.log");
 
  logFile.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream logStream (&logFile);
  logStream << txt << endl;
}

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(CVMIDialog);
  QApplication app(argc, argv);
    
  qInstallMsgHandler(log);

  CVMIDialog dialog;
  //return dialog.exec();
  dialog.show();
  return app.exec();
}
