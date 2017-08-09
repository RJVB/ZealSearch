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

#ifndef ZEALSEARCHPLUGIN_H
#define ZEALSEARCHPLUGIN_H

#include <KTextEditor/ConfigPage>
#include <KTextEditor/Plugin>
#include <KPluginFactory>

namespace KTextEditor
{
	class View;
}

class ZealSearchView;
class KConfig;

class ZealSearchPlugin
  : public KTextEditor::Plugin
{
  public:
    // Constructor
    explicit ZealSearchPlugin(QObject *parent = 0, const QVariantList &args = QVariantList());
    // Destructor
    virtual ~ZealSearchPlugin();
    
    static ZealSearchPlugin *self() { return plugin; }

//     void addView (KTextEditor::View *view);
//     void removeView (KTextEditor::View *view);
 
    QObject *createView(KTextEditor::MainWindow *mainWindow) override;

    int configPages() const override { return 1; };
    KTextEditor::ConfigPage *configPage(int number = 0, QWidget *parent = 0) override;
    void readConfig();
    void writeConfig();
 
    virtual void readConfig (KConfig *){};
    virtual void writeConfig (KConfig *){};
    
    QString getZealCmd(){ return zealCmd; }
    void setZealCmd(QString v) { zealCmd = v; }
    QMap<QString, QString> getDocSets(){ return docSets; }
    QString getDocSetsStr();
    void setDocSets(QMap<QString, QString> v) { docSets = v; }
    void setDocSetsStr(QString docSets_str);
  protected:
    QString zealCmd;
    QMap<QString, QString> docSets;
  private:
    static ZealSearchPlugin *plugin;
    ZealSearchView *m_view;
    QList<class ZealSearchView*> m_views;
  friend class ZealSearchView;
};

// K_PLUGIN_FACTORY_DECLARATION(ZealSearchPluginFactory)

#endif
