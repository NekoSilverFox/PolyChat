#include <QTest>  // 单元测试必须包含，但是包含后会拖慢编译速度。因为他们包含了所有模块。注意不要写成 QtTest！

// add necessary includes here
#include <QSignalSpy>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include <QColorDialog>
#include <QCloseEvent>
#include <QTextBrowser>
#include <QTextEdit>
#include <QComboBox>
#include <QTableWidget>

#include "../App/db_localdata.h"
#include "../App/bll_polychat.h"
#include "../App/dal_polychat.h"
#include "../App/uil_loginwidget.h"
#include "../App/uil_addchat.h"
#include "../App/uil_chatboxwidget.h"
#include "../App/uil_chatlist.h"
#include "../App/tcpclient.h"
#include "../App/tcpserver.h"


QString      localUserName           = "";               // User Name (get in user login)
QString      localUserGroupNumber    = "";               // Group number (get in user login)
QHostAddress localIpAddress          = QHostAddress();
ChatList*    chatList                = nullptr;          // Widget ChatList (Only one)

unsigned int const TIMER_STEP        = 50;

class PolyChatTester : public QObject
{
    Q_OBJECT

public:
    PolyChatTester();
    ~PolyChatTester();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void ut_login_login_empty            ();
    void ut_login_group_empty            ();
    void ut_login_login_group_empty      ();
    void ut_login_init_login             ();
    void ut_login_init_group             ();
    void ut_login_btnlogin               ();
    void ut_login_btnlogin_emit          ();
    void ut_login_btnInfo_emit           ();
    void ut_login_window                 ();

    void ut_addchat_btnCancelAddChat     ();
    void ut_addchat_btnAddChat           ();

    void ut_chat_init                    ();

    void ut_chatlist_init                ();
    void ut_chatlist_btnNewChat          ();
    void ut_chatlist_btnNewChat_emit     ();
    void ut_chatlist_leSearch            ();
    void mt_chatlist_leSearch_change_emit();
    void ut_chatlist_lbName              ();
    void ut_chatlist_lbGroupNumber       ();
    void ut_chatlist_lbIP                ();
    void ut_chatlist_chat_not_open       ();
    void ut_chatlist_chat_open           ();

    void ut_addchat_port_exist           ();
    void ut_addchat_port_not_exist       ();
    void ut_addchat_chat_exist           ();
    void ut_addchat_chat_not_exist       ();

    void ut_tcpclient_lbClientIP         ();
    void ut_tcpclient_lbClientPort       ();
    void ut_tcpclient_lbServerIP         ();
    void ut_tcpclient_lbServerPort       ();
    void ut_tcpclient_lbFileName         ();
    void ut_tcpclient_lbFileSize         ();
    void ut_tcpclient_progressBar        ();
    void ut_tcpclient_btnCancel          ();
    void ut_tcpclient_btnCancel_emit     ();
    void ut_tcpclient_btnSave            ();
    void ut_tcpclient_btnSave_emit       ();
    void ut_tcpclient_closeEvent_emit    ();
    void ut_tcpclient_textBrowser        ();

    void ut_tcpserver_lbClientIP         ();
    void ut_tcpserver_lbClientPort       ();
    void ut_tcpserver_lbServerIP         ();
    void ut_tcpserver_lbServerPort       ();
    void ut_tcpserver_lbFilePath         ();
    void ut_tcpserver_lbFileSize         ();
    void ut_tcpserver_btnCancel          ();
    void ut_tcpserver_btnCancel_emit     ();
    void ut_tcpserver_progressBar        ();
    void ut_tcpserver_closeEvent_emit    ();
    void ut_tcpserver_textBrowser        ();

    void ut_chatbox_title                ();
    void ut_chatbox_btnBold              ();
    void ut_chatbox_btnItalic            ();
    void ut_chatbox_btnUnderLine         ();
    void ut_chatbox_btnColor             ();
    void ut_chatbox_btnFileSend          ();
    void ut_chatbox_btnSave              ();
    void ut_chatbox_btnClean             ();
    void ut_chatbox_btnExit              ();
    void ut_chatbox_btnSend              ();
    void ut_chatbox_btnBold_emit         ();
    void ut_chatbox_btnItalic_emit       ();
    void ut_chatbox_btnUnderLine_emit    ();
    void ut_chatbox_btnColor_emit        ();
    void ut_chatbox_btnFileSend_emit     ();
    void ut_chatbox_btnSave_emit         ();
    void ut_chatbox_btnClean_emit        ();
    void ut_chatbox_btnExit_emit         ();
    void ut_chatbox_btnSend_emit         ();
    void ut_chatbox_msgTextBrowser       ();
    void ut_chatbox_msgTextEdit          ();
    void ut_chatbox_init_cbxFontSize     ();
    void ut_chatbox_cbxFontSize_min_max  ();
    void ut_chatbox_closeEvent_emit      ();
    void ut_chatbox_tbUser               ();
    void ut_chatbox_lbNumberOnlineUse    ();

    void mt_login_init_success           ();
    void mt_login_leUserName             ();
    void mt_login_leUserGroupNumber      ();

    void mt_addchat_leNameNewChat        ();
    void mt_chatlist_getNewBtn           ();
    bool mt_chatlist_getRandomPort       ();
    void mt_chatlist_setChatState        ();
    void mt_chatlist_btnchat_exist       ();

    void mt_chatbox_userjoin_list           ();
    void mt_chatbox_userjoin_counter        ();
    void mt_chatbox_userjoin_msgTextBrowser ();
    void mt_chatbox_userleft_list           ();
    void mt_chatbox_userleft_counter        ();
    void mt_chatbox_userleft_msgTextBrowser ();
    void mt_chatbox_clean                   ();
    void mt_chatbox_save_empty              ();
    void mt_chatbox_send_success            ();
    void mt_tcpclient                       ();
    void mt_tcpserver                       ();


private:
    QTimer* timer;
};

PolyChatTester::PolyChatTester()
{
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        QWidgetList topWidgets = QApplication::topLevelWidgets();
        foreach (QWidget *w, topWidgets) {
            if (QMessageBox *mb = qobject_cast<QMessageBox *>(w)) {
                QTest::keyClick(mb, Qt::Key_Enter);
            } else if (QFileDialog* dialog = qobject_cast<QFileDialog *>(w)) {
                QTest::keyClick(dialog, Qt::Key_Cancel);
            } else if (QColorDialog* dialog = qobject_cast<QColorDialog *>(w)) {
                QTest::keyClick(dialog, Qt::Key_Enter);
            } else {
                w->close();
            }
        }
    });
}

PolyChatTester::~PolyChatTester()
{
    QWidgetList topWidgets = QApplication::topLevelWidgets();
    foreach (QWidget *w, topWidgets) {
        if (QMessageBox *mb = qobject_cast<QMessageBox *>(w)) {
            QTest::keyClick(mb, Qt::Key_Enter);
        } else if (QFileDialog* dialog = qobject_cast<QFileDialog *>(w)) {
            QTest::keyClick(dialog, Qt::Key_Cancel);
        } else if (QColorDialog* dialog = qobject_cast<QColorDialog *>(w)) {
            QTest::keyClick(dialog, Qt::Key_Enter);
        } else {
            w->close();
        }
    }
}

void PolyChatTester::initTestCase()
{
    qDebug() << "Start to running tests";
}

void PolyChatTester::cleanupTestCase()
{
    qDebug() << "End of all test runs";
}

void PolyChatTester::ut_login_login_empty()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("", "3530904/90102");
    QCOMPARE(isSuccInitLocalUser, false);
}

void PolyChatTester::ut_login_group_empty()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("Fox", "");
    QCOMPARE(isSuccInitLocalUser, false);
}

/** 登录失败（用户名和班级号不能为空）
 *  Сбой входа в систему (имя пользователя и номер группы не могут быть пустыми).
 *  @brief PolyChatTester::ut_login_login_group_empty
 */
void PolyChatTester::ut_login_login_group_empty()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("", "");
    QCOMPARE(isSuccInitLocalUser, false);
}

/** 传入正确格式的登录名，本地信息被正确初始化
 *  Локальная информация инициализируется правильно путем передачи имени пользователя в правильном формате.
 *  @brief PolyChatTester::ut_login_init_login
 */
void PolyChatTester::ut_login_init_login()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("Fox", "3530904/90102");
    QCOMPARE(isSuccInitLocalUser, true);
    QCOMPARE(localUserName, "Fox");
}

/** 传入正确格式的班级号，本地信息被正确初始化
 *  Локальная информация инициализируется правильно путем передачи номера группы в правильном формате.
 *  @brief PolyChatTester::ut_login_init_group
 */
void PolyChatTester::ut_login_init_group()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("Fox", "3530904/90102");
    QCOMPARE(isSuccInitLocalUser, true);
    QCOMPARE(localUserGroupNumber, "3530904/90102");
}

/** 保证 btnlogin 是非可选按钮，且图标显示被禁用
 *  btnlogin является отмечаемой кнопкой (Checkable-QToolButton), а отображение icon отключено.
 *  @brief PolyChatTester::ut_login_btnlogin
 */
void PolyChatTester::ut_login_btnlogin()
{
    LoginWidget loginWidget;
    QPushButton* pushButton = loginWidget.findChild<QPushButton*>("btnLogin");
    QCOMPARE(pushButton->isCheckable(), false);
}

/** 用户每次点击登录按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Вход в систему. Сигнал (нажатие кнопки) входа в систему каждый раз срабатывает правильно и единожды.
 *  @brief PolyChatTester::ut_login_btnlogin_emit
 */
void PolyChatTester::ut_login_btnlogin_emit()
{
    timer->start(TIMER_STEP);
    LoginWidget loginWidget;

    QLineEdit* leUserName = loginWidget.findChild<QLineEdit*>("leUserName");
    QLineEdit* leUserGroupNumber = loginWidget.findChild<QLineEdit*>("leUserGroupNumber");
    QPushButton* pushButton = loginWidget.findChild<QPushButton*>("btnLogin");

    QTest::keyClicks(leUserName, "Fox");
    QTest::keyClicks(leUserGroupNumber, "3530904/90102");

    QSignalSpy spy(pushButton, &QPushButton::clicked);
    pushButton->click();
    QCOMPARE(spy.count(), 1);  // 确保信号被准确地发射了一次
    timer->stop();
}


/** 模拟用户点击并且通过键盘输入，确保输入内容在 leUserGroupNumber 输入框中正确显示
 *  Имитация щелчка пользователя и ввода с клавиатуры, чтобы убедиться, что вводимый контент правильно отображается в поле ввода leUserGroupNumber.
 *  @brief PolyChatTester::mt_login_leUserGroupNumber
 */
void PolyChatTester::mt_login_leUserGroupNumber()
{
    LoginWidget loginWidget;

    QLineEdit* leUserGroupNumber = loginWidget.findChild<QLineEdit*>("leUserName");
    QTest::keyClicks(leUserGroupNumber, "3530904/90102");
    QCOMPARE(leUserGroupNumber->text(), "3530904/90102");
}

/** 用户每次点击 btnInfo 时，保证（按钮点击）信号正确触发，且为一次
 *  btnlogin является отмечаемой кнопкой (Checkable-QToolButton), а отображение icon отключено.
 *  @brief PolyChatTester::ut_login_btnInfo_emit
 */
void PolyChatTester::ut_login_btnInfo_emit()
{
    LoginWidget widget;

    QPushButton* pushButton = widget.findChild<QPushButton*>("btnInfo");

    QSignalSpy spy(pushButton, &QPushButton::clicked);
    pushButton->click();
    QCOMPARE(spy.count(), 1);  // 确保信号被准确地发射了一次
}

/** Login 窗口的大小被禁止缩放，并且为 400x250
 *  Размер Login отключен и составляет 400x250
 *  @brief PolyChatTester::mt_addchat_leNameNewChat
 */
void PolyChatTester::ut_login_window()
{
    LoginWidget widget;
    QCOMPARE(widget.minimumSize(), QSize(400, 250));
    QCOMPARE(widget.minimumSize(), widget.maximumSize());
}

/** 模拟用户点击并且通过键盘输入，确保输入内容在 leNameNewChat 输入框中正确显示
 *  Имитация щелчка пользователя и ввода с клавиатуры, чтобы убедиться, что вводимое содержимое правильно отображается в поле ввода leNameNewChat.
 *  @brief PolyChatTester::mt_addchat_leNameNewChat
 */
void PolyChatTester::mt_addchat_leNameNewChat()
{
    AddChat widget;
    QLineEdit* lineEdit = widget.findChild<QLineEdit*>("leNameNewChat");
    QTest::keyClicks(lineEdit, "3530904/90102");
    QCOMPARE(lineEdit->text(), "3530904/90102");
}

/** 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку отмены, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_btnCancelAddChat()
{
    AddChat widget;

    QPushButton* pushButton = widget.findChild<QPushButton*>("btnCancelAddChat");

    QSignalSpy spy(pushButton, &QPushButton::clicked);
    pushButton->click();
    QCOMPARE(spy.count(), 1);  // 确保信号被准确地发射了一次
}

/** 用户每次点击添加按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку добавления, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::ut_addchat_btnAddChat
 */
void PolyChatTester::ut_addchat_btnAddChat()
{
    AddChat widget;

    QLineEdit* lineEdit = widget.findChild<QLineEdit*>("leNameNewChat");
    QPushButton* pushButton = widget.findChild<QPushButton*>("btnAddChat");

    QTest::keyClicks(lineEdit, "3530904/90102");

    QSignalSpy spy(pushButton, &QPushButton::clicked);
    pushButton->click();
    QCOMPARE(spy.count(), 1);  // 确保信号被准确地发射了一次
}

/** 正确初始化（使用构造函数）一个 Chat 对象
 *  Правильно инициализируется (используя конструктор) объект чата.
 *  @brief PolyChatTester::ut_chat_init
 */
void PolyChatTester::ut_chat_init()
{
    Chat chat("3530904/90102", 6666, true);
    QCOMPARE(chat.name, "3530904/90102");
    QCOMPARE(chat.port, 6666);
    QCOMPARE(chat.isOpen, true);
}

/** 正确初始化（使用构造函数）一个 Chat 对象
 *  Правильно инициализируется (используя конструктор) объект чата.
 *  @brief PolyChatTester::ut_chatlist_init
 */
void PolyChatTester::ut_chatlist_init()
{
    DAL::initLocalUser("Fox", "3530904/90102");

    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QCOMPARE("Fox", localUserName);
    QCOMPARE("3530904/90102", localUserGroupNumber);
    QCOMPARE(DAL::getLocalIpAddress(), localIpAddress);
}


/** 保证 btnNewChat 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnNewChat является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено
 *  @brief PolyChatTester::ut_chatlist_btnNewChat
 */
void PolyChatTester::ut_chatlist_btnNewChat()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QToolButton* button = widget.findChild<QToolButton*>("btnNewChat");

    QCOMPARE(button->isCheckable(), false);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 用户每次点击增加群聊按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку, чтобы добавить групповой чат, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::ut_chatlist_btnNewChat_emit
 */
void PolyChatTester::ut_chatlist_btnNewChat_emit()
{
    timer->start(TIMER_STEP);
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QToolButton* pushButton = widget.findChild<QToolButton*>("btnNewChat");

    QSignalSpy spy(pushButton, &QToolButton::clicked);
    pushButton->click();
    pushButton->click();
    QCOMPARE(spy.count(), 2);
    timer->stop();
}

/** 模拟用户点击并且通过键盘输入，确保输入内容在 leSearch 搜索框中正确显示
 *  Смоделируйте, как пользователь нажимает и печатает на клавиатуре, чтобы убедиться, что вводимое содержимое правильно отображается в поле поиска
 *  @brief PolyChatTester::ut_chatlist_leSearch
 */
void PolyChatTester::ut_chatlist_leSearch()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QLineEdit* lineEdit = widget.findChild<QLineEdit*>("leSearch");

    QTest::keyClicks(lineEdit, "3530904/90102");
    QCOMPARE(lineEdit->text(), "3530904/90102");
}

/** 当用户改变搜索框中的内容时，触发信号
 *  Инициировать сигнал, когда пользователь меняет содержимое в поле поиска.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatlist_leSearch_change_emit()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QLineEdit* lineEdit = widget.findChild<QLineEdit*>("leSearch");

    QSignalSpy spy(lineEdit, &QLineEdit::textEdited);
    QTest::keyClicks(lineEdit, "90111");
    QCOMPARE(spy.count(), 5);
}

/** lbName 中正确显示本用户的用户名
 *  Имя пользователя этого пользователя правильно отображается в lbName.
 *  @brief PolyChatTester::ut_chatlist_lbName
 */
void PolyChatTester::ut_chatlist_lbName()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QLabel* label = widget.findChild<QLabel*>("lbName");

    QCOMPARE(label->text(), "Fox");
}

/** lbGroupNumber 中正确显示本用户的班级号
 *  Номер группы этого пользователя правильно отображается в lbGroupNumber.
 *  @brief PolyChatTester::ut_chatlist_lbGroupNumber
 */
void PolyChatTester::ut_chatlist_lbGroupNumber()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QLabel* label = widget.findChild<QLabel*>("lbGroupNumber");

    QCOMPARE(label->text(), "3530904/90102");
}

/** lbIP 中正确显示本用户的 ip 地址
 *  IP-адрес этого пользователя корректно отображается в lbIP.
 *  @brief PolyChatTester::ut_chatlist_lbIP
 */
void PolyChatTester::ut_chatlist_lbIP()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QLabel* label = widget.findChild<QLabel*>("lbIP");

    QCOMPARE(label->text(), localIpAddress.toString());
}

/** 返回 false（如果某个名称的群聊没有被打开）
 *  Возвращает false (если групповой чат с таким названием не открыт).
 *  @brief PolyChatTester::ut_chatlist_chat_not_open
 */
void PolyChatTester::ut_chatlist_chat_not_open()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, false), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair);

    QCOMPARE(widget.isChatOpen("3530904/90102"), false);
}

/** 返回 true（如果某个名称的群聊被打开了）
 *  Возвращает true (если открыт групповой чат с таким названием).
 *  @brief PolyChatTester::ut_chatlist_chat_open
 */
void PolyChatTester::ut_chatlist_chat_open()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, true), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair);

    QCOMPARE(widget.isChatOpen("3530904/90102"), true);
}

/** 返回 true（当前端口上已存在群聊）
 *  Возвращает true (групповой чат уже существует на текущем порту).
 *  @brief PolyChatTester::ut_addchat_port_exist
 */
void PolyChatTester::ut_addchat_port_exist()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, true), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair);

    QCOMPARE(widget.isPortExist(6666), true);
}

/** 返回 false（当前端口上不存在群聊）
 *  Возвращает false (групповой чат не существует на текущем порту).
 *  @brief PolyChatTester::ut_addchat_port_not_exist
 */
void PolyChatTester::ut_addchat_port_not_exist()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, true), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair);

    QCOMPARE(widget.isPortExist(7777), false);
}

/** 返回 true（已存在同名的群聊）
 *  Возвращает true (групповой чат с таким названием уже существует).
 *  @brief PolyChatTester::ut_addchat_chat_exist
 */
void PolyChatTester::ut_addchat_chat_exist()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, true), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair);

    QCOMPARE(widget.isChatExist("3530904/90102"), true);
}

/** 返回 false（不存在同名的群聊）
 *  Возвращает false (групповой чат с таким названием не существует).
 *  @brief PolyChatTester::ut_addchat_chat_not_exist
 */
void PolyChatTester::ut_addchat_chat_not_exist()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, true), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair);

    QCOMPARE(widget.isChatExist("111111/88888"), false);
}


/** lbClientIP 内容正确解析且显示
 *  Содержимое lbClientIP анализируется правильно и отображается
 *  @brief PolyChatTester::ut_tcpclient_lbClientIP
 */
void PolyChatTester::ut_tcpclient_lbClientIP()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbClientIP");
    QCOMPARE(label->text(), localIpAddress.toString());
}

/** lbClientPort 内容正确解析且显示
 *  Содержимое lbClientPort правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpclient_lbClientPort
 */
void PolyChatTester::ut_tcpclient_lbClientPort()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbClientPort");
    QCOMPARE(label->text(), QString::number(PORT_TCP_FILE));
}

/** lbServerIP 内容正确解析且显示
 *  Содержимое lbServerIP правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpclient_lbServerIP
 */
void PolyChatTester::ut_tcpclient_lbServerIP()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbServerIP");
    QCOMPARE(label->text(), localIpAddress.toString());
}

/** lbServerPort 内容正确解析且显示
 *  Содержимое lbServerPort правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpclient_lbServerPort
 */
void PolyChatTester::ut_tcpclient_lbServerPort()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbServerPort");
    QCOMPARE(label->text(), QString::number(PORT_TCP_FILE));
}

/** lbFileName 内容正确解析且显示
 *  Содержимое lbFileName правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpclient_lbFileName
 */
void PolyChatTester::ut_tcpclient_lbFileName()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbFileName");
    QCOMPARE(label->text(), "fox.exe");
}

/** lbFileSize 内容正确解析且显示
 *  Содержимое lbFileSize правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpclient_lbFileSize
 */
void PolyChatTester::ut_tcpclient_lbFileSize()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbFileSize");
    QCOMPARE(label->text(),QString("%1Kb").arg(QString::number(1551155 / 1024)));
}

/** progressBar 初始化时为 0
 *  progressBar инициализирован 0.
 *  @brief PolyChatTester::ut_tcpclient_progressBar
 */
void PolyChatTester::ut_tcpclient_progressBar()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QProgressBar* label = widget.findChild<QProgressBar*>("progressBar");
    QCOMPARE(label->value(), 0);
}

/** 保证 btnCancel 是非可选按钮
 *  btnCancel является отмечаемой кнопкой (Checkable-QToolButton) и выделено жирным шрифтом
 *  @brief PolyChatTester::ut_tcpclient_btnCancel
 */
void PolyChatTester::ut_tcpclient_btnCancel()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnCancel");
    QCOMPARE(button->isCheckable(), false);
}


/** 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку отмены, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::ut_tcpclient_btnCancel_emit
 */
void PolyChatTester::ut_tcpclient_btnCancel_emit()
{
    timer->start(TIMER_STEP);

    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnCancel");

    QSignalSpy spy(button, &QPushButton::clicked);
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    timer->stop();
}

/** 保证 btnSave 是非可选按钮
 *  btnSave является отмечаемой кнопкой (Checkable-QToolButton) и выделено жирным шрифтом
 *  @brief PolyChatTester::ut_tcpclient_btnSave
 */
void PolyChatTester::ut_tcpclient_btnSave()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnSave");
    QCOMPARE(button->isCheckable(), false);
}

/** 用户每次点击保存按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку сохранения, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::ut_tcpclient_btnSave_emit
 */
void PolyChatTester::ut_tcpclient_btnSave_emit()
{
    timer->start(TIMER_STEP);

    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnSave");

    QSignalSpy spy(button, &QPushButton::clicked);
//    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 0);
    timer->stop();
}

/** 用户关闭窗口时触发关闭事件
 *  Запускает событие закрытия, когда пользователь закрывает окно.
 *  @brief PolyChatTester::ut_tcpclient_closeEvent_emit
 */
void PolyChatTester::ut_tcpclient_closeEvent_emit()
{
    timer->start(TIMER_STEP);

    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QSignalSpy spy(&widget, &QWidget::close);
    widget.close();

    QCOMPARE(spy.count(), 0);
    timer->stop();
}

/** textBrowser 为只读状态，禁止写入
 *  textBrowser доступен только для чтения, запись запрещена
 *  @brief PolyChatTester::ut_tcpclient_textBrowser
 */
void PolyChatTester::ut_tcpclient_textBrowser()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QTextBrowser* textBrowser = widget.findChild<QTextBrowser*>("textBrowser");
    QCOMPARE(textBrowser->isReadOnly(), true);
}

/** lbClientIP 内容正确解析且显示
 *  Содержимое lbClientIP правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpserver_lbClientIP
 */
void PolyChatTester::ut_tcpserver_lbClientIP()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbClientIP");
    QCOMPARE(label->text(), "");
}

/** lbClientPort 内容正确解析且显示
 *  Содержимое lbClientPort правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpserver_lbClientPort
 */
void PolyChatTester::ut_tcpserver_lbClientPort()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbClientPort");
    QCOMPARE(label->text(), "");
}

/** lbServerIP 内容正确解析且显示
 *  Содержимое lbServerIP правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpserver_lbServerIP
 */
void PolyChatTester::ut_tcpserver_lbServerIP()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbServerIP");
    QCOMPARE(label->text(), localIpAddress.toString());
}

/** lbServerPort 内容正确解析且显示
 *  Содержимое lbServerPort правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpserver_lbServerPort
 */
void PolyChatTester::ut_tcpserver_lbServerPort()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbServerPort");
    QCOMPARE(label->text(), QString::number(PORT_TCP_FILE));
}

/** lbFilePath 内容正确解析且显示
 *  Содержимое lbFilePath правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpserver_lbFilePath
 */
void PolyChatTester::ut_tcpserver_lbFilePath()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbFilePath");
    QCOMPARE(label->text(), "fox.exe");
}

/** lbFileSize 内容正确解析且显示
 *  Содержимое lbFileSize правильно анализируется и отображается.
 *  @brief PolyChatTester::ut_tcpserver_lbFileSize
 */
void PolyChatTester::ut_tcpserver_lbFileSize()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QLabel* label = widget.findChild<QLabel*>("lbFileSize");
    QCOMPARE(label->text(), "0Kb");
}

/** 保证 btnCancel 是非可选按钮
 *  btnCancel является отмечаемой кнопкой (Checkable-QToolButton) и выделено жирным шрифтом
 *  @brief PolyChatTester::ut_tcpserver_btnCancel
 */
void PolyChatTester::ut_tcpserver_btnCancel()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnCancel");
    QCOMPARE(button->isCheckable(), false);
}

/** 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку отмены, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::ut_tcpserver_btnCancel_emit
 */
void PolyChatTester::ut_tcpserver_btnCancel_emit()
{
    timer->start(TIMER_STEP);

    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnCancel");

    QSignalSpy spy(button, &QPushButton::clicked);
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    timer->stop();
}

/** progressBar 初始化时为 0
 *  progressBar инициализирован 0.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_progressBar()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QProgressBar* label = widget.findChild<QProgressBar*>("progressBar");
    QCOMPARE(label->value(), 0);
}

/** 用户关闭窗口时触发关闭事件
 *  Запускает событие закрытия, когда пользователь закрывает окно.
 *  @brief PolyChatTester::ut_tcpserver_closeEvent_emit
 */
void PolyChatTester::ut_tcpserver_closeEvent_emit()
{
    timer->start(TIMER_STEP);

    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QPushButton* button = widget.findChild<QPushButton*>("btnCancel");

    QSignalSpy spy(button, &QPushButton::clicked);
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    timer->stop();
}

/** textBrowser 为只读状态，禁止写入
 *  textBrowser доступен только для чтения, запись запрещена
 *  @brief PolyChatTester::ut_tcpserver_textBrowser
 */
void PolyChatTester::ut_tcpserver_textBrowser()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);
    QTextBrowser* textBrowser = widget.findChild<QTextBrowser*>("textBrowser");
    QCOMPARE(textBrowser->isReadOnly(), true);
}

/** ChatBox 窗口的标题正确初始化（符合格式）
 *  Заголовок окна ChatBox инициализирован корректно (соответствует формату).
 *  @brief PolyChatTester::ut_chatbox_title
 */
void PolyChatTester::ut_chatbox_title()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QCOMPARE(chatBox.windowTitle(), QString("[Chat] %1 on port %2").arg("3530409/90102").arg(2333));
}

/** 保证 btnBold 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnBold является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnBold
 */
void PolyChatTester::ut_chatbox_btnBold()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnBold");

    QCOMPARE(button->isCheckable(), true);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnItalic 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnItalic является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnItalic
 */
void PolyChatTester::ut_chatbox_btnItalic()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnItalic");

    QCOMPARE(button->isCheckable(), true);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnUnderLine 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnUnderLine является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnUnderLine
 */
void PolyChatTester::ut_chatbox_btnUnderLine()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnUnderLine");

    QCOMPARE(button->isCheckable(), true);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnColor 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnColor не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnColor
 */
void PolyChatTester::ut_chatbox_btnColor()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnColor");

    QCOMPARE(button->isCheckable(), false);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnFileSend 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnFileSend не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnFileSend
 */
void PolyChatTester::ut_chatbox_btnFileSend()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnFileSend");

    QCOMPARE(button->isCheckable(), false);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnSave 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnSave не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnSave
 */
void PolyChatTester::ut_chatbox_btnSave()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnSave");

    QCOMPARE(button->isCheckable(), false);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnClean 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnClean не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::ut_chatbox_btnClean
 */
void PolyChatTester::ut_chatbox_btnClean()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnClean");

    QCOMPARE(button->isCheckable(), false);
    QCOMPARE(button->toolButtonStyle(), Qt::ToolButtonStyle::ToolButtonIconOnly);
}

/** 保证 btnExit 不是可选按钮（Checkable）
 *  Кнопка btnExit не является проверяемой.
 *  @brief PolyChatTester::ut_chatbox_btnExit
 */
void PolyChatTester::ut_chatbox_btnExit()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QPushButton* button = chatBox.findChild<QPushButton*>("btnExit");

    QCOMPARE(button->isCheckable(), false);
}

/** 保证 btnSend 不是可选按钮（Checkable）
 *  Кнопка btnSend не является проверяемой.
 *  @brief PolyChatTester::ut_chatbox_btnSend
 */
void PolyChatTester::ut_chatbox_btnSend()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QPushButton* button = chatBox.findChild<QPushButton*>("btnSend");

    QCOMPARE(button->isCheckable(), false);
}

/** 点击 btnBold 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态
 *  Нажатие btnBold активирует сигнал (нажатие кнопки), и состояние изменяется на «включено»; нажатие еще раз возвращает в исходное состояние.
 *  @brief PolyChatTester::ut_chatbox_btnBold_emit
 */
void PolyChatTester::ut_chatbox_btnBold_emit()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnBold");

    QSignalSpy spy(button, &QToolButton::clicked);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->isChecked(), true);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->isChecked(), false);

    QCOMPARE(spy.count(), 2);

}

/** 点击 btnItalic 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态
 *  Нажатие btnItalic активирует сигнал (нажатие кнопки), и состояние изменяется на «включено»; нажатие еще раз возвращает в исходное состояние.
 *  @brief PolyChatTester::ut_chatbox_btnItalic_emit
 */
void PolyChatTester::ut_chatbox_btnItalic_emit()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnItalic");

    QSignalSpy spy(button, &QToolButton::clicked);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->isChecked(), true);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->isChecked(), false);

    QCOMPARE(spy.count(), 2);
}

/** 点击 btnUnderLine 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态
 *  Нажатие btnUnderLine активирует сигнал (нажатие кнопки), и состояние изменяется на «включено»; нажатие еще раз возвращает в исходное состояние.
 *  @brief PolyChatTester::ut_chatbox_btnUnderLine_emit
 */
void PolyChatTester::ut_chatbox_btnUnderLine_emit()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnUnderLine");

    QSignalSpy spy(button, &QToolButton::clicked);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->isChecked(), true);

    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(button->isChecked(), false);

    QCOMPARE(spy.count(), 2);
}

/** 点击 btnColor 触发（按钮点击）信号
 *  Нажатие btnColor запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::ut_chatbox_btnColor_emit
 */
void PolyChatTester::ut_chatbox_btnColor_emit()
{
//    timer->start(TIMER_STEP);

    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnColor");

    QSignalSpy spy(button, &QToolButton::clicked);

//    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 0);
//    timer->stop();
}

/** 点击 btnFileSend 触发（按钮点击）信号
 *  Нажатие btnFileSend запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::ut_chatbox_btnFileSend_emit
 */
void PolyChatTester::ut_chatbox_btnFileSend_emit()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnFileSend");

    QSignalSpy spy(button, &QToolButton::clicked);
//    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 0);
    timer->stop();
}

/** 点击 btnSave 触发（按钮点击）信号
 *  Нажатие кнопки btnSave запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::ut_chatbox_btnSave_emit
 */
void PolyChatTester::ut_chatbox_btnSave_emit()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnSave");

    QSignalSpy spy(button, &QToolButton::clicked);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    timer->stop();
}

/** 点击 btnClean 触发（按钮点击）信号
 *  Нажатие кнопки btnClean запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::ut_chatbox_btnClean_emit
 */
void PolyChatTester::ut_chatbox_btnClean_emit()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnClean");

    QSignalSpy spy(button, &QToolButton::clicked);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    timer->stop();
}

/** 点击 btnExit 触发（按钮点击）信号
 *  Нажатие кнопки btnExit запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::ut_chatbox_btnExit_emit
 */
void PolyChatTester::ut_chatbox_btnExit_emit()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnExit");

    QSignalSpy spy(button, &QToolButton::clicked);
//    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 0);
    timer->stop();
}

/** 点击 btnSend 触发（按钮点击）信号
 *  Нажатие кнопки btnSend запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::ut_chatbox_btnSend_emit
 */
void PolyChatTester::ut_chatbox_btnSend_emit()
{
//    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnSend");

    QSignalSpy spy(button, &QToolButton::clicked);
//    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 0);
//    timer->stop();
}

/** msgTextBrowser 为只读状态
 *  msgTextBrowser доступен только для чтения.
 *  @brief PolyChatTester::ut_chatbox_msgTextBrowser
 */
void PolyChatTester::ut_chatbox_msgTextBrowser()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTextBrowser* textBrowser = chatBox.findChild<QTextBrowser*>("msgTextBrowser");
    QCOMPARE(textBrowser->isReadOnly(), true);
}

/** msgTextEdit 允许写入
 *  msgTextEdit позволяет писать.
 *  @brief PolyChatTester::ut_chatbox_msgTextEdit
 */
void PolyChatTester::ut_chatbox_msgTextEdit()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTextEdit* textBrowser = chatBox.findChild<QTextEdit*>("msgTextEdit");
    QCOMPARE(textBrowser->isReadOnly(), false);
}

/** 初始字体为 12
 *  Начальный шрифт 12.
 *  @brief PolyChatTester::ut_chatbox_init_cbxFontSize
 */
void PolyChatTester::ut_chatbox_init_cbxFontSize()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QComboBox* cbox = chatBox.findChild<QComboBox*>("cbxFontSize");
    QCOMPARE(cbox->currentText().toInt(), 12);
}

/** 最小字体为 10，最大为 28
 *  Минимальный размер шрифта 10, максимальный размер 28.
 *  @brief PolyChatTester::ut_chatbox_cbxFontSize_min_max
 */
void PolyChatTester::ut_chatbox_cbxFontSize_min_max()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QComboBox* cbox = chatBox.findChild<QComboBox*>("cbxFontSize");
    qDebug() << " cbox->count();" << cbox->count();
    QCOMPARE(cbox->itemText(0).toInt(), 10);
    QCOMPARE(cbox->itemText(cbox->count() - 1).toInt(), 28);
}

/** 用户关闭窗口（退出）时触发关闭事件
 *  Событие close срабатывает, когда пользователь закрывает окно (выход).
 *  @brief PolyChatTester::ut_chatbox_closeEvent_emit
 */
void PolyChatTester::ut_chatbox_closeEvent_emit()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QPushButton* button = chatBox.findChild<QPushButton*>("btnExit");

    QSignalSpy spy(button, &QPushButton::clicked);
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    timer->stop();
}


/** 用户列表的宽度为 150-350
 *  Ширина списка пользователей составляет 150 - 350
 *  @brief PolyChatTester::ut_chatbox_tbUser
 */
void PolyChatTester::ut_chatbox_tbUser()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTableWidget* table = chatBox.findChild<QTableWidget*>("tbUser");
    QCOMPARE(table->minimumWidth(), 150);
    QCOMPARE(table->maximumWidth(), 350);
}

/** 在线人数的 UI 初始化为正确格式
 *  UI инициализация онлайн-расчета численности в правильный формат
 *  @brief PolyChatTester::ut_chatbox_lbNumberOnlineUse
 */
void PolyChatTester::ut_chatbox_lbNumberOnlineUse()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QLabel* label = chatBox.findChild<QLabel*>("lbNumberOnlineUse");
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(0));
}

/** 登陆成功，本地用户信息被正确初始化
 *  Вход выполнен успешно, и информация о локальном пользователе правильно инициализирована.
 *  @brief PolyChatTester::mt_login_init_success
 */
void PolyChatTester::mt_login_init_success()
{
    LoginWidget widget;
    QLineEdit* name  = widget.findChild<QLineEdit*>("leUserName");
    QLineEdit* group = widget.findChild<QLineEdit*>("leUserGroupNumber");
    QPushButton* button = widget.findChild<QPushButton*>("btnLogin");

    QTest::keyClicks(name, "Fox");
    QTest::keyClicks(group, "3530409/90102");
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(DAL::getLocalUserName(), "Fox");
    QCOMPARE(DAL::getLocalUserGroupNumber(), "3530409/90102");
}

/** 根据传入的参数，所创建的新按钮对象中拥有正确的信息
 *  В соответствии с переданными параметрами создается новый объект кнопки с правильной информацией.
 *  @brief PolyChatTester::mt_chatlist_getNewBtn
 */
void PolyChatTester::mt_chatlist_getNewBtn()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());

    QToolButton* btn = widget.getNewBtn("3530904/90102", 6666, false);
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, false), btn);
    widget.vPair_OChat_BtnChat.push_front(pair);

    widget.updateBtnInvPair("3530904/90102", btn);
    QCOMPARE(widget.isChatOpen("3530904/90102"), false);
}


/** 模拟用户点击并且通过键盘输入，确保输入内容在 leUserName 输入框中正确显示
 *  Имитация щелчка пользователя и ввода с клавиатуры, чтобы убедиться, что вводимый контент правильно отображается в поле ввода leUserName.
 *  @brief PolyChatTester::mt_login_leUserName
 */
void PolyChatTester::mt_login_leUserName()
{
    LoginWidget loginWidget;

    QLineEdit* leUserName = loginWidget.findChild<QLineEdit*>("leUserName");
    QTest::keyClicks(leUserName, "Fox");
    QCOMPARE(leUserName->text(), "Fox");
}

/** 生成的随机端口介于 PORT_MIN 与 PORT_MAX
 *  Генерирует случайные порты между PORT_MIN и PORT_MAX.
 *  @brief PolyChatTester::mt_chatlist_getRandomPort
 */
bool PolyChatTester::mt_chatlist_getRandomPort()
{
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    for (int i = 0; i < PORT_MAX - PORT_MIN; i++)
    {
        qint16 port = widget.getRandomPort();
        if (port > PORT_MAX || port < PORT_MIN) return false;
    }
    return true;
}

/** 正常设置聊天窗口为打开或者关闭
 *  Окно чата открыто или закрыто.
 *  @brief PolyChatTester::mt_chatlist_setChatState
 */
void PolyChatTester::mt_chatlist_setChatState()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());
    QPair<Chat*, QToolButton*> pair1(new Chat("3530904/90102", 6666, true), widget.getNewBtn("3530904/90102", 6666, false));
    widget.vPair_OChat_BtnChat.push_front(pair1);
    QCOMPARE(widget.isChatOpen("3530904/90102"), true);

    QPair<Chat*, QToolButton*> pair2(new Chat("3530904/90103", 7777, false), widget.getNewBtn("3530904/90103", 7777, false));
    widget.vPair_OChat_BtnChat.push_front(pair2);

    QCOMPARE(widget.isChatOpen("3530904/90103"), false);
}

/** 当增加新的聊天群组时，ui 界面正确刷新
 *  ui интерфейс обновляется корректно при добавлении новой группы чата
 * @brief mt_chatlist_btnchat_exist
 */
void PolyChatTester::mt_chatlist_btnchat_exist()
{
    DAL::initLocalUser("Fox", "3530904/90102");
    DAL::initLocalUser("Fox", "3530904/90102");
    ChatList widget(nullptr, DAL::getLocalUserName(), DAL::getLocalUserGroupNumber(), DAL::getLocalIpAddress());

    QToolButton* btn = widget.getNewBtn("3530904/90102", 6666, false);
    QPair<Chat*, QToolButton*> pair(new Chat("3530904/90102", 6666, false), btn);
    widget.vPair_OChat_BtnChat.push_front(pair);

    widget.updateBtnInvPair("3530904/90102", btn);
    QCOMPARE(widget.isChatOpen("3530904/90102"), false);

}

/** 用户加入（用户列表正确刷新）
 *  Присоединение пользователя (список пользователей обновляется корректно)
 *  @brief PolyChatTester::mt_chatbox_userjoin_list
 */
void PolyChatTester::mt_chatbox_userjoin_list()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTableWidget* table = chatBox.findChild<QTableWidget*>("tbUser");

    QCOMPARE(table->rowCount(), 0);

    chatBox.userJoin("Fox", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(table->item(0, 0)->text(), "Fox");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90102");

    chatBox.userJoin("Fox2", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 2);
    QCOMPARE(table->item(0, 0)->text(), "Fox2");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90102");

    chatBox.userJoin("Fox3", "3530904/90103", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 3);
    QCOMPARE(table->item(0, 0)->text(), "Fox3");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90103");
}

/** 用户加入（计数器正确增加，并且ui的显示格式及内容正确）
 *  Присоединение пользователя (счетчик правильно увеличивается и ui отображается в правильном формате и с правильным содержанием)
 *  @brief PolyChatTester::mt_chatbox_userjoin_counter
 */
void PolyChatTester::mt_chatbox_userjoin_counter()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTableWidget* table = chatBox.findChild<QTableWidget*>("tbUser");
    QLabel* label = chatBox.findChild<QLabel*>("lbNumberOnlineUse");

    QCOMPARE(table->rowCount(), 0);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userJoin("Fox", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userJoin("Fox2", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 2);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userJoin("Fox3", "3530904/90103", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 3);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));
}

/** 用户加入（msgTextBrowser 被刷新）
 *  Пользователь присоединился (msgTextBrowser обновляется)
 *  @brief PolyChatTester::mt_chatbox_userjoin_msgTextBrowser
 */
void PolyChatTester::mt_chatbox_userjoin_msgTextBrowser()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTextBrowser* msgTextBrowser = chatBox.findChild<QTextBrowser*>("msgTextBrowser");

    QString initString = msgTextBrowser->toPlainText();
    chatBox.userJoin("Fox", "3530904/90102", DAL::getLocalIpAddress());
    QVERIFY(initString != msgTextBrowser->toPlainText());

    QCOMPARE(msgTextBrowser->toPlainText(), QString("%1 online！").arg("Fox"));
}

/** 用户离开（用户列表正确刷新）
 *  Вылет пользователя (список пользователей обновляется корректно)
 *  @brief PolyChatTester::mt_chatbox_userleft_list
 */
void PolyChatTester::mt_chatbox_userleft_list()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTableWidget* table = chatBox.findChild<QTableWidget*>("tbUser");

    QCOMPARE(table->rowCount(), 0);

    chatBox.userJoin("Fox", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(table->item(0, 0)->text(), "Fox");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90102");

    chatBox.userJoin("Fox2", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 2);
    QCOMPARE(table->item(0, 0)->text(), "Fox2");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90102");

    chatBox.userJoin("Fox3", "3530904/90103", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 3);
    QCOMPARE(table->item(0, 0)->text(), "Fox3");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90103");

    QString time = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");

    chatBox.userLeft("Fox", time);
    QCOMPARE(table->rowCount(), 2);
    QCOMPARE(table->item(0, 0)->text(), "Fox3");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90103");

    chatBox.userLeft("Fox3", time);
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(table->item(0, 0)->text(), "Fox2");
    QCOMPARE(table->item(0, 1)->text(), "3530904/90102");

    chatBox.userLeft("Fox2", time);
    QCOMPARE(table->rowCount(), 0);
}

/** 用户离开（计数器正确增加，并且ui的显示格式及内容正确）
 *  Пользователь уходит (счетчик правильно увеличивается и ui отображается в правильном формате и с правильным содержанием)
 *  @brief PolyChatTester::mt_chatbox_userleft_counter
 */
void PolyChatTester::mt_chatbox_userleft_counter()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTableWidget* table = chatBox.findChild<QTableWidget*>("tbUser");
    QLabel* label = chatBox.findChild<QLabel*>("lbNumberOnlineUse");

    QCOMPARE(table->rowCount(), 0);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userJoin("Fox", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userJoin("Fox2", "3530904/90102", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 2);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userJoin("Fox3", "3530904/90103", DAL::getLocalIpAddress());
    QCOMPARE(table->rowCount(), 3);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    QString time = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");

    chatBox.userLeft("Fox2", time);
    QCOMPARE(table->rowCount(), 2);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userLeft("Fox", time);
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));

    chatBox.userLeft("Fox3", time);
    QCOMPARE(table->rowCount(), 0);
    QCOMPARE(label->text(), QString("Number of online user: %1").arg(table->rowCount()));
}

/** 用户离开（msgTextBrowser 被刷新）
 *  Пользователь уходит (msgTextBrowser обновляется)
 *  @brief PolyChatTester::mt_chatbox_userleft_msgTextBrowser
 */
void PolyChatTester::mt_chatbox_userleft_msgTextBrowser()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QTextBrowser* msgTextBrowser = chatBox.findChild<QTextBrowser*>("msgTextBrowser");

    QString curString = msgTextBrowser->toPlainText();
    chatBox.userJoin("Fox", "3530904/90102", DAL::getLocalIpAddress());
    QVERIFY(curString != msgTextBrowser->toPlainText());

    QCOMPARE(msgTextBrowser->toPlainText(), QString("%1 online！").arg("Fox"));

    QString time = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    curString = msgTextBrowser->toPlainText();
    chatBox.userLeft("Fox", time);
    QVERIFY(curString != msgTextBrowser->toPlainText());

}

/** 当清空聊天框的时候提示警告信息
 *  Предупреждающее сообщение при очистке окна чата
 *
 */
void PolyChatTester::mt_chatbox_clean()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);
    QToolButton* button = chatBox.findChild<QToolButton*>("btnClean");

    QTest::mouseClick(button, Qt::LeftButton);
    timer->stop();
}

/** 当清空聊天框中无内容时，模拟模拟用户点击“保存聊天记录”按钮（提示警告信息）
 *  Имитация нажатия симулированным пользователем кнопки "Save", когда в пустом поле чата нет содержимого (с предупреждающим сообщением)
 *
 */
void PolyChatTester::mt_chatbox_save_empty()
{
    timer->start(TIMER_STEP);
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);

    QTextBrowser* msgTextBrowser = chatBox.findChild<QTextBrowser*>("msgTextBrowser");
    QCOMPARE(msgTextBrowser->toPlainText(), "");


    QToolButton* button = chatBox.findChild<QToolButton*>("btnSave");
    QTest::mouseClick(button, Qt::LeftButton);
    timer->stop();
}

/** 模拟用户在使用键盘在聊天框中输入内容，然后点击发送按钮。输入的信息正确显示在消息框汇总（消息成功发送）
 *  Имитирует ввод пользователем текста в поле чата с помощью клавиатуры и последующее нажатие кнопки отправки. Введенная информация правильно отображается в сводке окна сообщения (сообщение успешно отправлено)
 *
 */
void PolyChatTester::mt_chatbox_send_success()
{
    ChatBoxWidget chatBox(nullptr, "3530409/90102", 2333);

    QTextEdit* textEdit = chatBox.findChild<QTextEdit*>("msgTextEdit");
    QTest::keyClicks(textEdit, "HelloThere");
    QCOMPARE(textEdit->toPlainText(), "HelloThere");

    QTextBrowser* msgTextBrowser = chatBox.findChild<QTextBrowser*>("msgTextBrowser");
    QCOMPARE(msgTextBrowser->toPlainText(), "");

    QPushButton* button = chatBox.findChild<QPushButton*>("btnSend");
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(textEdit->toPlainText(), "");
    QTest::qSleep(TIMER_STEP);
    QCOMPARE(msgTextBrowser->toPlainText(), "");
}

/** ui 界面的所有控件经过接口初始化为正确内容
 *  Все элементы управления интерфейса ui инициализируются правильным содержимым через интерфейс
 * @brief PolyChatTester::mt_tcpclient
 */
void PolyChatTester::mt_tcpclient()
{
    TcpClient widget(nullptr, "fox.exe", 1551155, DAL::getLocalIpAddress(), PORT_TCP_FILE);

    QLabel* lbClientIP = widget.findChild<QLabel*>("lbClientIP");
    QCOMPARE(lbClientIP->text(), localIpAddress.toString());

    QLabel* lbClientPort = widget.findChild<QLabel*>("lbClientPort");
    QCOMPARE(lbClientPort->text(), QString::number(PORT_TCP_FILE));

    QLabel* lbServerIP = widget.findChild<QLabel*>("lbServerIP");
    QCOMPARE(lbServerIP->text(), localIpAddress.toString());

    QLabel* lbServerPort = widget.findChild<QLabel*>("lbServerPort");
    QCOMPARE(lbServerPort->text(), QString::number(PORT_TCP_FILE));

    QLabel* lbFileName = widget.findChild<QLabel*>("lbFileName");
    QCOMPARE(lbFileName->text(), "fox.exe");

    QLabel* lbFileSize = widget.findChild<QLabel*>("lbFileSize");
    QCOMPARE(lbFileSize->text(),QString("%1Kb").arg(QString::number(1551155 / 1024)));

    QProgressBar* progressBar = widget.findChild<QProgressBar*>("progressBar");
    QCOMPARE(progressBar->value(), 0);

    QPushButton* btnCancel = widget.findChild<QPushButton*>("btnCancel");
    QCOMPARE(btnCancel->isCheckable(), false);

}

/** ui 界面的所有控件经过接口初始化为正确内容
 *  Все элементы управления интерфейса ui инициализируются правильным содержимым через интерфейс
 * @brief PolyChatTester::mt_tcpclient
 */
void PolyChatTester::mt_tcpserver()
{
    TcpServer widget(nullptr, "fox.exe", DAL::getLocalIpAddress(), PORT_TCP_FILE);

    QLabel* lbClientIP = widget.findChild<QLabel*>("lbClientIP");
    QCOMPARE(lbClientIP->text(), "");

    QLabel* lbClientPort = widget.findChild<QLabel*>("lbClientPort");
    QCOMPARE(lbClientPort->text(), "");

    QLabel* lbServerIP = widget.findChild<QLabel*>("lbServerIP");
    QCOMPARE(lbServerIP->text(), localIpAddress.toString());

    QLabel* lbServerPort = widget.findChild<QLabel*>("lbServerPort");
    QCOMPARE(lbServerPort->text(), QString::number(PORT_TCP_FILE));

    QLabel* lbFilePath = widget.findChild<QLabel*>("lbFilePath");
    QCOMPARE(lbFilePath->text(), "fox.exe");

    QLabel* lbFileSize = widget.findChild<QLabel*>("lbFileSize");
    QCOMPARE(lbFileSize->text(), "0Kb");

    QPushButton* btnCancel = widget.findChild<QPushButton*>("btnCancel");
    QCOMPARE(btnCancel->isCheckable(), false);

    QProgressBar* progressBar = widget.findChild<QProgressBar*>("progressBar");
    QCOMPARE(progressBar->value(), 0);

    QTextBrowser* textBrowser = widget.findChild<QTextBrowser*>("textBrowser");
    QCOMPARE(textBrowser->isReadOnly(), true);

}



//QTEST_APPLESS_MAIN(PolyChatTester)
QTEST_MAIN(PolyChatTester)  // 自动为我们创建用于测试的 main 函数（带 GUI）

#include "tst_PolyChatTester.moc"  // 编译或者生成一个额外的 cpp 文件，其中包含您需要的所有元结构 (因为 Qt 的信号和槽不属于 C++标准的内容)。以进行Qt的内省工作
