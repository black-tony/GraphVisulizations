#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraph.h"
#include "inputwindow.h"
class MyGraphVisulizer;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Start_btn_clicked();
    void getGraphlimit(int NodeNum);
    void getGraphInfo(const MyGraphMap<int> &g);
    void onShowLinkList(bool checked);
    void onShowRecursiveDFS(bool checked);
    void onShowNonRecursiveDFS(bool checked);
    void onShowBFS(bool checked);


private:
    Ui::MainWindow *ui;
    MyGraphList<int> usermap;
    InputWindow* inputWindow;
    QVBoxLayout *tmplayout;
    MyGraphVisulizer *visual;
    QPushButton *showLinkList, *showRecursiveDFS, *showNonRecursiveDFS, *showBFS;
};
#endif // MAINWINDOW_H
