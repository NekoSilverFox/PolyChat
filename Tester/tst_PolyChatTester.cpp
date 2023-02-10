#include <QTest>  // 单元测试必须包含，但是包含后会拖慢编译速度。因为他们包含了所有模块。注意不要写成 QtTest！

// add necessary includes here
#include <QLineEdit>
#include <QPushButton>

#include "../App/db_localdata.h"
#include "../App/bll_polychat.h"
#include "../App/dal_polychat.h"
#include "../App/uil_addchat.h"
#include "../App/uil_chatboxwidget.h"
#include "../App/uil_chatlist.h"
#include "../App/tcpclient.h"
#include "../App/tcpserver.h"


QString      localUserName           = "";               // User Name (get in user login)
QString      localUserGroupNumber    = "";               // Group number (get in user login)
QHostAddress localIpAddress          = QHostAddress();
ChatList*    chatList                = nullptr;          // Widget ChatList (Only one)

class PolyChatTester : public QObject
{
    Q_OBJECT

public:
    PolyChatTester();
    ~PolyChatTester();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_case1();
    void testGui();
    void testSingal();

    void ut_login_login_empty            ();
    void ut_login_group_empty            ();
    void ut_login_login_group_empty      ();
    void ut_login_init_login             ();
    void ut_login_init_group             ();
    void ut_login_btnlogin_emit          ();
    void ut_login_leUserName             ();
    void ut_login_leUserGroupNumber      ();
    void ut_addchat_leNameNewChat        ();
    void ut_addchat_btnCancelAddChat     ();
    void ut_addchat_btnAddChat           ();
    void ut_chat_init                    ();
    void ut_chatlist_btnNewChat_emit     ();
    void ut_chatlist_leSearch            ();
    void ut_chatlist_leSearch_change_emit();
    void ut_chatlist_lbName              ();
    void ut_chatlist_lbGroupNumber       ();
    void ut_chatlist_lbIP                ();
    void ut_addchat_port_exist           ();
    void ut_addchat_port_not_exist       ();
    void ut_addchat_chat_exist           ();
    void ut_addchat_chat_not_exist       ();
    void ut_chatlist_chat_not_open       ();
    void ut_chatlist_chat_open           ();
    void ut_tcpclient_lbClientIP         ();
    void ut_tcpclient_lbClientPort       ();
    void ut_tcpclient_lbServerIP         ();
    void ut_tcpclient_lbServerPort       ();
    void ut_tcpclient_lbFileName         ();
    void ut_tcpclient_lbFileSize         ();
    void ut_tcpclient_progressBar        ();
    void ut_tcpclient_btnCancel_emit     ();
    void ut_tcpclient_btnSave_emit       ();
    void ut_tcclient_closeEvent_emit     ();
    void ut_tcpserver_lbClientIP         ();
    void ut_tcpserver_lbClientPort       ();
    void ut_tcpserver_lbServerIP         ();
    void ut_tcpserver_lbServerPort       ();
    void ut_tcpserver_lbFilePath         ();
    void ut_tcpserver_lbFileSize         ();
    void ut_tcpserver_btnCancel_emit     ();
    void ut_tcpserver_progressBar        ();
    void ut_tcpserver_closeEvent_emit    ();
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
    void mt_login_init_succ              ();
    void mt_chatlist_getNewBtn           ();
    void mt_chatlist_getRandomPort       ();
    void mt_chatlist_setChatState        ();
    void mt_chatbox_btnBold              ();
    void mt_chatbox_btnItalic            ();
    void mt_chatbox_btnUnderLine         ();
};

PolyChatTester::PolyChatTester() { }

PolyChatTester::~PolyChatTester() { }

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
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_login_init_login()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("Fox", "3530904/90102");
    QCOMPARE(localUserName, "Fox");
}

/** 传入正确格式的班级号，本地信息被正确初始化
 *  Локальная информация инициализируется правильно путем передачи номера группы в правильном формате.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_login_init_group()
{
    bool isSuccInitLocalUser = DAL::initLocalUser("Fox", "3530904/90102");
    QCOMPARE(localUserGroupNumber, "3530904/90102");
}

/** 用户每次点击登录按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Вход в систему. Сигнал (нажатие кнопки) входа в систему каждый раз срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_login_btnlogin_emit()
{

}

/** 模拟用户点击并且通过键盘输入，确保输入内容在 leUserName 输入框中正确显示
 *  Имитация щелчка пользователя и ввода с клавиатуры, чтобы убедиться, что вводимый контент правильно отображается в поле ввода leUserName.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_login_leUserName()
{

}

/** 模拟用户点击并且通过键盘输入，确保输入内容在 leUserGroupNumber 输入框中正确显示
 *  Имитация щелчка пользователя и ввода с клавиатуры, чтобы убедиться, что вводимый контент правильно отображается в поле ввода leUserGroupNumber.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_login_leUserGroupNumber()
{

}

/** 模拟用户点击并且通过键盘输入，确保输入内容在 leNameNewChat 输入框中正确显示
 *  Имитация щелчка пользователя и ввода с клавиатуры, чтобы убедиться, что вводимое содержимое правильно отображается в поле ввода leNameNewChat.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_leNameNewChat()
{

}

/** 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку отмены, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_btnCancelAddChat()
{

}

/** 用户每次点击添加按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку добавления, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_btnAddChat()
{

}

/** 正确初始化（使用构造函数）一个 Chat 对象
 *  Правильно инициализируется (используя конструктор) объект чата. Смоделируйте, как пользователь нажимает и печатает на клавиатуре, чтобы убедитьс
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chat_init()
{

}

/** 用户每次点击增加群聊按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку, чтобы добавить групповой чат, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_btnNewChat_emit()
{

}

/** 模拟用户点击并且通过键盘输入，确保输入内容在 leSearch 搜索框中正确显示
 *  Смоделируйте, как пользователь нажимает и печатает на клавиатуре, чтобы убедиться, что вводимое содержимое правильно отображается в поле поиска
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_leSearch()
{

}

/** 当用户改变搜索框中的内容时，触发信号
 *  Инициировать сигнал, когда пользователь меняет содержимое в поле поиска.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_leSearch_change_emit()
{

}

/** lbName 中正确显示本用户的用户名
 *  Имя пользователя этого пользователя правильно отображается в lbName.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_lbName()
{

}

/** lbGroupNumber 中正确显示本用户的班级号
 *  Номер группы этого пользователя правильно отображается в lbGroupNumber.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_lbGroupNumber()
{

}

/** lbIP 中正确显示本用户的 ip 地址
 *  IP-адрес этого пользователя корректно отображается в lbIP.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_lbIP()
{

}

/** 返回 false（当前端口上已存在群聊）
 *  Возвращает false (групповой чат уже существует на текущем порту).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_port_exist()
{

}

/** 返回 true（当前端口上不存在群聊）
 *  Возвращает true (групповой чат не существует на текущем порту).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_port_not_exist()
{

}

/** 返回 false（已存在同名的群聊）
 *  Возвращает false (групповой чат с таким названием уже существует).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_chat_exist()
{

}

/** 返回 true（不存在同名的群聊）
 *  Возвращает true (групповой чат с таким названием не существует).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_addchat_chat_not_exist()
{

}

/** 返回 false（如果某个名称的群聊没有被打开）
 *  Возвращает false (если групповой чат с таким названием не открыт).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_chat_not_open()
{

}

/** 返回 true（如果某个名称的群聊被打开了）
 *  Возвращает true (если открыт групповой чат с таким названием).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatlist_chat_open()
{

}

/** lbClientIP 内容正确解析且显示
 *  Содержимое lbClientIP анализируется правильно и отображается
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_lbClientIP()
{

}

/** lbClientPort 内容正确解析且显示
 *  Содержимое lbClientPort правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_lbClientPort()
{

}

/** lbServerIP 内容正确解析且显示
 *  Содержимое lbServerIP правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_lbServerIP()
{

}

/** lbServerPort 内容正确解析且显示
 *  Содержимое lbServerPort правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_lbServerPort()
{

}

/** lbFileName 内容正确解析且显示
 *  Содержимое lbFileName правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_lbFileName()
{

}

/** lbFileSize 内容正确解析且显示
 *  Содержимое lbFileSize правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_lbFileSize()
{

}

/** progressBar 初始化时为 0
 *  progressBar инициализирован 0.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_progressBar()
{

}

/** 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку отмены, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_btnCancel_emit()
{

}

/** 用户每次点击保存按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку сохранения, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpclient_btnSave_emit()
{

}

/** 用户关闭窗口时触发关闭事件
 *  Запускает событие закрытия, когда пользователь закрывает окно.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcclient_closeEvent_emit()
{

}

/** lbClientIP 内容正确解析且显示
 *  Содержимое lbClientIP правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_lbClientIP()
{

}

/** lbClientPort 内容正确解析且显示
 *  Содержимое lbClientPort правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_lbClientPort()
{

}

/** lbServerIP 内容正确解析且显示
 *  Содержимое lbServerIP правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_lbServerIP()
{

}

/** lbServerPort 内容正确解析且显示
 *  Содержимое lbServerPort правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_lbServerPort()
{

}

/** lbFilePath 内容正确解析且显示
 *  Содержимое lbFilePath правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_lbFilePath()
{

}

/** lbFileSize 内容正确解析且显示
 *  Содержимое lbFileSize правильно анализируется и отображается.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_lbFileSize()
{

}

/** 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次
 *  Каждый раз, когда пользователь нажимает кнопку отмены, сигнал (нажатие кнопки) срабатывает правильно и единожды.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_btnCancel_emit()
{

}

/** progressBar 初始化时为 0
 *  progressBar инициализирован 0.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_progressBar()
{

}

/** 用户关闭窗口时触发关闭事件
 *  Запускает событие закрытия, когда пользователь закрывает окно.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_tcpserver_closeEvent_emit()
{

}

/** ChatBox 窗口的标题正确初始化（符合格式）
 *  Заголовок окна ChatBox инициализирован корректно (соответствует формату).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_title()
{

}

/** 保证 btnBold 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnBold является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnBold()
{

}

/** 保证 btnItalic 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnItalic является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnItalic()
{

}

/** 保证 btnUnderLine 是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnUnderLine является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnUnderLine()
{

}

/** 保证 btnColor 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnColor не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnColor()
{

}

/** 保证 btnFileSend 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnFileSend не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnFileSend()
{

}

/** 保证 btnSave 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnSave не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnSave()
{

}

/** 保证 btnClean 不是可选按钮（Checkable-QToolButton），且文字显示被禁用
 *  btnClean не является отмечаемой кнопкой (Checkable-QToolButton), а отображение текста отключено.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnClean()
{

}

/** 保证 btnExit 不是可选按钮（Checkable）
 *  Кнопка btnExit не является проверяемой.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnExit()
{

}

/** 保证 btnSend 不是可选按钮（Checkable）
 *  Кнопка btnSend не является проверяемой.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnSend()
{

}

/** 点击 btnBold 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态
 *  Нажатие btnBold активирует сигнал (нажатие кнопки), и состояние изменяется на «включено»; нажатие еще раз возвращает в исходное состояние.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnBold_emit()
{

}

/** 点击 btnItalic 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态
 *  Нажатие btnItalic активирует сигнал (нажатие кнопки), и состояние изменяется на «включено»; нажатие еще раз возвращает в исходное состояние.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnItalic_emit()
{

}

/** 点击 btnUnderLine 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态
 *  Нажатие btnUnderLine активирует сигнал (нажатие кнопки), и состояние изменяется на «включено»; нажатие еще раз возвращает в исходное состояние.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnUnderLine_emit()
{

}

/** 点击 btnColor 触发（按钮点击）信号
 *  Нажатие btnColor запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnColor_emit()
{

}

/** 点击 btnFileSend 触发（按钮点击）信号
 *  Нажатие btnFileSend запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnFileSend_emit()
{

}

/** 点击 btnSave 触发（按钮点击）信号
 *  Нажатие кнопки btnSave запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnSave_emit()
{

}

/** 点击 btnClean 触发（按钮点击）信号
 *  Нажатие кнопки btnClean запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnClean_emit()
{

}

/** 点击 btnExit 触发（按钮点击）信号
 *  Нажатие кнопки btnExit запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnExit_emit()
{

}

/** 点击 btnSend 触发（按钮点击）信号
 *  Нажатие кнопки btnSend запускает сигнал (нажатие кнопки).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_btnSend_emit()
{

}

/** msgTextBrowser 为只读状态
 *  msgTextBrowser доступен только для чтения.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_msgTextBrowser()
{

}

/** msgTextEdit 允许写入
 *  msgTextEdit позволяет писать.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_msgTextEdit()
{

}

/** 初始字体为 12
 *  Начальный шрифт 12.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_init_cbxFontSize()
{

}

/** 最小字体为 10，最大为 28
 *  Минимальный размер шрифта 10, максимальный размер 28.
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_cbxFontSize_min_max()
{

}

/** 用户关闭窗口（退出）时触发关闭事件
 *  Событие close срабатывает, когда пользователь закрывает окно (выход).
 *  @brief PolyChatTester::
 */
void PolyChatTester::ut_chatbox_closeEvent_emit()
{

}

/** 登陆成功，本地用户信息被正确初始化
 *  Вход выполнен успешно, и информация о локальном пользователе правильно инициализирована.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_login_init_succ()
{

}

/** 根据传入的参数，所创建的新按钮对象中拥有正确的信息
 *  В соответствии с переданными параметрами создается новый объект кнопки с правильной информацией.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatlist_getNewBtn()
{

}

/** 生成的随机端口介于 PORT_MIN 与 PORT_MAX
 *  Генерирует случайные порты между PORT_MIN и PORT_MAX.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatlist_getRandomPort()
{

}

/** 正常设置聊天窗口为打开或者关闭
 *  Окно чата открыто или закрыто.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatlist_setChatState()
{

}

/** 当 btnBold 为选中状态时，用户输入文字被加粗
 *  Когда выбрано btnBold, текст, введенный пользователем, выделяется полужирным шрифтом.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatbox_btnBold()
{

}

/** 当 btnItalic 为选中状态时，用户输入文字被倾斜
 *  Когда выбрано btnItalic, текст, введенный пользователем, выделяется курсивом.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatbox_btnItalic()
{

}

/** 当 btnUnderLine 为选中状态时，用户输入文字被添加下划线
 *  Когда выбрано btnUnderLine, текст, введенный пользователем, будет подчеркнут.
 *  @brief PolyChatTester::
 */
void PolyChatTester::mt_chatbox_btnUnderLine()
{

}



void PolyChatTester::test_case1()
{
    QCOMPARE("冰糖雪狸", "冰糖雪狸");
}

/** 测试 GUI（包含了获取 private 成员的方法）
 * @brief PolyChatTester::testGui
 */
void PolyChatTester::testGui()
{

}

/**
 * @brief PolyChatTester::testSingal
 */
void PolyChatTester::testSingal()
{

}









//QTEST_APPLESS_MAIN(PolyChatTester)
QTEST_MAIN(PolyChatTester)  // 自动为我们创建用于测试的 main 函

#include "tst_PolyChatTester.moc"  // 编译或者生成一个额外的 cpp 文件，其中包含您需要的所有元结构 (因为 Qt 的信号和槽不属于 C++标准的内容)。以进行Qt的内省工作
