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

#include "zealsearch_config.h"
#include "zealsearchplugin.h"

#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#include <QMessageBox>
#include <QLabel>
#include <QIcon>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KConfigGroup>
#include <KSharedConfig>
#include <klocalizedstring.h>

#include <QDebug>

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
        cg.sync();
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
#ifdef Q_OS_MACOS
        zealCmd->setText(QStringLiteral("open -a zeal.app -W -n --args \"dash://%1\""));
#else
        zealCmd->setText(cg.readEntry("zeal_command", "/usr/bin/zeal \"dash://%1\""));
#endif
    }
    emit changed(false);
}

void ZealSearch_config::defaults()
{
#ifdef Q_OS_MACOS
    // go through LaunchServices. Use -W -n (wait and always start a new instance) to
    // simulate regular system(2) or fork()+exec() behaviour. Without -n LaunchServices
    // refuses to send the argument if the application is already running.
    zealCmd->setText(QStringLiteral("open -a zeal.app -W -n --args \"dash://%1\""));
#else
    zealCmd->setText(QStringLiteral("/usr/bin/zeal \"dash://%1\""));
#endif
    docSets->setPlainText(QStringLiteral("php:html,joomla,php,wordpress\nhtml:html\ncss:css,less\njs:javascript,jquery\n"));
    emit changed(true);
}
 
void ZealSearch_config::slotChanged()
{
    emit changed(true);
}

ZealSearchConfigPage::ZealSearchConfigPage(QWidget* parent, ZealSearchPlugin* plugin)
    : KTextEditor::ConfigPage(parent)
    , m_plugin(plugin)
{
    m_config = new ZealSearch_config(this);
    // we get our layout from the KCM
    setLayout(m_config->layout());
}

ZealSearchConfigPage::~ZealSearchConfigPage()
{
    delete m_config;
}

void ZealSearchConfigPage::apply()
{
    m_config->save();
}

void ZealSearchConfigPage::reset()
{
    m_config->load();
}

void ZealSearchConfigPage::defaults()
{
    m_config->defaults();
}

QString ZealSearchConfigPage::name() const
{
    return i18n("ZealSearch");
}

QString ZealSearchConfigPage::fullName() const
{
    return i18n("ZealSearch Settings");
}

QIcon ZealSearchConfigPage::icon() const
{
    return QIcon::fromTheme(QStringLiteral("zeal"));
}


// #include "zealsearch_config.moc"
 
