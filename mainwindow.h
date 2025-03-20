#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onButtonClicked();
    void onParseContent(const QString& content, QTextEdit* rightTextEdit);

private slots:
    void onOpenFileClicked(QLabel* label,QTextEdit* textEdit); // 选择文件并读取内容

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
