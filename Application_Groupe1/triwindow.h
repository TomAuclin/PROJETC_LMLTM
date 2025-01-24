#ifndef TRIWINDOW_H
#define TRIWINDOW_H


#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
class triwindow : public QDialog
{
    Q_OBJECT

public:
    explicit triwindow(QWidget *parent = nullptr);

private:
    QPushButton *button1;
    QPushButton *button2;

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
};

#endif // TRIWINDOW_H
