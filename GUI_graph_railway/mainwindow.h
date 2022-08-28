#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MyGraphVisulizer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const static QString RESULT_DEFAULT_TEXT;
    Ui::MainWindow *ui;
    QLabel *dijResult, *hint;
    QPushButton *addNodeButton, *addEdgeButton, *calcButton, *delEdgeButton;
    QComboBox *startStation, *endStation;
    MyGraphVisulizer *graphArea;
    QString genDijAnsStr(const QList<int> &route, const QList<bool> &change, const QList<QString> &ndoeInfos) const;
private slots:
    void onNodeStrChanged();
    void onAddEdgeButtonClicked();
    void onAddNodeButtonClicked();
    void onCalcButtonClicked();
    void onDelEdgeButtonClicked();
};
#endif // MAINWINDOW_H
