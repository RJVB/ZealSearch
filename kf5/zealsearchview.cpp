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
#include "zealsearch_debug.h"

#include <KTextEditor/View>
#include <KTextEditor/MainWindow>

#include <KXMLGUIFactory>
#include <KConfigGroup>
#include <KSharedConfig>
#include <klocalizedstring.h>
#include <KActionCollection>
#include <KStringHandler>

#include <QProcess>
#include <QString>
#include <QFileInfo>
#include <QMap>
#include <QMapIterator>
#include <QStringList>
#include <QMenu>

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
    action->setText(i18n("Lookup"));
    //action->setShortcut(Qt::CTRL + Qt::Key_XYZ);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(insertZealSearch()));

    m_ctxMenu = new KActionMenu(i18n("Zeal"), this);
    actionCollection()->addAction(QLatin1String("popup_zeal"), m_ctxMenu);
    m_lookup = m_ctxMenu->menu()->addAction(i18n("Lookup: %1",QString()), this, SLOT(insertZealSearch()));
    connect(m_ctxMenu->menu(), &QMenu::aboutToShow, this, &ZealSearchView::aboutToShow);

    if (m_mWin && m_mWin->guiFactory()) {
        m_mWin->guiFactory()->addClient(this);
    }
}

ZealSearchView::~ZealSearchView()
{
    if (m_mWin && m_mWin->guiFactory()) {
        m_mWin->guiFactory()->removeClient(this);
    }
}

QString ZealSearchView::currentWord() const
{
    const QFileInfo fi(activeView()->document()->documentName());
    const QString docExt = fi.suffix();
    QString searchTerm; /*= activeView()->selectionText()*/;
    KTextEditor::View *kv = activeView();
    if (!kv) {
        qCDebug(KTEZEAL) << "no KTextEditor::View" << endl;
        return QString();
    }

    if (kv->selection() && kv->selectionRange().onSingleLine()) {
        searchTerm = kv->selectionText();
    }

    if (searchTerm.isEmpty()) {
        if (!kv->cursorPosition().isValid()) {
            qCDebug(KTEZEAL) << "cursor not valid!" << endl;
            return QString();
        }

        const int line = kv->cursorPosition().line();
        const int col = kv->cursorPosition().column();
        const bool includeColon = true;

        searchTerm = kv->document()->line(line);

        int startPos = qMax(qMin(col, searchTerm.length()-1), 0);
        int endPos = startPos;
        while (startPos >= 0 && (searchTerm[startPos].isLetterOrNumber() ||
            (searchTerm[startPos] == QLatin1Char(':') && includeColon) ||
            searchTerm[startPos] == QLatin1Char('_') ||
            searchTerm[startPos] == QLatin1Char('~')))
        {
            startPos--;
        }
        while (endPos < (int)searchTerm.length() && (searchTerm[endPos].isLetterOrNumber() ||
            (searchTerm[endPos] == QLatin1Char(':') && includeColon) ||
            searchTerm[endPos] == QLatin1Char('_'))) {
            endPos++;
        }
        if  (startPos == endPos) {
            qCDebug(KTEZEAL) << "no word found!" << endl;
            return QString();
        }

        searchTerm = searchTerm.mid(startPos+1, endPos-startPos-1);

        while (searchTerm.endsWith(QLatin1Char(':'))) {
            searchTerm.remove(searchTerm.size()-1, 1);
        }

        while (searchTerm.startsWith(QLatin1Char(':'))) {
            searchTerm.remove(0, 1);
        }
    }

    if(!m_docSets[docExt].isEmpty()) {
        searchTerm = m_docSets[docExt] + QStringLiteral(":") + searchTerm;
    }
    return searchTerm;
}

void ZealSearchView::insertZealSearch()
{
    QString searchTerm = currentWord();

    if (!searchTerm.isEmpty()) {
        QProcess myProcess;
        myProcess.startDetached(m_zealCmd.arg(searchTerm));
    }
}

void ZealSearchView::aboutToShow()
{
    QString currWord = currentWord();
    if (currWord.isEmpty()) {
        return;
    }

    m_lookup->setText(i18n("Lookup: %1", KStringHandler::csqueeze(currWord, 30)));
}

// #include "zealsearchview.moc"
