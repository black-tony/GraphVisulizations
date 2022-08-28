#ifndef VISITWINDOW_H
#define VISITWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include "mygraph.h"
class QueueVisulizer;
class StackVisulizer;
class MyGraphVisulizer;
class VisitWindow : public QWidget
{
    Q_OBJECT
public:
    enum AlgorithmChoose{
        RECURSIVE_DFS,
        NONRECURSIVE_DFS,
        BFS
    };
    explicit VisitWindow(QWidget *parent = nullptr);
    VisitWindow(AlgorithmChoose algType, const MyGraphList<int> &graph, QWidget *parent = nullptr);
//    void setIsQueue(bool newIsQueue);

    void setGraph(const MyGraphList<int> &newGraph);

    void setAlgType(AlgorithmChoose newAlgType);

private:
    AlgorithmChoose algType;
    QList<int> nodeVec;
    QList<bool> vis;
    QBoxLayout *tmpLayout;
    MyGraphVisulizer *graphVisual;
    QPushButton *nextBtn;
    QueueVisulizer* queueVisual;
    StackVisulizer* stackVisual;
    QLabel *searchResult;
    MyGraphList<int> graph;
    int preVisNode, preChangedNode;
    qsizetype previsEdge;
    void endAlg();
    void BFSOneStep();
    void recursiveDFSOneStep();
    void nonrecursiveDFSOneStep();
signals:

private slots:
    void onNextBtnClicked();
    void onStartBtnClicked();
};

#endif // VISITWINDOW_H
