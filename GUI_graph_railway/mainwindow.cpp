#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mygraphvisulizer.h"
const QString MainWindow::RESULT_DEFAULT_TEXT = "路径计算结果在这里显示";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("综合应用题目2");

    resize(1000, 600);
    QVBoxLayout *baseLayout = new QVBoxLayout(ui->centralwidget);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QHBoxLayout *graphLayout = new QHBoxLayout();
    QVBoxLayout *comboLayout = new QVBoxLayout();

    dijResult = new QLabel(ui->centralwidget);
    dijResult->setText(RESULT_DEFAULT_TEXT);


    hint = new QLabel(ui->centralwidget);
    hint->setText("点击按钮添加或删除路径及地铁站, 不选中任何按钮时右键可以删除节点, 左键拖动节点可以调整节点位置\n绿色: 出发站; 红色: 结束站; 蓝色: 途径站; 洋红:中转站 ");

    QLabel *startStationHint = new QLabel(ui->centralwidget);
    startStationHint->setText("出发站点");
    QLabel *endStationHint = new QLabel(ui->centralwidget);
    endStationHint->setText("结束站点");

    addNodeButton = new QPushButton(ui->centralwidget);
    addNodeButton->setText("添加地铁站点");
    addNodeButton->setCheckable(true);
    addNodeButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");

    connect(addNodeButton, &QPushButton::clicked, this, &MainWindow::onAddNodeButtonClicked);

    addEdgeButton = new QPushButton(ui->centralwidget);
    addEdgeButton->setText("添加地铁线路");
    addEdgeButton->setCheckable(true);
    addEdgeButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");
    connect(addEdgeButton, &QPushButton::clicked, this, &MainWindow::onAddEdgeButtonClicked);

    calcButton = new QPushButton(ui->centralwidget);
    calcButton->setText("计算路径");
    connect(calcButton, &QPushButton::clicked, this, &MainWindow::onCalcButtonClicked);

    delEdgeButton = new QPushButton(ui->centralwidget);
    delEdgeButton->setText("删除地铁线路");
    delEdgeButton->setCheckable(true);
    delEdgeButton->setStyleSheet("QPushButton:checked{background-color:lightgreen;}");
    connect(delEdgeButton, &QPushButton::clicked, this, &MainWindow::onDelEdgeButtonClicked);

    startStation = new QComboBox(ui->centralwidget);

    endStation = new QComboBox(ui->centralwidget);

    graphArea = new MyGraphVisulizer(ui->centralwidget);
    connect(graphArea, &MyGraphVisulizer::nodeStrChanged, this, &MainWindow::onNodeStrChanged);

    btnLayout->addWidget(addNodeButton);
    btnLayout->addWidget(addEdgeButton);
    btnLayout->addWidget(delEdgeButton);
    btnLayout->addWidget(calcButton);

    comboLayout->addWidget(startStationHint);
    comboLayout->addWidget(startStation);
    comboLayout->addWidget(endStationHint);
    comboLayout->addWidget(endStation);


    graphLayout->addWidget(graphArea);
    graphLayout->addLayout(comboLayout);

    baseLayout->addLayout(graphLayout);
    baseLayout->addWidget(dijResult);
    baseLayout->addLayout(btnLayout);
    baseLayout->addWidget(hint);

    ui->centralwidget->setLayout(baseLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNodeStrChanged()
{
    startStation->clear();
    endStation->clear();
    for(const auto &i : qAsConst(graphArea->nodeInfos))
    {
        startStation->addItem(i);
        endStation->addItem(i);
    }
}

void MainWindow::onAddEdgeButtonClicked()
{
    graphArea->resetGraph();
    dijResult->setText(RESULT_DEFAULT_TEXT);

    addNodeButton->setChecked(false);
    delEdgeButton->setChecked(false);
    graphArea->cancelStartNode();
    if(addEdgeButton->isChecked())
        graphArea->setState(MyGraphVisulizer::ADD_EDGE);
    else
        graphArea->setState(MyGraphVisulizer::POINTER);
    qDebug() << "EdgeChecked" << addEdgeButton->isChecked();

}

void MainWindow::onAddNodeButtonClicked()
{
    graphArea->resetGraph();
    dijResult->setText(RESULT_DEFAULT_TEXT);

    addEdgeButton->setChecked(false);
    delEdgeButton->setChecked(false);

    graphArea->cancelStartNode();
    if(addNodeButton->isChecked())
        graphArea->setState(MyGraphVisulizer::ADD_NODE);
    else
        graphArea->setState(MyGraphVisulizer::POINTER);
    qDebug() << "NodeChecked" << addNodeButton->isChecked();
}
QString MainWindow::genDijAnsStr(const QList<int> &route, const QList<bool> &change, const QList<QString>& ndoeInfos) const
{
    QString tmp = "路径计算结果: ";
    if(route.size() != change.size())
        return "FAIL!!";
    bool isFirst = true;
    for(int i = 0; i < route.size(); ++i)
    {
        if(isFirst)
            isFirst = false;
        else
            tmp += " -> ";
        if(change[i])
            tmp += "[换乘]" + ndoeInfos[route[i]];
        else
            tmp += ndoeInfos[route[i]];
    }
    return tmp;
}
void MainWindow::onCalcButtonClicked()
{
    graphArea->resetGraph();
    dijResult->setText(RESULT_DEFAULT_TEXT);
    graphArea->cancelStartNode();

    addNodeButton->setChecked(false);
    addEdgeButton->setChecked(false);
    graphArea->cancelStartNode();
    graphArea->setState(MyGraphVisulizer::RUNNING_ALG);
    MyGraphList<RailwayEdgeInfo> tmpGraph;
    QList<QString> nodeNames;
    tmpGraph = graphArea->dumpGraphList(&nodeNames);
    if(tmpGraph.getNodeNum() <= 0)
    {
        QMessageBox::information(this, "运行结果", "图中没有任何节点!", QMessageBox::Ok);
        return;
    }
    QString startName = startStation->currentText();
    QString endName = endStation->currentText();
    int startId = std::find(nodeNames.begin(), nodeNames.end(), startName) - nodeNames.begin();
    int endId = std::find(nodeNames.begin(), nodeNames.end(), endName) - nodeNames.begin();
    QList<int> route;
    QList<bool> change;
    int ans = dijkstra(&tmpGraph, startId, endId, &route, &change);
    if(ans == -1)
    {
        QMessageBox::information(this, "运行结果", "选择的两点之间不连通!", QMessageBox::Ok);
        return;
    }
    if(ans == -2)
    {
        QMessageBox::information(this, "运行结果", "图中没有任何节点!", QMessageBox::Ok);
        return;
    }
//    qDebug() << "startId" << startId << "endId" << endId;
//    qDebug() << "ans=" << ans << "route:" << route;
    dijResult->setText(genDijAnsStr(route, change, nodeNames));

    graphArea->highlightRoute(&route, &change);

//    QMessageBox::information(this, "running", "test", QMessageBox::Ok);

    graphArea->setState(MyGraphVisulizer::POINTER);



}

void MainWindow::onDelEdgeButtonClicked()
{
    graphArea->resetGraph();
    dijResult->setText(RESULT_DEFAULT_TEXT);

    addNodeButton->setChecked(false);
    addEdgeButton->setChecked(false);
    graphArea->cancelStartNode();
    if(delEdgeButton->isChecked())
        graphArea->setState(MyGraphVisulizer::DEL_EDGE);
    else
        graphArea->setState(MyGraphVisulizer::POINTER);
}

