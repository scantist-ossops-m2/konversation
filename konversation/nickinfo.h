/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  nickinfo.h    -  Nick Information
  begin:     Sat Jan 17 2004
  copyright: (C) 2004 by Gary Cramblitt
  email:     garycramblitt@comcast.net
*/

#ifndef NICKINFO_H
#define NICKINFO_H

/*
  @author Gary Cramblitt
*/

#include <qstringlist.h>
#include <qdatetime.h>
#include <ksharedptr.h>

#include <kabc/addressbook.h>
#include "addressbook.h"

class Server;

/*
  The NickInfo object is a data container for information about a single nickname.
  It is owned by the Server object and should NOT be deleted by anything other than Server.
  If using code alters the NickInfo object, it should call Server::nickInfoUpdated to
  let Server know that the object has been modified.
*/

class NickInfo : public QObject, public KShared
{
  Q_OBJECT

  public:
    NickInfo(const QString& nick, Server* server);
    ~NickInfo();
     
    // Get properties of NickInfo object.
    QString getNickname() const;
    QString getHostmask() const;
    bool isAway();
    QString getAwayMessage();
    QString getIdentdInfo();
    QString getVersionInfo();
    bool isNotified();
    QString getRealName();
    QString getNetServer();
    QString getNetServerInfo();
    QDateTime getOnlineSince();
    QString tooltip();
    // Return the Server object that owns this NickInfo object.
    Server* getServer();
     
    // Return the kabc (kaddressbook) contact for this nick
    KABC::Addressee getAddressee();
    
    // Set properties of NickInfo object.
    // If any of these are called, call Server::nickInfoUpdated to let Server know about the change.
    void setNickname(const QString& newNickname);
    void setHostmask(const QString& newMask);
    void setAway(bool state);
    void setAwayMessage(const QString& newMessage);
    void setIdentdInfo(const QString& newIdentdInfo);
    void setVersionInfo(const QString& newVersionInfo);
    void setNotified(bool state);
    void setRealName(const QString& newRealName);
    void setNetServer(const QString& newNetServer);
    void setNetServerInfo(const QString& newNetServerInfo);
    void setOnlineSince(const QDateTime& datetime);
    void refreshAddressee();
    
  protected:
    QString nickname;
    Server* owningServer;
    QString hostmask;
    bool away;
    QString awayMessage;
    QString identdInfo;
    QString versionInfo;
    bool notified;
    QString realName;
    QString netServer;
    QString netServerInfo;
    QDateTime onlineSince;
    KABC::Addressee addressee;
  signals:
    void nickInfoChanged(void);
};

/** A NickInfoPtr is a pointer to a NickInfo object.  Since it is a KSharedPtr, the NickInfo
 * object is automatically destroyed when all references are destroyed.
 */
typedef KSharedPtr<NickInfo> NickInfoPtr;
/** A NickInfoMap is a list of NickInfo objects, indexed and sorted by lowercase nickname.
 */
typedef QMap<QString,NickInfoPtr> NickInfoMap;

#endif

