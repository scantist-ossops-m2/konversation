/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  konversationapplication.h  -  The main application
  begin:     Mon Jan 28 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com
*/

#ifndef KONVERSATIONAPPLICATION_H
#define KONVERSATIONAPPLICATION_H

#include <kapplication.h>

#include "preferences.h"
#include "ircevent.h"
#include "osd.h"
#include "konvdcop.h"
#include "identity.h"
#include "nickinfo.h"

class QCString;

class KonversationMainWindow;
class KonvDCOP;
class Server;
class PrefsDialog;
class QuickConnectDialog;
class Images;

namespace Konversation {
  class Sound;
  class NotificationHandler;
}

/*
  @author Dario Abatianni
*/

class KonversationApplication : public KApplication
{
  Q_OBJECT

  public:
    static Preferences preferences;

    /** This function in general shouldn't be called, because in the future there
     *  may be multiple windows.
     *  However, in some situations we have messageboxes that aren't targeted for
     *  any particular main window, such as general errors from dcop calls.
     *  
     *  Note to any MDI developer - get this to return any of the windows, or some
     *  'main' one.
     */
    KonversationMainWindow *getMainWindow();
    // URL-Catcher
    void storeUrl(const QString& who,const QString& url);
    const QStringList& getUrlList();

    // DCOP: Returns a list of signals we should emit
    QPtrList<IRCEvent> retrieveHooks(EVENT_TYPE type);

    KonversationApplication();
    ~KonversationApplication();
    
    static KonversationApplication* instance();
    
    void syncPrefs();
    Server* getServerByName(const QString& name);
    
    /** For dcop and addressbook, a user can be specified as user@irc.server.net
     *  or user@servergroup or using the unicode seperator symbol 0xE120 instead
     *  of the "@".  This function takes a string like the above examples, and
     *  modifies ircnick and serverOrGroup to contain the split up string.  If
     *  the string doesn't have an @ or 0xE120, ircnick is set to the
     *  nick_server, and serverOrGroup is set to empty.
     *  Behaviour is undefined for serverOrGroup if multiple @ or 0xE120 are found.
     *  @param nick_server A string containting ircnick and possibly servername or server group
     *  @param ircnick This is modified to contain the ircnick
     *  @param serverOrGroup This is modified to contain the servername, servergroup or an empty string.
     */
    static void splitNick_Server(QString nick_server, QString &ircnick, QString &serverOrGroup);
    
    /** Tries to find a nickinfo for a given ircnick on a given ircserver.
     *  @param ircnick The case-insensitive ircnick of the person you want to find.  e.g. "johnflux"
     *  @param serverOrGroup The case-insensitive server name (e.g. "irc.kde.org") or server group name (e.g. "freenode"), or null to search all servers
     *  @return A nickinfo for this user and server if one is found.
     */
    NickInfoPtr getNickInfo(const QString &ircnick, const QString &serverOrGroup);
    
    OSDWidget* osd;
    
    Konversation::Sound* sound();
    
    Images* images();

    /* Updates all nick icons according to new theme */
    void updateNickIcons();
    
    // Returns list of pointers to Servers.
    const QPtrList<Server> getServerList();
    
    Konversation::NotificationHandler* notificationHandler() const { return m_notificationHandler; }

    // Nick color stuff
    uint& getColorOffset();
    QStringList& getColorList();
    QMap<QString,QString>& getColorMap();

  signals:
    void catchUrl(const QString& who,const QString& url);
    void prefsChanged();

  public slots:
    void connectToServer(int number);
    void quickConnectToServer(const QString& hostName, 
			      const QString& port = "6667", 
			      const QString& channel="",
    			      const QString& nick = KonversationApplication::preferences.getNickname(0),
			      const QString& password="",
			      const bool& useSSL=FALSE
			      );
    void readOptions();
    void saveOptions(bool updateGUI=true);
    void quitKonversation();

    void closePrefsDialog();

    void deleteUrl(const QString& who,const QString& url);
    void clearUrlList();
    /** Send a message to all servers that you are away. */
    void toggleAway();
    bool emitDCOPSig(const QString& appId, const QString& objId, const QString& signal, QByteArray& data);

  protected slots:
    void openPrefsDialog();
    void openPrefsDialog(Preferences::Pages page);
    void openQuickConnectDialog();
    void removeServer(Server* server);

    void dcopMultiServerRaw(const QString &command);
    void dcopRaw(const QString& server, const QString &command);
    void dcopSay(const QString& server,const QString& target,const QString& command);
    void dcopInfo(const QString& string);
    void insertRememberLine();
    void appearanceChanged();
    void sendMultiServerCommand(const QString& command, const QString& parameter);
    void dcopConnectToServer(const QString& url, int port, const QString& channel, const QString& password);
    
  private:
    QPtrList<Server> serverList;
    QStringList urlList;
    PrefsDialog* prefsDialog;
    KonvDCOP* dcopObject;
    KonvPrefsDCOP* prefsDCOP;
    KonvIdentDCOP* identDCOP;
    KonversationMainWindow* mainWindow;
    Konversation::Sound* m_sound;
    QuickConnectDialog* quickConnectDialog;
    Images* m_images;
    
    Konversation::NotificationHandler* m_notificationHandler;

    uint colorOffSet;
    QStringList colorList;
    QMap<QString,QString> colorMap;
};

#endif
