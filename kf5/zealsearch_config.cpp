/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Mykola G <g@whatwhatweb.com>
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

#include "zealsearch_config.h"
#include "zealsearchplugin.h"

#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#include <QMessageBox>
#include <QLabel>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KConfigGroup>
#include <KSharedConfig>

ZealSearch_config::ZealSearch_config(QWidget *parent, const QVariantList &args)
  : KCModule(parent, args)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    zealCmd = new QLineEdit(this);
    zealCmd->setPlaceholderText(QStringLiteral("Zeal launch command"));
    zealCmd->setToolTip(QStringLiteral("Zeal launch command. %1 is replaced by text selected in editor."));
    layout->addWidget(zealCmd);
    docSets = new QTextEdit(this);
    docSets->setToolTip(QStringLiteral("Enter new line separated docset limitations for file extensions.\n"
        "More explanation here: http://zealdocs.org/usage.html"));
    layout->addWidget(docSets);
    setLayout(layout);
    load();
    connect(zealCmd, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
    connect(docSets, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

ZealSearch_config::~ZealSearch_config(){}

void ZealSearch_config::save()
{
    if(ZealSearchPlugin::self()){
        ZealSearchPlugin::self()->setZealCmd(zealCmd->text());
        ZealSearchPlugin::self()->setDocSetsStr(docSets->toPlainText());
        ZealSearchPlugin::self()->writeConfig();
    } else {
        KConfigGroup cg(KSharedConfig::openConfig(), "ZealSearch Plugin");
        cg.writeEntry("zeal_command", zealCmd->text()); 
    }
    emit changed(false);
}
 
void ZealSearch_config::load()
{
    if(ZealSearchPlugin::self()){
        ZealSearchPlugin::self()->readConfig();
        zealCmd->setText(ZealSearchPlugin::self()->getZealCmd());
        docSets->setPlainText(ZealSearchPlugin::self()->getDocSetsStr());
    }else{
        KConfigGroup cg(KSharedConfig::openConfig(), "ZealSearch Plugin");
        zealCmd->setText(cg.readEntry("zeal_command", "/usr/bin/zeal \"dash://%1\""));
    }
    emit changed(false);
}

void ZealSearch_config::defaults()
{
    zealCmd->setText(QStringLiteral("/usr/bin/zeal \"dash://%1\""));
    docSets->setPlainText(QStringLiteral("php:html,joomla,php,wordpress\nhtml:html\ncss:css,less\njs:javascript,jquery\n"));
    emit changed(true);
}
 
void ZealSearch_config::slotChanged()
{
    emit changed(true);
}
 
#include "zealsearch_config.moc"
 
