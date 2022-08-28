#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphlinkvisulizer.h"
#include "mygraphvisulizer.h"
#include "visitwindow.h"
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("算法实现题目2");

    inputWindow = nullptr;
    tmplayout= nullptr;
    visual= nullptr;
    showLinkList= nullptr, showRecursiveDFS= nullptr, showNonRecursiveDFS= nullptr, showBFS= nullptr;
}

MainWindow::~MainWindow()
{
    dumpObjectTree();
    delete ui;
}


void MainWindow::on_Start_btn_clicked()
{
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    qDebug() << "开始按钮被触摸!";
    if(inputWindow != NULL)
        delete inputWindow;
    inputWindow = new InputWindow(this);
    inputWindow->setWindowModality(Qt::WindowModal);
//    inputWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(inputWindow, &InputWindow::getGraphLimit, this, this->getGraphlimit);
    connect(inputWindow, &InputWindow::getGraphInfo, this, this->getGraphInfo);
    inputWindow->show();
    qDebug() << "开始按钮处理完成!";


}

void MainWindow::getGraphlimit(int nodeNum)
{
    qDebug() << "mainwindow received" << nodeNum;
//    this->ui->Start_btn->setHidden(true);
}

void MainWindow::getGraphInfo(const MyGraphMap<int> &g)
{
    usermap = MyGraphList(g);
    qDebug() << "n =" << usermap.getNodeNum();
    qDebug() << "inf =" << usermap.NOT_A_EDGE;
    ui->Start_btn->setText("重新输入邻接矩阵");
//    ui->Start_btn->setEnabled(true);

    if(showLinkList == nullptr)
    {
        showLinkList = new QPushButton(ui->centralwidget);
        showLinkList->setText("显示建立的邻接链表");
        connect(showLinkList, &QPushButton::clicked, this, &MainWindow::onShowLinkList);
    }
    if(showRecursiveDFS == nullptr)
    {
        showRecursiveDFS = new QPushButton(ui->centralwidget);
        showRecursiveDFS->setText("显示递归深度优先遍历");
        connect(showRecursiveDFS, &QPushButton::clicked, this, &MainWindow::onShowRecursiveDFS);

    }
    if(showNonRecursiveDFS == nullptr)
    {
        showNonRecursiveDFS = new QPushButton(ui->centralwidget);
        showNonRecursiveDFS->setText("显示非递归深度优先遍历");

        connect(showNonRecursiveDFS, &QPushButton::clicked, this, &MainWindow::onShowNonRecursiveDFS);

    }
    if(showBFS == nullptr)
    {
        showBFS = new QPushButton(ui->centralwidget);
        showBFS->setText("显示广度优先遍历");

        connect(showBFS, &QPushButton::clicked, this, &MainWindow::onShowBFS);
    }
    QList<QString> nodeNums;
    for(int i = 0; i < usermap.getNodeNum(); ++i)
        nodeNums.append(QString::number(i));
    if(visual == nullptr)
        visual = new MyGraphVisulizer(&usermap, &nodeNums, ui->centralwidget);
    else
        visual->setGraph(&usermap, &nodeNums);
    if(tmplayout == nullptr)
    {
        tmplayout = new QVBoxLayout(ui->centralwidget);
        QLabel *hint = new QLabel(ui->centralwidget);
        hint->setText("点击拖动节点可以调整节点位置");
        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addWidget(showLinkList);
        btnLayout->addWidget(showRecursiveDFS);
        btnLayout->addWidget(showNonRecursiveDFS);
        btnLayout->addWidget(showBFS);
        tmplayout->addWidget(ui->Start_btn);
        tmplayout->addLayout(btnLayout);
        tmplayout->addWidget(hint);
        tmplayout->addWidget(visual);
        ui->Start_btn->setGeometry(width() / 2 - 80 , 10 , 160, 30);
        visual->resize(800, 600);
        ui->centralwidget->setLayout(tmplayout);

    }


//    delete inputWindow;
    this->dumpObjectTree();
}

void MainWindow::onShowLinkList(bool checked)
{
    Q_UNUSED(checked);
    QWidget *newWindow = new QWidget();
    newWindow->setAttribute(Qt::WA_DeleteOnClose, true);
//    newWindow->resize();
    QVBoxLayout *newWindowLayout = new QVBoxLayout(newWindow);
    QLabel *tmpHint = new QLabel(newWindow);
    tmpHint->setText("链表中的信息格式:(边权, 指向节点)");
    newWindowLayout->addWidget(tmpHint);
    GraphLinkVisulizer *v = new GraphLinkVisulizer(newWindow);
    v->setMyGraph(&usermap);
    v->setGeometry(0, 0, 800, 600);
    newWindowLayout->addWidget(v);
    newWindow->setLayout(newWindowLayout);
    newWindow->setWindowTitle("图的邻接链表");
    newWindow->show();
    return;
}

void MainWindow::onShowRecursiveDFS(bool checked)
{
    Q_UNUSED(checked);
    VisitWindow *nowDFS = new VisitWindow(VisitWindow::AlgorithmChoose::RECURSIVE_DFS, usermap);
    nowDFS->setAttribute(Qt::WA_DeleteOnClose, true);
    nowDFS->setWindowTitle("递归深度优先搜索可视化");
    nowDFS->show();

}

void MainWindow::onShowNonRecursiveDFS(bool checked)
{
    Q_UNUSED(checked);
    VisitWindow *nowDFS = new VisitWindow(VisitWindow::AlgorithmChoose::NONRECURSIVE_DFS, usermap);
    nowDFS->setAttribute(Qt::WA_DeleteOnClose, true);
    nowDFS->setWindowTitle("非递归深度优先搜索可视化");
    nowDFS->show();

}

void MainWindow::onShowBFS(bool checked)
{
    Q_UNUSED(checked);
    VisitWindow *nowBFS = new VisitWindow(VisitWindow::AlgorithmChoose::BFS, usermap);
    nowBFS->setAttribute(Qt::WA_DeleteOnClose, true);
    nowBFS->setWindowTitle("广度优先搜索可视化");
    nowBFS->show();


}

