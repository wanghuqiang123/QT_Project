
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
#include <QTabWidget>
#include <QVector>
#include "FindDialog.h"
#include "ReplaceDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    bool isClose;
    QTabWidget m_tabwidget;
    QPlainTextEdit mainedit;
    QVector<QPlainTextEdit *>v_mainedit;

    QLabel statusLbl;
    QString m_filepath;
    QVector<bool>m_isTextChange;
    QSharedPointer<FindDialog>m_pfindDialog;
    QSharedPointer<ReplaceDialog>m_pReplaceDlg;

    bool m_BackStyleSet;

    MainWindow();
    MainWindow(const MainWindow&);
    MainWindow& operator= (const MainWindow&);

    bool construct();
    bool initTabWidget();
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initMainEditor(QString filename);
    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);
    bool initSetMenu(QMenuBar* mb);
    bool initFileToolItem(QToolBar* tb);
    bool makeAction(QAction*& action,QWidget* parent, QString text, int key);
    bool makeAction(QAction*& action,QWidget* parent, QString tip, QString icon);

    void setSavename(int index,QString path);
    QString getFilename(QString path);
    //member function
    int showQueryMessage(QString Message);
    QString showFileDialog(QFileDialog::AcceptMode mode,QString title);
    void showErroeMessage(const QString message);
    QString saveCurrentData(QString path = "");
    void preEditorChange(int index);
    void openFileToEditor(QString path);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);
protected:
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *e);  //悬挂事件
    void dropEvent(QDropEvent *e);    //放下事件
    void DeleteVector();
    void openNewTab(QString filename = "new");
    void removeEditPoint(int index);   //删除对应的vector中的下标号
    void onBackStyleSetNew();
public:
    static MainWindow* NewInstance();
    QToolBar* toolbar();
    void openFile(QString path);
    ~MainWindow();
protected slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onTextChanged();
    void onFileNew();
    void onFilePrint();
    void onCloseTab(int index);
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

    //these slots functions is every m_tabwidget QPlainTextedit's slot ;
    void onEditUndo();
    void onEditRedo();
    void onEditCopy();
    void onEditCut();
    void onEditPaste();
    void onEditSelectAll();

    //预判槽函数


};

#endif // MAINWINDOW_H
