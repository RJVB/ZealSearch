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
#include "zealsearch_config.h"
#include "zealsearchview.h"

#include <KTextEditor/Document>
#include <KTextEditor/View>

#include <KXMLGUIFactory>
#include <KPluginFactory>
#include <KPluginLoader>
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

ZealSearchPlugin *ZealSearchPlugin::plugin = 0;

K_PLUGIN_FACTORY_WITH_JSON (ZealSearchPluginFactory, "ktexteditor_zealsearch.json", registerPlugin<ZealSearchPlugin>();)

ZealSearchPlugin::ZealSearchPlugin(QObject *parent, const QVariantList &args)
: KTextEditor::Plugin(parent)
{
    Q_UNUSED(args);
    // parent->metaObject()->className() == "KatePluginManager" in kate
    // parent->metaObject()->className() == "KDevelop::Core" in KDevelop
    plugin = this;
    readConfig();
}

ZealSearchPlugin::~ZealSearchPlugin()
{
    plugin = 0;
}

QObject *ZealSearchPlugin::createView(KTextEditor::MainWindow *mainWindow)
{
    m_view = new ZealSearchView(this, mainWindow, zealCmd, docSets);
    return m_view;
}

void ZealSearchPlugin::setDocSetsStr(QString docSets_str)
{
    docSets.clear();
    QStringList docSets_list = docSets_str.split(QStringLiteral("\n"), QString::SkipEmptyParts);
    QStringListIterator dli(docSets_list);
    while (dli.hasNext()) {
        QString i = dli.next();
        QStringList p = i.split(QStringLiteral(":"));
        if(p.size() == 2){
            docSets[p.at(0)] = p.at(1);
        }
    }
}

QString ZealSearchPlugin::getDocSetsStr()
{
    QStringList docSets_list;
    QMapIterator<QString, QString> dsi(docSets);
    while (dsi.hasNext()) {
        dsi.next();
        docSets_list << dsi.key() + QStringLiteral(":") + dsi.value();
    }
    QString docSets_str = docSets_list.join(QStringLiteral("\n"));
    return docSets_str;
}

void ZealSearchPlugin::readConfig()
{
    KConfigGroup cg(KSharedConfig::openConfig(), QStringLiteral("ZealSearch Plugin"));
#ifdef Q_OS_MACOS
    zealCmd = cg.readEntry("zeal_command", "open -a zeal.app -W -n --args \"dash://%1\"");
#else
    zealCmd = cg.readEntry("zeal_command", "/usr/bin/zeal \"dash://%1\"");
#endif
    QString docSets_str = cg.readEntry("zeal_docsets",
        "php:html,joomla,php,wordpress\nhtml:html\ncss:css,less\njs:javascript,jquery\n");
    setDocSetsStr(docSets_str);
}

void ZealSearchPlugin::writeConfig()
{
    KConfigGroup cg(KSharedConfig::openConfig(), QStringLiteral("ZealSearch Plugin"));
    cg.writeEntry("zeal_command", zealCmd);
    cg.writeEntry("zeal_docsets", getDocSetsStr());
}

KTextEditor::ConfigPage *ZealSearchPlugin::configPage(int number, QWidget *parent)
{
    if (number != 0) {
        return Q_NULLPTR;
    } else {
        return new ZealSearchConfigPage(parent, this);
    }
}

#include "zealsearchplugin.moc"
