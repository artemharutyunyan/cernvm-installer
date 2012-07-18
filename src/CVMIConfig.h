/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMICONFIG_H
#define CVMICONFIG_H

#define DEFAULT_RELEASE_SOURCE "http://rbuilder.cern.ch/installer/installer.cgi"

#include <QString>
#include <QFile>

class CVMIConfig
{
public:
  CVMIConfig();
  ~CVMIConfig(){}

  QString getImageDir();
  QString getVMDir();
  QString getHypervisorDir();
  QString getShowDevReleases();
  QString getReleaseSource();
 
  void setImageDir (QString dir);
  void setVMDir (QString dir);
  void setHypervisorDir (QString dir);
  void setShowDevReleases (QString show); 
  void setReleaseSource (QString source);
 
  void write();
  void read();
  void reset();  

private:
  // Config variables
  QString imageDir;
  QString vmDir;
  QString hypervisorDir;
  QString showDevReleases;
  QString releaseSource;

  // Conf file 
  QString confFileName;  
  QString confDirPath;
  QString confFilePath;
};

#endif // CVMICONFIG_H

