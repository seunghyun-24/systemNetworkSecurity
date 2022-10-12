#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void changeMoney(int diff);
    int money{0};
    void checkBuyAbility(int money);


private slots:
    void on_pb100_clicked();

    void on_pb500_clicked();

    void on_pb50_clicked();

    void on_pbCoffee_clicked();

    void on_pbTea_clicked();

    void on_pbReset_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
