//
// C++ Implementation: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "prefspagenicklistbehavior.h"

#include <qheader.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>

#include <kiconloader.h>
#include <klineedit.h>
#include <klocale.h>

#include "preferences.h"
#include "valuelistviewitem.h"

PrefsPageNicklistBehavior::PrefsPageNicklistBehavior(QWidget* newParent, Preferences* newPreferences)
 : NicklistBehavior_Config(newParent)
{
  preferences = newPreferences;

  kcfg_DoubleClickCommand->setText(preferences->getChannelDoubleClickAction());
  kcfg_SortUserStatus->setChecked(preferences->getSortByStatus());
  kcfg_SortCaseInsensitive->setChecked(preferences->getSortCaseInsensitive());

  kcfg_SortOrder->header()->hide();
  kcfg_SortOrder->setSorting(-1);

  for(int index = 64; index != 0; index >>= 1)
  {
    if(preferences->getNoRightsValue() == index) new ValueListViewItem(0, kcfg_SortOrder, i18n("Normal Users"));
    if(preferences->getAwayValue() == index)     new ValueListViewItem(1, kcfg_SortOrder, i18n("Away Users"));
    if(preferences->getVoiceValue() == index)    new ValueListViewItem(2, kcfg_SortOrder, i18n("Voice (+v)"));
    if(preferences->getHalfopValue() == index)   new ValueListViewItem(3, kcfg_SortOrder, i18n("Halfops (+h)"));
    if(preferences->getOpValue() == index)       new ValueListViewItem(4, kcfg_SortOrder, i18n("Operators (+o)"));
    if(preferences->getOwnerValue() == index)    new ValueListViewItem(5, kcfg_SortOrder, i18n("Channel Owners"));
    if(preferences->getAdminValue() == index)    new ValueListViewItem(6, kcfg_SortOrder, i18n("Channel Admins"));
  }

  kcfg_UpButton->setIconSet(SmallIconSet("up"));
  kcfg_DownButton->setIconSet(SmallIconSet("down"));

  connect(kcfg_UpButton,SIGNAL (clicked()),this,SLOT (moveUp()) );
  connect(kcfg_DownButton,SIGNAL (clicked()),this,SLOT (moveDown()) );
}

void PrefsPageNicklistBehavior::applyPreferences()
{
  preferences->setChannelDoubleClickAction(kcfg_DoubleClickCommand->text());
  preferences->setSortByStatus(kcfg_SortUserStatus->isChecked());
  preferences->setSortCaseInsensitive(kcfg_SortCaseInsensitive->isChecked());

  int flag = 1;

  for(int index = 0; index < 7; index++)
  {
    ValueListViewItem* item = static_cast<ValueListViewItem*>(kcfg_SortOrder->itemAtIndex(index));
    int value = item->getValue();

    if(value == 0) preferences->setNoRightsValue(flag);
    else if(value == 1) preferences->setAwayValue(flag);
    else if(value == 2) preferences->setVoiceValue(flag);
    else if(value == 3) preferences->setHalfopValue(flag);
    else if(value == 4) preferences->setOpValue(flag);
    else if(value == 5) preferences->setOwnerValue(flag);
    else if(value == 6) preferences->setAdminValue(flag);

    flag <<= 1;
  }
}

void PrefsPageNicklistBehavior::moveUp()
{
  QListViewItem* item = kcfg_SortOrder->selectedItem();

  if(item)
  {
    int pos = kcfg_SortOrder->itemIndex(item);

    if(pos) {
      item->itemAbove()->moveItem(item);
    }
  }
}

void PrefsPageNicklistBehavior::moveDown()
{
  QListViewItem* item = kcfg_SortOrder->selectedItem();

  if(item && item != kcfg_SortOrder->lastItem()) {
    item->moveItem(item->itemBelow());
  }
}

#include "prefspagenicklistbehavior.moc"
