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

    virtual void save() override;
    virtual void load() override;
    virtual void defaults() override;

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
    virtual ~ZealSearchConfigPage();

    virtual QString name() const override;
    virtual QString fullName() const override;
    virtual QIcon icon() const override;

    void apply() override;
    void reset() override;
    void defaults() override;

private Q_SLOTS:

private:
    ZealSearchPlugin* m_plugin;
    ZealSearch_config* m_config;
};

#endif // ZEALSEARCH_CONFIG_H
