/*
 * Copyright (C) 2015  Mykola G <g@whatwhatweb.com>
 * Copyright (C) 2017  R.J.V. Bertin <rjvbertin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "zealsearchplugin.h"
#include "zealsearchview.h"

#include <KTextEditor/View>

#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KConfigGroup>
#include <KSharedConfig>
#include <klocalizedstring.h>

#include <QProcess>
#include <QString>
#include <QFileInfo>
#include <QMap>
#include <QMapIterator>
#include <QStringList>


ZealSearchView::ZealSearchView(KTextEditor::Plugin *plugin, KTextEditor::MainWindow *mainWin, const QString& zealCmd, const QMap<QString, QString>& docSets)
: QObject(mainWin)
, m_mWin(mainWin)
, m_plugin(plugin)
, m_zealCmd(zealCmd)
, m_docSets(docSets)
{
    KXMLGUIClient::setComponentName(QLatin1String("ktexteditor_zealsearch"), i18n("Zeal Search"));
    setXMLFile(QLatin1String("zealsearchui.rc"));

    KConfigGroup cg(KSharedConfig::openConfig(), "ZealSearch Plugin");

    QAction *action = actionCollection()->addAction(QLatin1String("tools_zealsearch"));
    action->setText(i18n("ZealSearch"));
    //action->setShortcut(Qt::CTRL + Qt::Key_XYZ);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(insertZealSearch()));

    m_mWin->guiFactory()->addClient(this);
}

ZealSearchView::~ZealSearchView()
{
    m_mWin->guiFactory()->removeClient(this);
}

void ZealSearchView::insertZealSearch()
{
    QFileInfo fi(activeView()->document()->documentName());
    QString docExt = fi.suffix();
    QString searchTerm = activeView()->selectionText();
    if(!m_docSets[docExt].isEmpty()) {
        searchTerm = m_docSets[docExt] + QStringLiteral(":") + searchTerm;
    }

    QProcess myProcess;
    myProcess.startDetached(m_zealCmd.arg(searchTerm));
}

// #include "zealsearchview.moc"
