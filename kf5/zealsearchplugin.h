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
 
    QObject *createView(KTextEditor::MainWindow *mainWindow);

    int configPages() const { return 1; };
    KTextEditor::ConfigPage *configPage(int number = 0, QWidget *parent = 0);
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
  private:
    static ZealSearchPlugin *plugin;
    ZealSearchView *m_view;
    QList<class ZealSearchView*> m_views;
    QString zealCmd;
    QMap<QString, QString> docSets;
};

// K_PLUGIN_FACTORY_DECLARATION(ZealSearchPluginFactory)

#endif
