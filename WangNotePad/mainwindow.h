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
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPlainTextEdit mainedit;
    QLabel statusLbl;
    QString m_filepath;


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
    QString showFileDialog(QFileDialog::AcceptMode mode,QString title);
    void showErroeMessage(const QString message);
public:
    static MainWindow* NewInstance();
    ~MainWindow();
private slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
};

#endif // MAINWINDOW_H
