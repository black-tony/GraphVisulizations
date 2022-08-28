#include "inputwindow.h"
#include <QDebug>
InputWindow::InputWindow(QMainWindow *parent)
    : QMainWindow{parent}
{
    this->setWindowTitle("输入邻接矩阵");
    this->resize(QSize(400, 300));

    inputWindow = new QWidget(this);
    QObject* mds = inputWindow;
    setCentralWidget(inputWindow);


    inputNodeNum = new QLineEdit(inputWindow);
    inputNodeNum->setPlaceholderText("请输入1~999之间的数值");
    inputNodeNum->setValidator(new QIntValidator(1, 999, this));

    inputFinish = new QPushButton(inputWindow);
    inputFinish->setText("finish input");
    connect(inputFinish, &QPushButton::clicked, this, this->outputText);

    QLabel* widthHint = new QLabel(inputWindow);
    widthHint->setText("节点数");
    widthHint->setMinimumWidth(70);

//    QLabel* heightHint = new QLabel(inputWindow);
//    heightHint->setText("列数");
//    heightHint->setMinimumWidth(60);

    QHBoxLayout* widthContainer = new QHBoxLayout();
    widthContainer->addWidget(widthHint);
    widthContainer->addWidget(inputNodeNum);
    widthContainer->setStretch(0, 1);
    widthContainer->setStretch(1, 4);

//    QHBoxLayout* heightContainer = new QHBoxLayout();
//    heightContainer->addWidget(heightHint);
//    heightContainer->addWidget(inputHeight);
//    heightContainer->setStretch(0, 1);
//    heightContainer->setStretch(1, 4);

    mylayout = new QVBoxLayout(inputWindow);
    mylayout->addLayout(widthContainer);
//    mylayout->addLayout(heightContainer);
    mylayout->addWidget(inputFinish);

    inputWindow->setLayout(mylayout);
    mds->dumpObjectTree();
}

void InputWindow::outputText()
{
    if(inputNodeNum->text() == "")
        return showErrorMessage("请输入节点数!");
    int nodeNum = inputNodeNum->text().toInt();
//    int columnNum = inputHeight->text().toInt();
    if(nodeNum == 0)
        return showErrorMessage("节点数不能为0!");
    qDebug() << "节点数:" << nodeNum;
    inputMap.resize(nodeNum, QList<QLineEdit*>(nodeNum, nullptr));

    emit getGraphLimit(nodeNum);
    qDebug() << "finish button pressed!";
    inputFinish->setDisabled(true);
    disconnect(inputFinish, &QPushButton::clicked, this, this->outputText);

    inputNodeNum->setEnabled(false);
//    inputHeight->setEnabled(false);
    QLabel *invalidHint = new QLabel(inputWindow);
    invalidHint->setText("不存在值");
    invalidHint->setMinimumWidth(80);

    inputInvalid = new QLineEdit(inputWindow);
    inputInvalid->setValidator(new QIntValidator(this));
    inputInvalid->setPlaceholderText("请输入在邻接矩阵中表示边不存在的无穷大值");

    QHBoxLayout *invalidContainer = new QHBoxLayout();
    invalidContainer->addWidget(invalidHint);
    invalidContainer->addWidget(inputInvalid);
    invalidContainer->setStretch(0, 1);
    invalidContainer->setStretch(1, 4);

    mylayout->addLayout(invalidContainer);
    QGridLayout *mapgrid = new QGridLayout();
    for(int i = 0; i <= nodeNum; ++i)
    {
        for(int j = 0; j <= nodeNum; ++j)
        {
            if(i == 0 && j != 0)
            {
                QLabel * tmp = new QLabel(inputWindow);
                tmp->setText(QString::number((j)));
                mapgrid->addWidget(tmp, i, j);
            }
            else if(i != 0 && j == 0)
            {
                QLabel * tmp = new QLabel(inputWindow);
                tmp->setText(QString::number((i)));
                mapgrid->addWidget(tmp, i, j);
            }
            else if(i && j)
            {
                QLineEdit *tmp = new QLineEdit(inputWindow);
                inputMap[i - 1][j - 1] = tmp;
                tmp->setValidator(new QIntValidator(this));
                tmp->setPlaceholderText("∞");
                if(i == j)
                    tmp->setEnabled(false);
//                tmp->set
                mapgrid->addWidget(tmp, i, j);

            }
        }
    }
    mylayout->addLayout(mapgrid);
    connect(inputFinish, &QPushButton::clicked, this, this->outputMap);
    inputFinish->setDisabled(false);


}

void InputWindow::outputMap()
{
    int nodeNum = inputNodeNum->text().toInt();
//    int columnNum = inputHeight->text().toInt();
    MyGraphMap<int> mymap(nodeNum);
    if(inputInvalid->text() == "")
        return showErrorMessage("请输入表示不连边的值!");
    int invalidVal = inputInvalid->text().toInt();
    mymap.setInvalidEdge(invalidVal);
    for(int i = 0; i < nodeNum; ++i)
    {
        for(int j = 0; j < nodeNum; ++j)
        {
            QLineEdit *tmp = inputMap[i][j];
            if(tmp->text() == "")
                mymap[i][j] = invalidVal;
            else
                mymap[i][j] = tmp->text().toInt();

        }
    }
    emit getGraphInfo(mymap);
    this->close();
}

void InputWindow::showErrorMessage(const QString &message)
{
    QMessageBox infoBox(this);
    infoBox.setIcon(QMessageBox::Icon::Critical);
    infoBox.setText(message);
    infoBox.setWindowTitle("错误提示");
    infoBox.exec();
}
