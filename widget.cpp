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
    ui->pbCoffee->setEnabled(money >= 100);
    ui->pbTea->setEnabled(money >= 150);
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


void Widget::on_pbReset_clicked() {
    QMessageBox msg;

    int Coin = this->money;
    int n100[1] = {0}, n50[1] = {0};

    QString refund;

    while(Coin >= 100) n100[0]++, Coin -= 100;
    while(Coin >= 50) n50[0]++, Coin -= 50;

    refund += "100 = " + QString::number(n100[0]) + ", 50 = " + QString::number(n50[0]);

    this->money = 0;
    ui->lcdNumber->display(money);

    msg.information(this, "Reset", refund);
}
