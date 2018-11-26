/*
 * Copyright (C) 2015  Mykola G <g@whatwhatweb.com>
 * Copyright (C) 2017-18  R.J.V. Bertin <rjvbertin@gmail.com>
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

#ifndef ZEALSEARCHVIEW_H
#define ZEALSEARCHVIEW_H

#include <QObject>
#include <QPointer>

#include <KTextEditor/MainWindow>
#include <KXMLGUIClient>
#include <KActionMenu>

class ZealSearchView : public QObject, public KXMLGUIClient
{
    Q_OBJECT
    public:
        explicit ZealSearchView(KTextEditor::Plugin *plugin, KTextEditor::MainWindow *mainWin, const QString& zealCmd, const QMap<QString, QString>& docSets);
        ~ZealSearchView();
        KTextEditor::View *activeView() const
        {
            if (m_mWin) {
                return m_mWin->activeView();
            } else {
                return nullptr;
            }
        }
        QString currentWord() const;

    private Q_SLOTS:
        void insertZealSearch();
        void aboutToShow();
        void configure();
    private:
        QPointer<KTextEditor::MainWindow> m_mWin;
        KTextEditor::Plugin* m_plugin;
        const QString& m_zealCmd;
        const QMap<QString, QString>& m_docSets;
        QPointer<KActionMenu> m_ctxMenu;
        QAction* m_lookup;
};

#endif
