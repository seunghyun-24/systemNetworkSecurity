#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pbCoffee->setEnabled(false); // delete this and make some function that activate the button if the money > beverage
    ui->pbTea->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::checkBuyAbility(int money){
    if(money >= 100) ui->pbCoffee->setEnabled(true);
    else ui->pbCoffee->setEnabled(false);

    if(money >= 150) ui->pbTea->setEnabled(true);
    else ui->pbTea->setEnabled(false);
}

void Widget::changeMoney(int diff){
    this->money+=diff;
    ui->lcdNumber->display(money);

    checkBuyAbility(this->money);
}

void Widget::on_pb100_clicked()
{
    changeMoney(100);
}

void Widget::on_pb500_clicked()
{
    changeMoney(500);
}

void Widget::on_pb50_clicked()
{
    changeMoney(50);
}

void Widget::on_pbCoffee_clicked()
{
    changeMoney(-100);
}

void Widget::on_pbTea_clicked()
{
    changeMoney(-150);
}

void Widget::on_pbReset_clicked()
{
    QMessageBox msg;
    
    int change = this->money;
    char value[2];

    change = change/100;

    sprintf(&value[0], "%d", change);

    int change2 = this->money - change*100;
    change2 = change2/10;
    sprintf(&value[1], "%d", change2);

    this->money = 0;
    ui->lcdNumber->display(money);

    msg.information(this, "Reset: return 100", &value[0]);
    msg.information(this, "Reset: return 10", &value[1]); // money (calculate)
}
