#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeySequence>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QSharedPointer>
#include "FindDialog.h"
#include "ReplaceDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPlainTextEdit mainedit;
    QLabel statusLbl;
    QString m_filepath;
    bool m_isTextChange;
    QSharedPointer<FindDialog>m_pfindDialog;
    QSharedPointer<ReplaceDialog>m_pReplaceDlg;

    MainWindow();
    MainWindow(const MainWindow&);
    MainWindow& operator= (const MainWindow&);

    bool construct();
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initMainEditor();
    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);
    bool initSetMenu(QMenuBar* mb);
    bool initFileToolItem(QToolBar* tb);
    bool makeAction(QAction*& action,QWidget* parent, QString text, int key);
    bool makeAction(QAction*& action,QWidget* parent, QString tip, QString icon);

    //member function
    int showQueryMessage(QString Message);
    QString showFileDialog(QFileDialog::AcceptMode mode,QString title);
    void showErroeMessage(const QString message);
    QString saveCurrentData(QString path = "");
    void preEditorChange();
    void openFileToEditor(QString path);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);
protected:
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *e);  //悬挂事件
    void dropEvent(QDropEvent *e);    //放下事件
public:
    static MainWindow* NewInstance();
    QToolBar* toolbar();
    void openFile(QString path);
    ~MainWindow();
private slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onTextChanged();
    void onFileNew();
    void onFilePrint();
    void onCopyAvailable(bool available);
    void onRedoAvailable(bool available);
    void onUndoAvailable(bool available);
    void onCursorPositiongChanged();
    void onEditDelete();
    void onFileExit();
    void onEditFind();
    void onReplace();
    void onBackStyleSet();
    void onEditGoto();
    void onViewToolbar();
    void onViewStatusbar();
    void onHelpAbout();
    void onFormatFont();
    void onFormatWrap();
    void onHelpManual();
};

#endif // MAINWINDOW_H
