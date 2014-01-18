#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SendPost(QUrl url,QString host,QString param);

private slots:

    void on_timer_reply();

    void on_plainTextEdit_textChanged();

    void on_actionChange_nickname_triggered();

    void on_actionOnline_triggered();

    void on_select_online_item(const QModelIndex &index);

    void on_change_nickname();

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionExit_triggered();

    void on_actionEnter_triggered();

    void on_pushButton_clicked();

    void is_income_message();

    void on_textEdit_textChanged();

    void on_actionAbout_program_triggered();

    void on_actionAbout_authors_triggered();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
