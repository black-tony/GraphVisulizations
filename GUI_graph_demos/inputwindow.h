#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include "mygraph.h"
class InputWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit InputWindow(QMainWindow *parent = nullptr);

signals:
    void getGraphLimit(int nodeNum);
    void getGraphInfo(const MyGraphMap<int> &g);
private slots:
    void outputText();
    void outputMap();
private:
    QWidget* inputWindow;
    QLineEdit *inputNodeNum, *inputInvalid;
    QPushButton* inputFinish;
    QVBoxLayout* mylayout;
    QList<QList<QLineEdit*>> inputMap;
    void showErrorMessage(const QString &message);
};

#endif // INPUTWINDOW_H
