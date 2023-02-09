#ifndef UIL_LOGINWIDGET_H
#define UIL_LOGINWIDGET_H

#include <QWidget>


namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

public slots:
    void userLogin();

private:
    Ui::LoginWidget *ui;
};

#endif // UIL_LOGINWIDGET_H
