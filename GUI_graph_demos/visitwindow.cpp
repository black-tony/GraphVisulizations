#include "visitwindow.h"
#include "queuevisulizer.h"
#include "stackvisulizer.h"
#include "mygraphvisulizer.h"
#include <QList>
#include <QMessageBox>
VisitWindow::VisitWindow(QWidget *parent)
    : QWidget{parent}
{
//    resize(1200, 1000);
    queueVisual = nullptr;
    stackVisual = nullptr;
    tmpLayout = nullptr;
    nextBtn = new QPushButton(this);
    connect(nextBtn, &QPushButton::clicked, this, &VisitWindow::onStartBtnClicked);
    graphVisual = nullptr;
    preChangedNode = -1;
    preVisNode = -1;
    previsEdge = -1;
}

VisitWindow::VisitWindow(AlgorithmChoose algType, const MyGraphList<int> &graph, QWidget *parent) : QWidget(parent),
    algType(algType)
{
    queueVisual = nullptr;
    stackVisual = nullptr;
    tmpLayout = nullptr;
    nextBtn = new QPushButton(this);
    connect(nextBtn, &QPushButton::clicked, this, &VisitWindow::onStartBtnClicked);
    graphVisual = nullptr;
    setGraph(graph);

}

void VisitWindow::setGraph(const MyGraphList<int> &newGraph)
{
    if(graphVisual != nullptr)
        delete graphVisual;
    preChangedNode = -1;
    preVisNode = -1;
    previsEdge = -1;
    graph = newGraph;
    vis.resize(graph.getNodeNum(), false);
    nodeVec.clear();
    nextBtn->setText("点击开始算法");

    if(algType == BFS)
    {
        if(queueVisual == nullptr)
            queueVisual = new QueueVisulizer(this);
        else
            queueVisual->clear();
    }
    else
    {
        if(stackVisual == nullptr)
            stackVisual = new StackVisulizer(this);
        else
            stackVisual->clear();
    }
    QList<QString> nodeInfo;
    for(int i = 0; i < graph.getNodeNum(); ++i)
        nodeInfo.append(QString::number(i));
    graphVisual = new MyGraphVisulizer(&graph, &nodeInfo, this);
    if(tmpLayout != nullptr)
        delete tmpLayout;
    tmpLayout = new QVBoxLayout(this);
    QLabel *hint = new QLabel(this);
    hint->setText("点击拖动节点可以调整节点位置");
    QLabel *myHint = new QLabel(this);
    myHint->setText("黑色: 原始状态, 蓝色: 已经访问节点, 绿色: 正在拓展节点, 红色: 当前处理节点");
    searchResult = new QLabel(this);
    searchResult->setText("遍历结果:");
    tmpLayout->addWidget(graphVisual);
    tmpLayout->addWidget(hint);
    tmpLayout->addWidget(nextBtn);
    tmpLayout->addWidget(myHint);
    tmpLayout->addWidget(searchResult);
    if(algType == BFS)
        tmpLayout->addWidget(queueVisual);
    else
        tmpLayout->addWidget(stackVisual);
    this->setLayout(tmpLayout);
}

void VisitWindow::setAlgType(AlgorithmChoose newAlgType)
{
    algType = newAlgType;
}

void VisitWindow::endAlg()
{
    QMessageBox infoBox(this);
    infoBox.setIcon(QMessageBox::Icon::Information);
    infoBox.setText("算法执行结束!");
    infoBox.setWindowTitle("算法执行提示");
    infoBox.exec();
    disconnect(nextBtn, &QPushButton::clicked, this, &VisitWindow::onNextBtnClicked);
    connect(nextBtn, &QPushButton::clicked, this, &VisitWindow::onStartBtnClicked);
    nextBtn->setText("点击开始算法");
//    preChangedNode = -1;
//    preVisNode = -1;
//    previsEdge = -1;
}

void VisitWindow::BFSOneStep()
{
    if(~preChangedNode)
        graphVisual->setNodeStateByIndex(preChangedNode, vis[preChangedNode] ? MyGraphVisulizer::NodeState::NODE_ROUTE :MyGraphVisulizer::NodeState::NODE_DEFAULT );
    if(~preVisNode)
    {
        qsizetype i = previsEdge + 1;
        if(i < graph[preVisNode].size())
        {
            int nextNode = graph[preVisNode][i].v;
            preChangedNode = nextNode;
            previsEdge = i;
            graphVisual->setNodeStateByIndex(nextNode, MyGraphVisulizer::NodeState::NODE_START);
            if(!vis[nextNode])
            {
                queueVisual->push(QString::number(nextNode));
                nodeVec.push_back(nextNode);
            }
            return;
        }
        else
        {
            preChangedNode = -1;
            graphVisual->setNodeStateByIndex(preVisNode, MyGraphVisulizer::NodeState::NODE_ROUTE);
            preVisNode = -1;
            previsEdge = -1;
            return;
        }
    }
    else if(nodeVec.size())
    {
        queueVisual->pop();
        int nowNode = nodeVec.front();
        nodeVec.pop_front();
        if(vis[nowNode])
            return;
        searchResult->setText(searchResult->text() + " ->" + QString::number(nowNode));
        graphVisual->setNodeStateByIndex(nowNode, MyGraphVisulizer::NodeState::NODE_END);
        preChangedNode = -1;
        preVisNode = nowNode;
        previsEdge = -1;
        vis[nowNode] = true;
        return;
    }
    else
    {
        for(int i = 0; i < graph.getNodeNum(); ++i)
        {
            if(!vis[i])
            {
                nodeVec.push_back(i);
                queueVisual->push(QString::number(i));
                graphVisual->setNodeStateByIndex(i, MyGraphVisulizer::NodeState::NODE_START);
                preChangedNode = i;
                previsEdge = -1;
                preVisNode = -1;
                break;
            }
        }
        if(nodeVec.empty())
            endAlg();
        return;
    }
}

void VisitWindow::recursiveDFSOneStep()
{
    if(~preChangedNode)
        graphVisual->setNodeStateByIndex(preChangedNode, vis[preChangedNode] ? MyGraphVisulizer::NodeState::NODE_ROUTE :MyGraphVisulizer::NodeState::NODE_DEFAULT );
    if(~preVisNode)
    {
        qsizetype i = previsEdge - 1;
        if(i >= 0)
        {
            int nextNode = graph[preVisNode][i].v;
            preChangedNode = nextNode;
            previsEdge = i;
            graphVisual->setNodeStateByIndex(nextNode, MyGraphVisulizer::NodeState::NODE_START);
            if(!vis[nextNode])
            {
                stackVisual->push(QString::number(nextNode));
                nodeVec.push_back(nextNode);
            }
            return;
        }
        else
        {
            preChangedNode = -1;
            graphVisual->setNodeStateByIndex(preVisNode, MyGraphVisulizer::NodeState::NODE_ROUTE);
            preVisNode = -1;
            previsEdge = -1;
            return;
        }
    }
    else if(nodeVec.size())
    {
        stackVisual->pop();
        int nowNode = nodeVec.last();
        nodeVec.pop_back();
        if(vis[nowNode])
            return;
        searchResult->setText(searchResult->text() + " ->" + QString::number(nowNode));
        graphVisual->setNodeStateByIndex(nowNode, MyGraphVisulizer::NodeState::NODE_END);
        preChangedNode = -1;
        preVisNode = nowNode;
        previsEdge = graph[nowNode].size();
        vis[nowNode] = true;
        return;
    }
    else
    {
        for(int i = 0; i < graph.getNodeNum(); ++i)
        {
            if(!vis[i])
            {
                nodeVec.push_back(i);
                stackVisual->push(QString::number(i));
                graphVisual->setNodeStateByIndex(i, MyGraphVisulizer::NodeState::NODE_START);
                preChangedNode = i;
                previsEdge = -1;
                preVisNode = -1;
                break;
            }
        }
        if(nodeVec.empty())
            endAlg();
        return;
    }
}

void VisitWindow::nonrecursiveDFSOneStep()
{
    if(~preChangedNode)
        graphVisual->setNodeStateByIndex(preChangedNode, vis[preChangedNode] ? MyGraphVisulizer::NodeState::NODE_ROUTE :MyGraphVisulizer::NodeState::NODE_DEFAULT );
    if(~preVisNode)
    {
        qsizetype i = previsEdge + 1;
        if(i < graph[preVisNode].size())
        {
            int nextNode = graph[preVisNode][i].v;
            preChangedNode = nextNode;
            previsEdge = i;
            graphVisual->setNodeStateByIndex(nextNode, MyGraphVisulizer::NodeState::NODE_START);
            if(!vis[nextNode])
            {
                stackVisual->push(QString::number(nextNode));
                nodeVec.push_back(nextNode);
            }
            return;
        }
        else
        {
            preChangedNode = -1;
            graphVisual->setNodeStateByIndex(preVisNode, MyGraphVisulizer::NodeState::NODE_ROUTE);
            preVisNode = -1;
            previsEdge = -1;
            return;
        }
    }
    else if(nodeVec.size())
    {
        stackVisual->pop();
        int nowNode = nodeVec.last();
        nodeVec.pop_back();
        if(vis[nowNode])
            return;
        searchResult->setText(searchResult->text() + " ->" + QString::number(nowNode));
        graphVisual->setNodeStateByIndex(nowNode, MyGraphVisulizer::NodeState::NODE_END);
        preChangedNode = -1;
        preVisNode = nowNode;
        previsEdge = -1;
        vis[nowNode] = true;
        return;
    }
    else
    {
        for(int i = 0; i < graph.getNodeNum(); ++i)
        {
            if(!vis[i])
            {
                nodeVec.push_back(i);
                stackVisual->push(QString::number(i));
                graphVisual->setNodeStateByIndex(i, MyGraphVisulizer::NodeState::NODE_START);
                preChangedNode = i;
                previsEdge = -1;
                preVisNode = -1;
                break;
            }
        }
        if(nodeVec.empty())
            endAlg();
        return;
    }
}

void VisitWindow::onNextBtnClicked()
{
    switch(algType)
    {
    case RECURSIVE_DFS:
        recursiveDFSOneStep();
        break;
    case NONRECURSIVE_DFS:
        nonrecursiveDFSOneStep();
        break;
    case BFS:
        BFSOneStep();
        break;
    default:
        break;
    }
}

void VisitWindow::onStartBtnClicked()
{
    preChangedNode = -1;
    preVisNode = -1;
    previsEdge = -1;
    graphVisual->resetGraph();
    vis.resize(vis.size(), false);
    disconnect(nextBtn, &QPushButton::clicked, this, &VisitWindow::onStartBtnClicked);
    connect(nextBtn, &QPushButton::clicked, this, &VisitWindow::onNextBtnClicked);
    nextBtn->setText("点击执行下一步");

}
