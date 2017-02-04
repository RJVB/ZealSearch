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

// K_PLUGIN_FACTORY_DEFINITION(ZealSearchPluginFactory, 
// 		 registerPlugin<ZealSearchPlugin>("ktexteditor_zealsearch");
// 		 registerPlugin<ZealSearch_config>("ktexteditor_zealsearch_config");
// 		)
K_PLUGIN_FACTORY_WITH_JSON (ZealSearchPluginFactory, "ktexteditor_zealsearch.json", registerPlugin<ZealSearchPlugin>();)
// K_EXPORT_PLUGIN(ZealSearchPluginFactory("ktexteditor_zealsearch", "ktexteditor_plugins"))

ZealSearchPlugin::ZealSearchPlugin(QObject *parent, const QVariantList &args)
: KTextEditor::Plugin(parent)
{
  Q_UNUSED(args);
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

// void ZealSearchPlugin::addView(KTextEditor::View *view)
// {
// 	ZealSearchView *nview = new ZealSearchView(view, zealCmd, docSets);
// 	m_views.append(nview);
// }
// 
// void ZealSearchPlugin::removeView(KTextEditor::View *view)
// {
// 	for(int z = 0; z < m_views.size(); z++)
// 	{
// 		if(m_views.at(z)->parentClient() == view)
// 		{
// 			ZealSearchView *nview = m_views.at(z);
// 			m_views.removeAll(nview);
// 			delete nview;
// 		}
// 	}
// }

void ZealSearchPlugin::setDocSetsStr(QString docSets_str)
{
  docSets.clear();
  QStringList docSets_list = docSets_str.split(QStringLiteral("\n"), QString::SkipEmptyParts);
  QStringListIterator dli(docSets_list);
  while (dli.hasNext()){
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
  zealCmd = cg.readEntry("zeal_command", "/usr/bin/zeal \"dash://%1\"");
  QString docSets_str = cg.readEntry("zeal_docsets", "php:html,joomla,php,wordpress\nhtml:html\ncss:css,less\njs:javascript,jquery\n");
  setDocSetsStr(docSets_str);
}

void ZealSearchPlugin::writeConfig()
{
  KConfigGroup cg(KSharedConfig::openConfig(), QStringLiteral("ZealSearch Plugin"));
  cg.writeEntry("zeal_command", zealCmd);
  cg.writeEntry("zeal_docsets", getDocSetsStr());
}

ZealSearchView::ZealSearchView(KTextEditor::Plugin *plugin, KTextEditor::MainWindow *mainWin, const QString& zealCmd, const QMap<QString, QString>& docSets)
: QObject(mainWin)
, m_mWin(mainWin)
, m_zealCmd(zealCmd)
, m_docSets(docSets)
{
    KXMLGUIClient::setComponentName(QLatin1String("ktexteditor_zealsearch"), i18n("Zeal Search"));
    setXMLFile(QLatin1String("zealsearchui.rc"));
//     setComponentData(ZealSearchPluginFactory::componentData());
    m_toolView = mainWin->createToolView(plugin, QLatin1String("kate_plugin_ktexteditor_zealsearch"),
                                        KTextEditor::MainWindow::Bottom,
                                        QIcon::fromTheme(QStringLiteral("zeal")),
                                        i18n("ZealSearch"));

    KConfigGroup cg(KSharedConfig::openConfig(), "ZealSearch Plugin");

    QAction *action = actionCollection()->addAction(QLatin1String("tools_zealsearch"));
    action->setText(i18n("ZealSearch"));
    //action->setShortcut(Qt::CTRL + Qt::Key_XYZ);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(insertZealSearch()));

//     QWidget *zealWidget = new QWidget(m_toolView);

    m_mWin->guiFactory()->addClient(this);
}

ZealSearchView::~ZealSearchView()
{
    m_mWin->guiFactory()->removeClient(this);

    delete m_toolView;
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

#include "zealsearchplugin.moc"
