/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  dcctransfer.cpp  -  description
  begin:     Mit Aug 7 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com

  $Id$
*/

#include "stdlib.h"

#include <qtimer.h>

#include <klocale.h>
#include <kdebug.h>

#include "dcctransfer.h"
#include "konversationapplication.h"

DccTransfer::DccTransfer(KListView* parent,DccType type,QString folder,QString partner,QString name,QString size,QString ipString,QString portString) :
             KListViewItem(parent)
{
  kdDebug() << "DccTransfer::DccTransfer()" << endl;

  dccSocket=0;
  
  setType(type);
  setPartner(partner);
  setFile(name);
  setSize(size.toUInt());
  setPosition(0);
  setIp(ipString);
  setPort(portString);
  setStatus(Queued);
  setFolder(folder);

  setBufferSize(KonversationApplication::preferences.getDccBufferSize());
  buffer=new char[bufferSize];

  statusText.append(i18n("Queued"));
  statusText.append(i18n("Lookup ..."));
  statusText.append(i18n("Connecting ..."));
  statusText.append(i18n("Offering ..."));
  statusText.append(i18n("Running"));
  statusText.append(i18n("Stalled"));
  statusText.append(i18n("Failed"));
  statusText.append(i18n("Aborted"));
  statusText.append(i18n("Done"));

  transferStarted=QDateTime::currentDateTime();
  lastActive=QDateTime::currentDateTime();

  updateCPS();

  if(getType()==Get)
  {
    setText(6,getIp());
    setText(7,getPort());

    if(KonversationApplication::preferences.getDccAutoGet()) startGet();
  }
  else
  {
    setStatus(Offering);
//    startSend();
  }
}

DccTransfer::~DccTransfer()
{
  kdDebug() << "DccTransfer::~DccTransfer(" << getFile() << ")" << endl;

  delete[] buffer;

  if(dccSocket)
  {
    dccSocket->close();
    delete dccSocket;
  }
}

void DccTransfer::startGet()
{
  kdDebug() << dccFolder << endl;

  if(KonversationApplication::preferences.getDccAddPartner())
    dir.setPath(dccFolder+"/"+dccPartner.lower());
  else
    dir.setPath(dccFolder);
  
  kdDebug() << dir.path() << endl;

  if(!dir.exists())
  {
    kdDebug() << dir.path() << " does not exist. Creating ..." << endl;
    // QT's mkdir() is too stupid to do this alone, so we use the shell command
    system(QString("mkdir -p "+dir.path()).latin1());
  }

  QString fullName(dccFile);
  if(KonversationApplication::preferences.getDccAddPartner()) fullName=dccPartner.lower()+"."+fullName;
  file.setName(dir.path()+"/"+fullName);
  
  kdDebug() << "Opening Socket to " << getIp() << " ..." << endl;
  setStatus(Lookup);

  dccSocket=new KExtendedSocket(getIp(),getPort().toUInt(),KExtendedSocket::inetSocket);

//QTimer* newTimer=new QTimer();
//connect(newTimer,SIGNAL (timeout()),this,SLOT (check()));
//newTimer->start(250);

  dccSocket->enableRead(false);
  dccSocket->enableWrite(false);
  dccSocket->setTimeout(5);

  kdDebug() << "Socket created." << endl;

  connect(dccSocket,SIGNAL (lookupFinished(int))  ,this,SLOT (lookupFinished(int)) );
  connect(dccSocket,SIGNAL (connectionSuccess())  ,this,SLOT (connectionSuccess()) );
  connect(dccSocket,SIGNAL (connectionFailed(int)),this,SLOT (broken(int)) );

  connect(dccSocket,SIGNAL (readyRead()),this,SLOT (readData()) );
  connect(dccSocket,SIGNAL (readyWrite()),this,SLOT (sendAck()) );

  kdDebug() << "Lookup ..." << endl;
  dccSocket->startAsyncConnect();
}

// NOTE: Only for debugging purposes. Will be deleted eventually
void DccTransfer::check()
{
  kdDebug() << "Status:" << dccSocket->socketStatus() << endl;
}

void DccTransfer::lookupFinished(int numOfResults)
{
  kdDebug() << "Lookup finished. Connecting ..." << endl;
  setStatus(Connecting);
  numOfResults=0; // suppress compiler warning
}

void DccTransfer::connectionSuccess()
{
  kdDebug() << "Connected! Starting transfer ..." << endl;
  setStatus(Running);
  dccSocket->enableRead(true);
  file.open(IO_WriteOnly);
}

void DccTransfer::broken(int errNo)
{
  kdDebug() << "DccTransfer: Error " << errNo << endl;

  setStatus(Failed);
  file.close();
}

void DccTransfer::readData()
{
  int actual=dccSocket->readBlock(buffer,bufferSize);
  if(actual>0)
  {
    setPosition(getPosition()+actual);
    dccSocket->enableWrite(true);
    file.writeBlock(buffer,actual);
    updateCPS();
  }
}

unsigned long intel(unsigned long value)
{
  value=((value & 0xff000000) >> 24) +
        ((value & 0xff0000) >> 8) +
        ((value & 0xff00) << 8) +
        ((value & 0xff) << 24);

  return value;
}

void DccTransfer::sendAck()
{
  unsigned long pos=intel(getPosition());
  
  dccSocket->enableWrite(false);
  dccSocket->writeBlock((char*) &pos,(unsigned long) 4);

  if(getPosition()==getSize())
  {
    setStatus(Done);
    dccSocket->close();
    file.close();
  }
}

void DccTransfer::writeData()
{
}

void DccTransfer::setStatus(DccStatus status)
{
  dccStatus=status;
  setText(8,statusText[status]);
}

void DccTransfer::setSize(unsigned long size)
{
  fileSize=size;
  setText(2,QString::number(fileSize));
}

void DccTransfer::setPosition(unsigned long pos)
{
  transferred=pos;
  setText(3,QString::number(transferred));
  // some clients fail to send the file size
  if(fileSize)
    setText(4,QString::number((int) (transferred*100.0/fileSize))+"%");
  else
    setText(4,i18n("unknown"));
}

void DccTransfer::setPartner(QString partner)
{
  dccPartner=partner;
  setText(0,dccPartner);
}

QString DccTransfer::getPartner()
{
  return dccPartner;
}

void DccTransfer::setFile(QString file)
{
  dccFile=file;
  setText(1,dccFile);
}

QString DccTransfer::getFile()
{
  return dccFile;
}

void DccTransfer::updateCPS()
{
  int elapsed=transferStarted.secsTo(QDateTime::currentDateTime());
  // prevent division by zero
  int cps=(elapsed) ? transferred/elapsed : 0;
  setText(5,QString::number(cps));
}

DccTransfer::DccStatus DccTransfer::getStatus() { return dccStatus; }
unsigned long DccTransfer::getPosition() { return transferred; }
unsigned long DccTransfer::getSize() { return fileSize; }

void DccTransfer::setType(DccType type) { dccType=type; }
DccTransfer::DccType DccTransfer::getType() { return dccType; }

void DccTransfer::setIp(QString ip) { dccIp=ip; }
QString DccTransfer::getIp() { return dccIp; }

void DccTransfer::setPort(QString port) { dccPort=port; }
QString DccTransfer::getPort() { return dccPort; }

void DccTransfer::setBufferSize(unsigned long size) { bufferSize=size; }
unsigned long DccTransfer::getBufferSize() { return bufferSize; }

void DccTransfer::setFolder(QString folder) { dccFolder=folder; }
QString DccTransfer::getFolder() { return dccFolder; }
