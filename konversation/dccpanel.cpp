/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  dccpanel.cpp  -  description
  begin:     Mit Aug 7 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com

  $Id$
*/

#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>

#include <klistview.h>
#include <kdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <krun.h>

#include "dccpanel.h"
#include "dcctransfer.h"

DccPanel::DccPanel(QWidget* parent) :
          ChatWindow(parent)
{
  setType(ChatWindow::DccPanel);

  dccListView=new KListView(this,"dcc_control_panel");

  dccListView->addColumn(i18n("Partner"));
  dccListView->addColumn(i18n("File"));
  dccListView->addColumn(i18n("Size"));
  dccListView->addColumn(i18n("Position"));
  dccListView->addColumn(i18n("% done"));
  dccListView->addColumn(i18n("CPS"));
  dccListView->addColumn(i18n("IP Address"));
  dccListView->addColumn(i18n("Port"));
  dccListView->addColumn(i18n("Status"));

  dccListView->setDragEnabled(true);
  dccListView->setAcceptDrops(true);
  dccListView->setSorting(-1,false);
  dccListView->setAllColumnsShowFocus(true);

  QHBox* buttonsBox=new QHBox(this);
  buttonsBox->setSpacing(spacing());
  acceptButton=new QPushButton(i18n("Accept"),buttonsBox,"start_dcc");
  abortButton =new QPushButton(i18n("Abort"),buttonsBox,"abort_dcc");
  removeButton=new QPushButton(i18n("Remove"),buttonsBox,"remove_dcc");
  openButton  =new QPushButton(i18n("Open"),buttonsBox,"open_dcc_file");
  infoButton  =new QPushButton(i18n("Information"),buttonsBox,"info_on_dcc_file");

  connect(dccListView,SIGNAL (selectionChanged()),this,SLOT (dccSelected()) );

  connect(acceptButton,SIGNAL (clicked()) ,this,SLOT (acceptDcc()) );
  connect(abortButton,SIGNAL (clicked()) ,this,SLOT (abortDcc()) );
  connect(removeButton,SIGNAL (clicked()) ,this,SLOT (removeDcc()) );
  connect(openButton,SIGNAL (clicked()) ,this,SLOT (runDcc()) );
}

DccPanel::~DccPanel()
{
}

void DccPanel::setButtons(bool accept,bool abort,bool remove,bool open,bool info)
{
  acceptButton->setEnabled(accept);
  abortButton->setEnabled(abort);
  removeButton->setEnabled(remove);
  openButton->setEnabled(open);
  infoButton->setEnabled(info);
}

void DccPanel::dccSelected()
{
  DccTransfer* item=static_cast<DccTransfer*>(getListView()->selectedItem());

  if(item)
  {
    DccTransfer::DccStatus status=item->getStatus();
    switch(status)
    {
      case DccTransfer::Queued:
      case DccTransfer::Lookup:
      case DccTransfer::Connecting:
        setButtons(true,true,true,false,false);
        break;
      case DccTransfer::Offering:
      case DccTransfer::Resuming:
        setButtons(false,true,true,true,true);
        break;
      case DccTransfer::Sending:
      case DccTransfer::Receiving:
      case DccTransfer::Stalled:
        setButtons(false,true,true,true,true);
        break;
      case DccTransfer::Aborted:
      case DccTransfer::Failed:
      case DccTransfer::Done:
        setButtons(false,false,true,true,true);
        break;
      default:
        setButtons(false,false,false,false,false);
    }
  }
  else setButtons(false,false,false,false,false);
}

void DccPanel::acceptDcc()
{
  DccTransfer* item=static_cast<DccTransfer*>(getListView()->selectedItem());
  if(item)
  {
    if(item->getType()==DccTransfer::Get && item->getStatus()==DccTransfer::Queued) item->startGet();
  }
}

void DccPanel::runDcc()
{
  DccTransfer* item=static_cast<DccTransfer*>(getListView()->selectedItem());
  if(item)
  {
    if(item->getType()==DccTransfer::Send || item->getType()==DccTransfer::ResumeSend)
      new KRun(item->getFile());
    else if(item->getType()==DccTransfer::Get || item->getType()==DccTransfer::ResumeGet)
      new KRun(item->getFullPath());
  }
}

void DccPanel::abortDcc()
{
  DccTransfer* item=static_cast<DccTransfer*>(getListView()->selectedItem());
  if(item) item->abort();
}

void DccPanel::removeDcc()
{
  DccTransfer* item=static_cast<DccTransfer*>(getListView()->selectedItem());

  if(item)
  {
    DccTransfer::DccStatus status=item->getStatus();
    bool doDelete=true;

    if(status!=DccTransfer::Queued &&
       status!=DccTransfer::Offering &&
       status!=DccTransfer::Aborted &&
       status!=DccTransfer::Failed &&
       status!=DccTransfer::Done)
    {
      // TODO: do some user question here
      doDelete=false;
    }

    if(doDelete)
    {
      delete item;
      // select next item so the user can clean up the list quickly
      item=static_cast<DccTransfer*>(getListView()->currentItem());
      if(item) getListView()->setSelected(item,true);
    }
  }
}

DccTransfer* DccPanel::getTransferByPort(QString port,DccTransfer::DccType type)
{
  int index=0;
  DccTransfer* item;
  do
  {
    // TODO: Get rid of this cast
    item=static_cast<DccTransfer*>(getListView()->itemAtIndex(index++));
    if(item)
    {
      if(item->getStatus()<DccTransfer::Failed &&
         item->getType()==type &&
         item->getPort()==port) return item;
    }
  } while(item);

  return 0;
}
// To find the resuming dcc over firewalls that change the port numbers
DccTransfer* DccPanel::getTransferByName(QString name,DccTransfer::DccType type)
{
  int index=0;
  DccTransfer* item;
  do
  {
    // TODO: Get rid of this cast
    item=static_cast<DccTransfer*>(getListView()->itemAtIndex(index++));
    if(item)
    {
      if(item->getStatus()<DccTransfer::Failed &&
         item->getType()==type &&
         item->getFile()==name) return item;
    }
  } while(item);

  return 0;
}

// virtual
void DccPanel::adjustFocus()
{
}

KListView* DccPanel::getListView() { return dccListView; }

#include "dccpanel.moc"
