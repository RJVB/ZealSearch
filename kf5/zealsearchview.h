#ifndef ZEALSEARCHVIEW_H
#define ZEALSEARCHVIEW_H

#include <QObject>
#include <KTextEditor/MainWindow>
#include <KXMLGUIClient>

class ZealSearchView : public QObject, public KXMLGUIClient
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
    private Q_SLOTS:
        void insertZealSearch();
    private:
        KTextEditor::MainWindow *m_mWin;
        QWidget *m_toolView;
        const QString& m_zealCmd;
        const QMap<QString, QString>& m_docSets;
};

#endif
