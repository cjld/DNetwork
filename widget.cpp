#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_sendBtn_clicked() {
    if (socket) socket->write(ui->lineEdit->text().toStdString());
    else getMsg(ui->lineEdit->text().toStdString());
}

void Widget::on_createBtn_clicked()
{
    server=new DServer(this);
    server->listen("127.0.0.1",8389);
    connect(server,SIGNAL(newConnection(DSocket*)),this,SLOT(newConnect(DSocket*)));
}

void Widget::newConnect(DSocket *socket) {
    users.push_back(socket);
    connect(socket,SIGNAL(getMsg(string)),this,SLOT(getMsg(string)));
}

void Widget::on_joinBtn_clicked()
{
    socket=new DSocket();
    connect(socket,SIGNAL(getMsg(string)),this,SLOT(getMsg(string)));
    socket->connectToHost(ui->lineEdit->text().toStdString().c_str(),8389);
}

void Widget::getMsg(string s) {
    qDebug()<<s.size();
    ui->label->setText(ui->label->text()+'\n'+s.c_str());
    if (server)
        for (auto x : users)
            x->write(s);
}
