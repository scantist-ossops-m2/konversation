/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  preferences.h  -  description
  begin:     Tue Feb 5 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com

  $Id$
*/

#include <qobject.h>
#include <qlist.h>
#include <qsize.h>
#include <qstringlist.h>

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "serverentry.h"
#include "ignore.h"
#include "highlight.h"

/*
  @author Dario Abatianni
*/

/*
Options still to be GUIfied:

AutoReconnect (bool)
AutoRejoin (bool)
BlinkingTabs (bool)
FixedMOTD (bool)

Operator LEDs (int)
OperatorColor (int)
VoiceColor (int)
NoRightsColor (int)
*/

class Preferences : public QObject
{
  Q_OBJECT

  public:
    Preferences();
    ~Preferences();

    int serverWindowToolBarPos;
    int serverWindowToolBarStatus;
    int serverWindowToolBarIconText;
    int serverWindowToolBarIconSize;
    bool serverWindowStatusBarStatus;

    int addServer(const QString& serverString);
    void removeServer(int id);
    QString getServerByIndex(unsigned int);
    QString getServerById(int);
    ServerEntry* getServerEntryById(int id);
    int getServerIdByIndex(unsigned int);

    bool getAutoReconnect();
    bool getAutoRejoin();
    void setAutoReconnect(bool on);
    void setAutoRejoin(bool on);

    void clearServerList();
    void changeServerProperty(int id,int property,const QString& value);
    void updateServer(int id,const QString& newDefinition);

    void setLog(bool state);
    bool getLog();
    void setLowerLog(bool state);
    bool getLowerLog();
    void setLogFollowsNick(bool state);
    bool getLogFollowsNick();
    void setLogPath(QString path);
    QString getLogPath();

    void setDccAddPartner(bool state);
    bool getDccAddPartner();
    void setDccCreateFolder(bool state);
    bool getDccCreateFolder();
    void setDccAutoGet(bool state);
    bool getDccAutoGet();
    void setDccBufferSize(unsigned long size);
    unsigned long getDccBufferSize();
    void setDccPath(QString path);
    QString getDccPath();
    void setDccRollback(unsigned long bytes);
    unsigned long getDccRollback();

    void setBlinkingTabs(bool blink);
    bool getBlinkingTabs();
    void setBringToFront(bool state);
    bool getBringToFront();

    // Geometry functions
    QSize getServerWindowSize();
    QSize& getHilightSize();
    QSize& getButtonsSize();
    QSize& getIgnoreSize();
    QSize& getNotifySize();
    QSize& getNicksOnlineSize();
    QSize& getNicknameSize();
    QSize& getColorConfigurationSize();
    void setServerWindowSize(QSize newSize);
    void setHilightSize(QSize newSize);
    void setButtonsSize(QSize newSize);
    void setIgnoreSize(QSize newSize);
    void setNotifySize(QSize newSize);
    void setNicksOnlineSize(QSize newSize);
    void setNicknameSize(QSize newSize);
    void setColorConfigurationSize(QSize newSize);

    int getNotifyDelay();
    void setNotifyDelay(int delay);
    bool getUseNotify();
    void setUseNotify(bool use);
    QStringList getNotifyList();
    QString getNotifyString();
    void setNotifyList(QStringList newList);

    QPtrList<Highlight> getHilightList();
    void setHilightList(QPtrList<Highlight> newList);
    void addHilight(QString newHilight,QColor color);

    void setHilightNickColor(QString color);
    QColor getHilightNickColor();

    void setHilightNick(bool state);    // shall we hilight the current nick?
    bool getHilightNick();

    QStringList getButtonList();
    void setButtonList(QStringList newList);

    void addIgnore(QString newIgnore);
    void clearIgnoreList();
    QPtrList<Ignore> getIgnoreList();
    void setIgnoreList(QPtrList<Ignore> newList);

    QString getPartReason();
    void setPartReason(QString newReason);

    QString getKickReason();
    void setKickReason(QString newReason);

    QString getNickname(int index);
    QStringList getNicknameList();
    void setNickname(int index,QString newName);
    void setNicknameList(QStringList newList);

    QString channelMessageColor, queryMessageColor, serverMessageColor, actionMessageColor,
            backlogMessageColor, linkMessageColor, commandMessageColor;

    QString defaultChannelMessageColor, defaultQueryMessageColor, defaultServerMessageColor,
            defaultActionMessageColor, defaultBacklogMessageColor, defaultLinkMessageColor,
            defaultCommandMessageColor;

    QString getChannelMessageColor();
    QString getQueryMessageColor();
    QString getServerMessageColor();
    QString getActionMessageColor();
    QString getBacklogMessageColor();
    QString getLinkMessageColor();
    QString getCommandMessageColor();

    void setChannelMessageColor(QString color);
    void setQueryMessageColor(QString color);
    void setServerMessageColor(QString color);
    void setActionMessageColor(QString color);
    void setBacklogMessageColor(QString color);
    void setLinkMessageColor(QString color);
    void setCommandMessageColor(QString color);

    void setNickCompleteSuffixStart(QString suffix);
    void setNickCompleteSuffixMiddle(QString suffix);
    QString getNickCompleteSuffixStart();
    QString getNickCompleteSuffixMiddle();

    int getOpLedColor();
    int getVoiceLedColor();
    int getNoRightsLedColor();
    void setOpLedColor(int color);
    void setVoiceLedColor(int color);
    void setNoRightsLedColor(int color);

    bool getFixedMOTD();
    void setFixedMOTD(bool fixed);

    void setCommandChar(QString newCommandChar);
    QString getCommandChar();

    QString ident;
    QString realname;

  signals:
    void requestServerConnection(int number);
    void requestSaveOptions();

  protected:
    bool log;
    bool lowerLog;
    bool logFollowsNick;
    QString logPath;

    bool blinkingTabs;      // Do we want the LEDs on the tabs to blink?
    bool bringToFront;      // Do we want to see newly created tabs immediately?

    bool fixedMOTD;

    bool dccAddPartner;
    bool dccCreateFolder;   // create folders for each DCC partner?
    bool dccAutoGet;
    unsigned long dccBufferSize;
    unsigned long dccRollback;  // Rollback for Resume
    QString dccPath;

    bool autoReconnect;
    bool autoRejoin;

    int notifyDelay;
    int opLedColor;
    int voiceLedColor;
    int noRightsLedColor;
    bool useNotify;

    QStringList notifyList;
    QString commandChar;

    QString nickCompleteSuffixStart;
    QString nickCompleteSuffixMiddle;

    // Geometries
    QSize serverWindowSize;
    QSize buttonsSize;
    QSize hilightSize;
    QSize ignoreSize;
    QSize notifySize;
    QSize nicksOnlineSize;
    QSize nicknameSize;
    QSize colorConfigurationSize;

    QList<ServerEntry> serverList;
    QPtrList<Highlight> hilightList;

    bool hilightNick;
    QColor hilightNickColor;

    QStringList buttonList;

    QPtrList<Ignore> ignoreList;

    QStringList nicknameList;

    QString partReason;
    QString kickReason;
};

#endif
