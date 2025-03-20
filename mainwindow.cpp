#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "lexical-analysis.h"
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QFile>
#include <QTextStream>

QString fileContent = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("实验一 词法分析器");

    // 创建主布局
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建水平布局，将左侧标签、文件路径标签放置
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // 左侧标签显示 "20222131019 潘志健"
    QLabel* studentLabel = new QLabel("20222131019 潘志健", this);
    headerLayout->addWidget(studentLabel);  // 将左侧标签添加到布局中

    // 文件路径标签，右对齐
    QLabel* filePathLabel = new QLabel("选择的文件路径显示在这里", this);
    filePathLabel->setAlignment(Qt::AlignRight);  // 将文本右对齐
    headerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum)); // 占位符
    headerLayout->addWidget(filePathLabel);  // 将文件路径标签添加到右侧

    mainLayout->addLayout(headerLayout);  // 将头部布局添加到主布局

    // 创建按钮并调整其大小
    QPushButton* openFileButton = new QPushButton("选择文件", this);
    openFileButton->setFixedHeight(30);  // 设置按钮高度
    mainLayout->addWidget(openFileButton);

    // 创建解析按钮并调整其大小
    QPushButton* parseButton = new QPushButton("开始解析", this);
    parseButton->setFixedSize(100, 30);  // 设置按钮宽度和高度
    mainLayout->addWidget(parseButton);

    // 创建水平布局，包含左右两个文本框
    QHBoxLayout* textEditLayout = new QHBoxLayout();

    // 创建左侧文本框用于显示读取的内容
    QTextEdit* leftTextEdit = new QTextEdit(this);
    leftTextEdit->setReadOnly(true);  // 设置文本框为只读
    textEditLayout->addWidget(leftTextEdit);  // 将左侧文本框添加到布局中

    // 创建右侧文本框用于显示解析的内容
    QTextEdit* rightTextEdit = new QTextEdit(this);
    rightTextEdit->setReadOnly(true);  // 设置文本框为只读
    textEditLayout->addWidget(rightTextEdit);  // 将右侧文本框添加到布局中

    mainLayout->addLayout(textEditLayout);  // 将文本框布局添加到主布局中

    // 设置中心部件
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(openFileButton, &QPushButton::clicked, [this, filePathLabel, leftTextEdit]() {
        onOpenFileClicked(filePathLabel, leftTextEdit);
    });
    connect(parseButton, &QPushButton::clicked, [this, leftTextEdit, rightTextEdit]() {
        onParseContent(fileContent, rightTextEdit);
    });
}

// 选择文件并读取内容
void MainWindow::onOpenFileClicked(QLabel* filePathLabel, QTextEdit* leftTextEdit)
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件");

    if (filePath.isEmpty()) {
        qDebug() << "用户取消了文件选择";
        return;
    }

    qDebug() << "用户选择的文件：" << filePath;
    filePathLabel->setText(filePath);  // 设置文件路径到 QLabel

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        return;
    }

    QTextStream in(&file);
    fileContent = ""; // 清空文件内容
    while (!in.atEnd()) {
        fileContent += in.readLine() + "\n";  // 读取文件内容并拼接
    }

    qDebug() << fileContent;
    leftTextEdit->setText(fileContent);  // 在左侧文本框中显示文件内容
    file.close();
}

// 解析内容并显示解析结果
void MainWindow::onParseContent(const QString& content, QTextEdit* rightTextEdit)
{
    QString parsedContent = LEXICAL_ANALYSIS_H::parseSentense(content);
    qDebug() << parsedContent;

    // 在右侧文本框中显示解析后的内容
    rightTextEdit->setText(parsedContent);
}

MainWindow::~MainWindow()
{
    delete ui;
}
