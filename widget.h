#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <dnetwork.h>
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:
    void on_sendBtn_clicked();

    void on_createBtn_clicked();
    void newConnect(DSocket*);
    void getMsg(string);

    void on_joinBtn_clicked();

private:
    Ui::Widget *ui;
    DServer *server=0;
    DSocket *socket=0;
    vector<DSocket*> users;
};

#endif // WIDGET_H
