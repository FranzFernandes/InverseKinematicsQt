#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void penColor();
    void penWidth();

private:
    void createActions();
    void createMenus();

    ScribbleArea *scribbleArea;

    QMenu *fileMenu;
    QMenu *optionMenu;

    QAction *openAct;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
//    QAction *clearScreenAct;
};

#endif
