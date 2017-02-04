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

#ifndef ZEALSEARCH_CONFIG_H
#define ZEALSEARCH_CONFIG_H

#include <KCModule>
#include <KTextEditor/ConfigPage>
#include <qlineedit.h>

class QLineEdit;
class QTextEdit;

class ZealSearchPlugin;

class ZealSearch_config
  : public KCModule
{
  Q_OBJECT
  public:
    explicit ZealSearch_config(QWidget *parent = 0, const QVariantList &args = QVariantList());
    virtual ~ZealSearch_config();

    virtual void save();
    virtual void load();
    virtual void defaults();

  private Q_SLOTS:
    void slotChanged();

  private:
    QLineEdit* zealCmd;
    QTextEdit* docSets;
};

class ZealSearchConfigPage : public KTextEditor::ConfigPage {
    Q_OBJECT
public:
    explicit ZealSearchConfigPage(QWidget* parent = 0, ZealSearchPlugin *plugin = 0);
    ~ZealSearchConfigPage() {}

    virtual QString name() const;
    virtual QString fullName() const;
    virtual QIcon icon() const;

    void apply();
    void reset();
    void defaults() {}

private Q_SLOTS:

private:
    ZealSearchPlugin* m_plugin;
    ZealSearch_config* m_config;
};

#endif // ZEALSEARCH_CONFIG_H