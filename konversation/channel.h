/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
    channel.h  -  description
    begin:     Wed Jan 23 2002
    copyright: (C) 2002 by Dario Abatianni
    email:     eisfuchs@tigress.com

    $Id$
*/

#include <qpushbutton.h>
#include <qwidget.h>
#include <qcheckbox.h>
#include <qvbox.h>
#include <qlabel.h>

#include <kdialog.h>
// #include <kcombobox.h>

#ifndef CHANNEL_H
#define CHANNEL_H

#include "nick.h"
#include "nicklistview.h"
#include "chatwindow.h"
#include "server.h"
#include "quickbutton.h"
#include "modebutton.h"
#include "ircinput.h"
#include "nickchangedialog.h"
#include "topiccombobox.h"

/*
  @author Dario Abatianni
*/

class Channel : public ChatWindow
{
  Q_OBJECT

  public:
    Channel(QWidget* parent);
    ~Channel();

    void setServer(Server* server);
    void setName(const QString& newName);
    void setKey(const QString& newKey);
    const QString& getKey();

    void joinNickname(QString& nickname,QString& hostname);
    void renameNick(QString& nickname,QString& newName);
    void addNickname(QString& nickname,QString& hostmask,bool op,bool voice);
    void removeNick(QString& nickname,QString& reason,bool quit);
    void kickNick(QString& nickname,QString& kicker,QString& reason);
    Nick* getNickByName(QString& lookname);
    QList<Nick> getNickList();

    void adjustNicks(int value);
    void adjustOps(int value);
    void updateNicksOps();

    void setTopic(QString& topic);
    void setTopic(QString& nickname,QString& topic); // Overloaded
    void updateMode(QString& nick,char mode,bool plus,QString& parameter);
    void updateModeWidgets(char mode,bool plus,QString& parameter);
    void updateQuickButtons(QStringList newButtonList);

  signals:
    void newText(QWidget* channel);
    void prefsChanged();

  public slots:
    void setNickname(const QString&);
    void channelTextEntered();
    void sendChannelText(const QString& line);
    void newTextInView();
    void urlCatcher(QString url);

  protected slots:
    void completeNick();
    void quickButtonClicked(QString definition);
    void modeButtonClicked(int id,bool on);
    // Will be connected to NickListView::popupCommand(int)
    void popupCommand(int id);
    // Dialogs
    void openNickChangeDialog();
    void changeNickname(QString newNickname);
    void closeNickChangeDialog(QSize newSize);
    // will be called when the user types a new topic in the topic line
    void requestNewTopic(const QString& newTopic);

  protected:
    QStringList* getSelectedNicksList();

    int spacing() {  return KDialog::spacingHint(); };
    int margin() {  return KDialog::marginHint(); };

    int nicks;
    int ops;

    unsigned int completionPosition;

    QString topic; // Caches current topic
    TopicComboBox* topicLine;
    QStringList topicHistory;

    QString key;

    ModeButton* modeT;
    ModeButton* modeN;
    ModeButton* modeS;
    ModeButton* modeI;
    ModeButton* modeP;
    ModeButton* modeM;
    ModeButton* modeK;
    ModeButton* modeL;

    QLineEdit* limit;

    QLabel* nicksOps;
    NickListView* nicknameListView;
    QPushButton* nicknameButton;
    IRCInput* channelInput;
    QCheckBox* logCheckBox;

    NickChangeDialog* nickChangeDialog;
    QList<Nick> nicknameList;
    QList<QuickButton> buttonList;
};

#endif
