#include <QTest>  // 单元测试必须包含，但是包含后会拖慢编译速度。因为他们包含了所有模块。注意不要写成 QtTest！

// add necessary includes here
#include <QLineEdit>
#include <QPushButton>

#include "../App/db_localdata.h"
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
};

PolyChatTester::PolyChatTester()
{

}

PolyChatTester::~PolyChatTester()
{

}

void PolyChatTester::initTestCase()
{

}

void PolyChatTester::cleanupTestCase()
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

/** 测试信号
 * @brief PolyChatTester::testSingal
 */
void PolyChatTester::testSingal()
{

}










//QTEST_APPLESS_MAIN(PolyChatTester)
QTEST_MAIN(PolyChatTester)  // 自动为我们创建用于测试的 main 函

#include "tst_PolyChatTester.moc"  // 编译或者生成一个额外的 cpp 文件，其中包含您需要的所有元结构 (因为 Qt 的信号和槽不属于 C++标准的内容)。以进行Qt的内省工作
