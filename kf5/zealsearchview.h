#ifndef ZEALSEARCHVIEW_H
#define ZEALSEARCHVIEW_H

#include <QObject>
#include <KTextEditor/MainWindow>
#include <ktexteditor/sessionconfiginterface.h>
#include <KXMLGUIClient>

class ZealSearchView : public QObject, public KXMLGUIClient, public KTextEditor::SessionConfigInterface
{
    Q_OBJECT
    public:
        explicit ZealSearchView(KTextEditor::Plugin *plugin, KTextEditor::MainWindow *mainWin, const QString& zealCmd, const QMap<QString, QString>& docSets);
        ~ZealSearchView();
        KTextEditor::View *activeView()
        {
            if (m_mWin) {
                return m_mWin->activeView();
            } else {
                return NULL;
            }
        }
        // reimplemented: read and write session config
        void readSessionConfig (const KConfigGroup& config);
        void writeSessionConfig (KConfigGroup& config);
    private Q_SLOTS:
        void insertZealSearch();
    private:
        KTextEditor::MainWindow *m_mWin;
        QWidget *m_toolView;
        const QString& m_zealCmd;
        const QMap<QString, QString>& m_docSets;
};

#endif
