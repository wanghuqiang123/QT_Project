#include "mainwindow.h"

void MainWindow::onEditUndo()
{
    v_mainedit[m_tabwidget.currentIndex()]->undo();
}

void MainWindow::onEditRedo()
{
    v_mainedit[m_tabwidget.currentIndex()]->redo();
}
void MainWindow::onEditCopy()
{
    v_mainedit[m_tabwidget.currentIndex()]->copy();
}
void MainWindow::onEditCut()
{
    v_mainedit[m_tabwidget.currentIndex()]->cut();
}
void MainWindow::onEditPaste()
{
    v_mainedit[m_tabwidget.currentIndex()]->paste();
}
void MainWindow::onEditSelectAll()
{
    v_mainedit[m_tabwidget.currentIndex()]->selectAll();
}

void MainWindow::removeEditPoint(int index)
{
    delete v_mainedit[index];
    v_mainedit.remove(index);
    m_isTextChange.remove(index);
}
