/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  serverwindow.h  -  description
  begin:     Sun Jan 20 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com

  $Id$
*/

/* QT specific includes */
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qfile.h>

/* KDE specific includes */
#include <kmainwindow.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kstatusbar.h>

/*
  This is the Server Window Class. It shows a window
  with tab folders to store the channels and queries in.

  @author Dario Abatianni
*/

#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include "server.h"
#include "ircview.h"
#include "ircinput.h"
#include "highlightdialog.h"
#include "quickbuttonsdialog.h"
#include "notifydialog.h"
#include "ignoredialog.h"
#include "notifydialog.h"
#include "ledtabwidget.h"
#include "colorconfiguration.h"
#include "chatwindow.h"
#include "nicksonline.h"
#include "dccpanel.h"

class ServerWindow : public KMainWindow
{
  Q_OBJECT

  public:
    ServerWindow(Server* server);
    ~ServerWindow();

    void appendToStatus(const QString& type,const QString& message);
    void appendToFrontmost(const QString& type,const QString& message);
    void setServer(Server* server);
    Server* getServer();
    void setLog(bool activated);
    LedTabWidget* getWindowContainer();
    void addView(QWidget* pane,int color,const QString& name);
    void showView(QWidget* pane);
    DccPanel* getDccPanel();

  signals:
    void prefsChanged();
    void openPrefsDialog();

  public slots:
    void setNickname(const QString&);
    void newText(QWidget* view);
    void changedView(QWidget* view);
    void logText(const QString& text);
    void channelPrefsChanged();
    void resetLag();
    void updateLag(int msec);
    void tooLongLag(int msec);

  protected slots:
    void statusTextEntered();
    void addStatusView();
    void addDccPanel();
    void closeDccPanel();      // remove dcc panel from view, but does not delete it
    void deleteDccPanel();     // deletes dcc panel from memory
    void showToolbar();
    void showStatusbar();
    void openPreferences();
    void quitProgram();

    void openHilight();
    void applyHilight(QPtrList<Highlight> newList);
    void closeHilight(QSize newSize);

    void openIgnore();
    void applyIgnore(QPtrList<Ignore> newList);
    void closeIgnore(QSize newSize);

    void openNotify();
    void applyNotify(QStringList newList,bool use,int delay);
    void closeNotify(QSize newSize);

    void openNicksOnlineWindow();
    void closeNicksOnlineWindow(QSize newSize);

    void openButtons();
    void applyButtons(QStringList newList);
    void closeButtons(QSize newSize);

    void openColorConfiguration();
    void applyColorConfiguration(QString actionTextColor, QString backlogTextColor, QString channelTextColor,
                                 QString commandTextColor, QString linkTextColor, QString queryTextColor,
                                 QString serverTextColor);
    void closeColorConfiguration(QSize windowSize);

  protected:
    int spacing();
    int margin();

    void setLogfileName(const QString& name);

    void readOptions();
    void saveOptions();
    bool queryExit();

    enum StatusID
    {
      StatusText,LagOMeter
    };

    OutputFilter filter;
    LedTabWidget* windowContainer;
    QVBox* statusPane;
    IRCView* statusView;
    ChatWindow* frontView;

    DccPanel* dccPanel; // the adress of the dcc panel
    bool dccPanelOpen;  // to track if a dcc panel is already open

    QPushButton* nicknameButton;
    IRCInput* statusInput;
    QCheckBox* logCheckBox;

    Server* server;
    KToggleAction* showToolBarAction;
    KToggleAction* showStatusBarAction;
    HighlightDialog* hilightDialog;
    NotifyDialog* notifyDialog;
    IgnoreDialog* ignoreDialog;
    QuickButtonsDialog* buttonsDialog;
    ColorConfiguration* colorConfigurationDialog;
    NicksOnline* nicksOnlineWindow;

    QFile logfile;
    bool log;
    bool firstLog;
};

#endif
