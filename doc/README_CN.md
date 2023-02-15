<!-- SPbSTU  -->

 <p align="center">
  <img width="250px" src="https://github.com/NekoSilverFox/NekoSilverfox/blob/master/icons/logo_building_spbstu.png?raw=true" align="center" alt="ogo_building_spbstu" />
  </br>
  </br>
  <b><font size=4>Санкт-Петербургский государственный политехнический университет</font></b></br>
  <b>Институт компьютерных наук и технологий</b>

  <br>

 <p align="center"><b><font size=6>PolyChat</font></b></p>
 <p align="center"><b>教学聊天系统</b></p>

 <p align="center"></p>


<p align="center">
    <a href="../README.md">Руский язык</a>
</p>



<div align=left>
<!-- SPbSTU 最后一行 -->

<div align=center>
[![License](https://img.shields.io/badge/license-Apache%202.0-brightgreen)](LICENSE)

[![Build for MacOS](https://github.com/NekoSilverFox/PolyChat/actions/workflows/macos.yml/badge.svg)](https://github.com/NekoSilverFox/PolyChat/actions/workflows/macos.yml)
[![Build for Windows](https://github.com/NekoSilverFox/PolyChat/actions/workflows/windows.yml/badge.svg)](https://github.com/NekoSilverFox/PolyChat/actions/workflows/windows.yml)

</div>


[toc]


# 项目成员及规划

| 姓名            | 说明                                       |
| --------------- | ------------------------------------------ |
| Валерий Фофанов | 实现功能模块、编写文档                     |
| Ли Ицзя         | 实现功能模块、                             |
| Мэн Цзянин      | 窗体设计、代码框架搭建及组织、实现功能模块 |



# 需求

## 系统需求

现在理工大学的教学逐渐的从线上授课模式转换为线下授课模式。而在大学中的教室中授课时，经常会发生老师向学生下达任务或者发送文件时遇到各种问题。比如教室里的投影仪非常不清晰、后排同学无法看清黑板上写的字、老师在学校网络环境下向学生发送文件非常不方便、老师经常需要对不同学生对同一问题进行重复解答。

PolyChat 是一款跨平台即时通讯软件，旨在解决上述问题，提高老师和学生的工作效率。它有以下主要功能：

- 得益于局域网的封闭性，为了保证易用性和安全，用户只需要提供姓名和班级号码即可登录使用
- 用户可以快速组建和加入群聊，并在其中发送消息和文件
- 可编辑文本消息的格式
- 将消息内容保存为文件
- 清空聊天内容



## 技术性需求

有以下的主要的技术需求（要求）

- 为了保证跨平台性，将使用 Qt 进行开发
- 为了符合“高内聚，低耦合”思想，将采用三层架构的方式进行开发，把表示层（UI）、业务逻辑层（BLL）和数据访问层（DAL）分离
- 为了实现跨平性和可扩展性，将使用 Qt 进行开发。
- 文本消息传递采用 UDP 协议，而文件传输采用 TCP 协议。
- 当用户发送消息时，会在端口上进行广播。
- 在固定端口后绑定 UDP Socket，用信号与槽的方式进行监听是否有数据来临



# 功能规范

| 标识符                       | 解释                                                         |
| ---------------------------- | ------------------------------------------------------------ |
| Login_Deficient              | 如果姓名或者班级号有一个没有输入时，将显示一个通知，其中包括错误的文本 |
| Login_Minimize               | 当点击最小化按钮时，窗口会被最小化                           |
| Login_Maximize               | 当点击关闭按钮时，主程序退出                                 |
|                              |                                                              |
| DialogList_Load              | 当用户成功登陆，对话框列表（局域网中的群聊）会被调用，并且相应资源文件会被加载 |
| DialogList_Add_Chat          | 用户可以点击加号按钮，通过弹出的对话框指定群聊名称，之后该名称的群聊将会被创建。创建新群聊之后，群聊列表将会被刷新以增加新创建的群聊 |
| DialogList_Add_Chat_Clash    | 当某个名称的群聊已经存在时，如果尝试重复创建将会显示一个通知，其中包括错误信息 |
| DialogList_View              | 用户可以使用鼠标滚轮查看列表中的内容                         |
| DialogList_Select            | 用户可以在对话框列表中选择某个群聊                           |
| DialogList_Search            | 用户可以通过搜索框，通过群聊的名称搜索一个群聊               |
| DialogList_Join              | 当对话框列表中的某个群聊被点击(选中)时，此用户会加入群聊，并且会弹出聊天窗口 |
| DialogList_Join_Clash        | 当用户已经加入某个群聊时，如果再次加入会将显示一个通知，其中包括错误的文本 |
| DialogList_Minimize          | 当点击最小化按钮时，窗口会被最小化                           |
| DialogList_Maximize          | 当点击全屏按钮时，窗口会铺满整个屏幕                         |
| DialogList_Exit              | 按下关闭按钮，退出主程序                                     |
|                              |                                                              |
| ChatWidget_Text_Font_Type    | (选中一段文字)可以在下拉列表中修改字体，可修改的字体为电脑上已安装的。 |
| ChatWidget_Text_Font_Size    | (选中一段文字)可以在下拉列表中修改文字大小，字体大小从 12 到 28 |
| ChatWidget_Text_Font_B       | (选中一段文字)点击加粗按钮会使文字加粗                       |
| ChatWidget_Text_Font_I       | (选中一段文字)点击斜体按钮会使文字倾斜                       |
| ChatWidget_Text_Font_L       | (选中一段文字)点击下划线按钮会在文字底部添加下划线           |
| ChatWidget_Mess_View         | 用户可以使用鼠标滚轮查看聊天记录                             |
| ChatWidget_Mess_Clean        | 点击清空聊天记录按钮会清空自己窗口内的所有聊天记录，其他用户的窗口内容不受影响 |
| ChatWidget_Mess_Save         | 点击保存按钮可以将聊天记录保存为 txt 文件                    |
| ChatWidget_Mess_Send         | 点击发送按钮可以将输入框中的文字内容发送至群聊               |
| ChatWidget_Mess_Send_Empty   | 当文字输入框为空时，点击发送按钮无法发送任何内容             |
| ChatWidget_File_Send         | 用户可以选择一个文件，将其发送至群聊中                       |
| ChatWidget_File_Send_Large   | 如果文件大于 1Gb，那么将无法发送                             |
| ChatWidget_File_Receive      | 当用户发送文件后，其他用户可以选择是否接受此文件             |
| ChatWidget_File_Receive_Path | 如果用户选择接收文件，那么可以选择文件的保存位置             |
| ChatWidget_User_In           | 某为用户加入群聊时，所有用户都会收到消息                     |
| ChatWidget_User_Out          | 某位用户点击退出按钮会退出群聊，而其他用户不受影响           |
| ChatWidget_User_Out_Mess     | 某位用户退出群聊时，所有人都会收到此用户退出的消息           |
| ChatWidget_Minimize          | 当点击最小化按钮时，窗口会被最小化                           |
| ChatWidget_Maximize          | 当点击全屏按钮时，窗口会铺满整个屏幕                         |



# HLD



## 界面设计

### 登录

![widget_login](pic/README/widget_login.svg)

### 用户列表界面

<img src="./pic/README/chat_list.svg" alt="chat_list" style="zoom: 67%;" />

### 聊天窗口

<img src="./pic/README/widget_chatbox.svg" alt="chat_widget" style="zoom:67%;" />



## 软件架构

我们使用用例图来表述 PolyChat 的软件架构：

![architecture](pic/README/architecture.svg)

## 类图

PolyChat 中存在 3 个类，分别是群组列表、群组、用户类。他们的关系如下图所示：

![class](pic/README/class.svg)

## UDP 群聊部分与群组列表

本程序实现的功能是：局域网内，用户可以添加（创建）一个群聊，当一个群聊被创建时，其他主机上的群聊列表要同步更新（通过 UDP）。

每个用户加入到群聊后，则群聊界面的右端可以显示在线用户列表，分别显示的是用户名、班级号、ip地址。窗体左边是聊天内容显示界面，即群聊。每个人可以在聊天输入界面中输入文字（还可修改文字格式）并发送。

UDP 消息通过 `Broadcast` 机制在局域网中进行广播，并且使用了 `ShareAddress` 和 `ReuseAddressHint` 增强易用性，以下是它们的解释：

- `Broadcast`：在当前局域网中的所有端口进行广播
- `ShareAddress`：允许其他服务绑定到同一地址和端口。当多个进程通过监听同一地址和端口来分担一个服务的负载时，这很有用（例如，一个有多个预分叉监听器的Web服务器可以大大改善响应时间）。然而，由于任何服务都被允许重新绑定，这个选项要考虑到某些安全问题。所以通过把这个选项和 `ReuseAddressHint` 结合起来。
- `ReuseAddressHint`：即使地址和端口已经被另一个套接字绑定，它也应该尝试重新绑定该服务。

![ui_ChatList_ChatBox](pic/README/ui_ChatList_ChatBox.png)

### UDP 通讯协议

为了实现上述功能，并且保证程序的后期可拓展性。采用了定制的通讯协议，其中存在 8 个块：

- `第1块`：消息类型。目前有 6 种类型：
    - `ChatExist`: 当前群聊窗口存在，每隔一段时间广播一次
    - `ChatDestory`：删除当前群聊，当群聊中所有（最后一位）退用户出时发出
    - `Msg`：普通的文字消息
    - `File`：文件发送请求
    - `UserJoin`：用户加入当前群聊
    - `UserLeft`：用户离开当前群聊
- `第2块`：当前群聊的名字
- `第3块`：当前群聊位于的端口
- `第4块`：发送本消息的用户名
- `第5块`：发送本消息的用户所属班级
- `第6块`：发送本消息的用户 IP 地址
- `第7块`：消息内容（长度可变）
- `第8块`：消息内容（长度可变，用于拓展第 7 段）

如下图所示：

![udp-msg](pic/README/udp-msg.png)





### 服务器

**注意：在 PolyChat 中的聊天系统，类似于 P2P。一个用户既可以充当服务器又可以充当客户端。**

建立一个UDP Socket并绑定在固定端口后，用信号与槽的方式进行监听是否有数据来临。如果有，接收其数据并分析数据的消息类型，如果消息是新用户登录则更新用户列表并在聊天显示窗口中添加新用户上线通知；同理，如果是用户下线，则在用户列表中删除该用户且在聊天显示窗口中显示下线通知；如果是聊天消息，则接收该消息并且在窗口中显示。其流程图如下：

![udp_server](pic/README/udp_server.svg)

### 客户端

**注意：在 PolyChat 中的聊天系统，类似于 P2P。一个用户既可以充当服务器又可以充当客户端。**

首先当用户加入群聊时，获取用户名，班级号，主机名和ip地址，并广播给局域网的服务器（ChatBoxWidget）更新右侧的用户列表。然后当客户端需要发送信息时，则在聊天输入栏中输入信息并按发送键发送聊天内容，当然于此同时也广播本地系统的各种信息。其流程图如下：

![udp_client](pic/README/udp_client.svg)

## TCP文件传输部分

### TCP通讯协议

TCP 采用了定制的通讯协议，其中存在 4 个部分：

1~3 部分为文件头，第 4 部分为实际数据

- `第1部分`: 文件名
- `第2部分`: 文件大小（Byte）
- `第3部分`: 文件的 MD5
- `第4部分`: 数据（每个数据块的大小为 4Kb）

在传输中，服务端将首先发送文件头。为了防止 TCP 黏包，等待 20ms 后再开始发送数据块

![tcp_data](pic/README/tcp_data.png)

### 发送端

发送端，也即承担服务器角色的操作。

![ui_tcp_file_sender](pic/README/ui_tcp_file_sender.png)

点击文件发送按钮，在本地计算机中选择需要发送的文件，点击发送按钮，弹出发送端界面后，则进度条上会显示当前文件传送的信息，有已传送文件大小信息，传送速度等信息。如果想关闭发送过程，则单击关闭按钮。其流程图如下：

![tcp_sender](pic/README/tcp_sender.svg)

### 接收端

**接收端，也即承担客户端角色的操作。**

在界面中弹出一个对话框，问是否接自某个用户的文件传送信息，如果接受则单击 Yes 按钮，否则就单击 No 按钮。

![ui_tcp_file_getter_req](pic/README/ui_tcp_file_getter_req.png)

当接收文件时，选择好接收文件所存目录和文件名后就开始接收文件了，其过程也会显示接收文件的进度，接收端和客户端相关的信息。接收端的界面如下图所示：

![ui_tcp_file_getter](pic/README/ui_tcp_file_getter.png)

其流程图如下：

![tcp_getter](pic/README/tcp_getter.svg)



## 数据流

我们使用数据流图来表示 PolyChat 群聊窗口的信息接收处理过程：

![chat_port](pic/README/chat_port.svg)





# 测试

## 说明

将使用 QTest 对 PolyChat 进行测试。

QTestlib是Qt提供的一种针对基于Qt编写的程序或库的单元测试框架。QTestLib提供了单元测试框架的基本功能，并提供了针对GUI测试的扩展功能。以下是 QTestlib 的特性：

A、轻量级：QTestlib只包含6000行代码和60个导出符号
B、自包含：对于非GUI测试，QTestlib只需要Qt核心库的几个符号。
C、快速测试：QTestlib不需要特殊的测试执行程序，不需要为测试而进行特殊的注册。
D、数据驱动测试：一个测试程序可以在不同的测试数据集上执行多次。
E、基本的GUI测试：QTestlib提供了模拟鼠标和键盘事件的功能。
F、基准测试：QTestLIB支持基准测试并提供多种测量后端。
G、IDE友好：QTestlib的输出信息可以被Visual Studio和KDevelop解析。
H、线程安全：错误报告是线程安全的、原子性的。
J、类型安全：对模板进行了扩展使用，防止由隐式类型转换引起的错误。
K、易扩展：用户自定义类型可以容易地加入到测试数据和测试输出中。

测试必须在项目用生成可执行文件的构建器 qmake 构建时自动完成。在 Github 上使用了 CI/CD，触发器将会在代码更改时进行自动在 Windows 和 macOS 平台上进行构建和测试（同时在两个平台上进行测试是为了保证 PolyChat 的跨平台性）。

## 单元（模块）测试

|      | 测试名                           | 所属模块    | 说明（预期结果）                                             |
| ---- | -------------------------------- | ----------- | ------------------------------------------------------------ |
| 1    | ut_login_login_empty             | Login       | 登录失败（用户名不能为空）                                   |
| 2    | ut_login_group_empty             | Login       | 登录失败（班级号不能为空）                                   |
| 3    | ut_login_login_group_empty       | Login       | 登录失败（用户名和班级号不能为空）                           |
| 4    | ut_login_init_login              | Login       | 传入正确格式的登录名，本地信息被正确初始化                   |
| 5    | ut_login_init_group              | Login       | 传入正确格式的班级号，本地信息被正确初始化                   |
|      | ut_login_btnlogin                | Login       | 保证 btnlogin 是非可选按钮，且图标显示被禁用                 |
| 6    | ut_login_btnlogin_emit           | Login       | 用户每次点击登录按钮时，保证（按钮点击）信号正确触发，且为一次 |
|      | ut_login_btnInfo_emit            | Login       | 用户每次点击 btnInfo 时，保证（按钮点击）信号正确触发，且为一次 |
| 7    | ut_login_leUserName              | Login       | 模拟用户点击并且通过键盘输入，确保输入内容在 leUserName 输入框中正确显示 |
| 8    | ut_login_leUserGroupNumber       | Login       | 模拟用户点击并且通过键盘输入，确保输入内容在 leUserGroupNumber 输入框中正确显示 |
|      | ut_login_window                  | Login       | Login 窗口的大小被禁止缩放，并且为 400x250                   |
| 9    | ut_addchat_leNameNewChat         | Add Chat    | 模拟用户点击并且通过键盘输入，确保输入内容在 leNameNewChat 输入框中正确显示 |
| 10   | ut_addchat_btnCancelAddChat      | Add Chat    | 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次 |
| 11   | ut_addchat_btnAddChat            | Add Chat    | 用户每次点击添加按钮时，保证（按钮点击）信号正确触发，且为一次 |
| 12   | ut_chat_init                     | Chat(Class) | 正确初始化（使用构造函数）一个 Chat 对象                     |
| 13   | ut_chatlist_init                 | ChatList    | 正确初始化（使用构造函数）一个 ChatList 对象                 |
|      | ut_chatlist_btnNewChat           | ChatList    | 保证 btnNewChat 是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 14   | ut_chatlist_btnNewChat_emit      | ChatList    | 用户每次点击增加群聊按钮时，保证（按钮点击）信号正确触发，且为一次 |
| 15   | ut_chatlist_leSearch             | ChatList    | 模拟用户点击并且通过键盘输入，确保输入内容在 leSearch 搜索框中正确显示 |
| 16   | ut_chatlist_leSearch_change_emit | ChatList    | 当用户改变搜索框中的内容时，触发信号                         |
| 17   | ut_chatlist_lbName               | ChatList    | lbName 中正确显示本用户的用户名                              |
| 18   | ut_chatlist_lbGroupNumber        | ChatList    | lbGroupNumber 中正确显示本用户的班级号                       |
| 19   | ut_chatlist_lbIP                 | ChatList    | lbIP 中正确显示本用户的 ip 地址                              |
| 20   | ut_addchat_port_exist            | ChatList    | 返回 true（当前端口上已存在群聊）                            |
| 21   | ut_addchat_port_not_exist        | ChatList    | 返回 false（当前端口上不存在群聊）                           |
| 22   | ut_addchat_chat_exist            | ChatList    | 返回 true（已存在同名的群聊）                                |
| 23   | ut_addchat_chat_not_exist        | ChatList    | 返回 false（不存在同名的群聊）                               |
| 24   | ut_chatlist_chat_not_open        | ChatList    | 返回 true（如果某个名称的群聊没有被打开）                    |
| 25   | ut_chatlist_chat_open            | ChatList    | 返回 false（如果某个名称的群聊被打开了）                     |
| 26   | ut_tcpclient_lbClientIP          | TcpClient   | lbClientIP 内容正确解析且显示                                |
| 27   | ut_tcpclient_lbClientPort        | TcpClient   | lbClientPort 内容正确解析且显示                              |
| 28   | ut_tcpclient_lbServerIP          | TcpClient   | lbServerIP 内容正确解析且显示                                |
| 29   | ut_tcpclient_lbServerPort        | TcpClient   | lbServerPort 内容正确解析且显示                              |
| 30   | ut_tcpclient_lbFileName          | TcpClient   | lbFileName 内容正确解析且显示                                |
| 31   | ut_tcpclient_lbFileSize          | TcpClient   | lbFileSize 内容正确解析且显示                                |
| 32   | ut_tcpclient_progressBar         | TcpClient   | progressBar 初始化时为 0                                     |
|      | ut_tcpclient_btnCancel           | TcpClient   | 保证 btnCancel 是非可选按钮                                  |
| 33   | ut_tcpclient_btnCancel_emit      | TcpClient   | 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次 |
|      | ut_tcpclient_btnSave             | TcpClient   | 保证 btnSave 是非可选按钮                                    |
| 34   | ut_tcpclient_btnSave_emit        | TcpClient   | 用户每次点击保存按钮时，保证（按钮点击）信号正确触发，且为一次 |
| 35   | ut_tcpclient_closeEvent_emit     | TcpClient   | 用户关闭窗口时触发关闭事件                                   |
|      | ut_tcpclient_textBrowser         | TcpClient   | textBrowser 为只读状态，禁止写入                             |
| 36   | ut_tcpserver_lbClientIP          | TcpServer   | lbClientIP 内容正确解析且显示                                |
| 37   | ut_tcpserver_lbClientPort        | TcpServer   | lbClientPort 内容正确解析且显示                              |
| 22   | ut_tcpserver_lbServerIP          | TcpServer   | lbServerIP 内容正确解析且显示                                |
| 38   | ut_tcpserver_lbServerPort        | TcpServer   | lbServerPort 内容正确解析且显示                              |
| 39   | ut_tcpserver_lbFilePath          | TcpServer   | lbFilePath 内容正确解析且显示                                |
| 40   | ut_tcpserver_lbFileSize          | TcpServer   | lbFileSize 内容正确解析且显示                                |
|      | ut_tcpserver_btnCancel           | TcpServer   | 保证 btnCancel 是非可选按钮                                  |
| 41   | ut_tcpserver_btnCancel_emit      | TcpServer   | 用户每次点击取消按钮时，保证（按钮点击）信号正确触发，且为一次 |
| 42   | ut_tcpserver_progressBar         | TcpServer   | progressBar 初始化时为 0                                     |
| 43   | ut_tcpserver_closeEvent_emit     | TcpServer   | 用户关闭窗口时触发关闭事件                                   |
|      | ut_tcpserver_textBrowser         | TcpServer   | textBrowser 为只读状态，禁止写入                             |
| 44   | ut_chatbox_title                 | ChatBox     | ChatBox 窗口的标题正确初始化（符合格式）                     |
| 45   | ut_chatbox_btnBold               | ChatBox     | 保证 btnBold 是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 46   | ut_chatbox_btnItalic             | ChatBox     | 保证 btnItalic 是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 47   | ut_chatbox_btnUnderLine          | ChatBox     | 保证 btnUnderLine 是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 48   | ut_chatbox_btnColor              | ChatBox     | 保证 btnColor 不是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 49   | ut_chatbox_btnFileSend           | ChatBox     | 保证 btnFileSend 不是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 50   | ut_chatbox_btnSave               | ChatBox     | 保证 btnSave 不是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 51   | ut_chatbox_btnClean              | ChatBox     | 保证 btnClean 不是可选按钮（Checkable-QToolButton），且文字显示被禁用 |
| 52   | ut_chatbox_btnExit               | ChatBox     | 保证 btnExit 不是可选按钮（Checkable）                       |
| 53   | ut_chatbox_btnSend               | ChatBox     | 保证 btnSend 不是可选按钮（Checkable）                       |
| 54   | ut_chatbox_btnBold_emit          | ChatBox     | 点击 btnBold 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态 |
| 55   | ut_chatbox_btnItalic_emit        | ChatBox     | 点击 btnItalic 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态 |
| 56   | ut_chatbox_btnUnderLine_emit     | ChatBox     | 点击 btnUnderLine 触发（按钮点击）信号，且状态改变为 enable；再次点击恢复为初始状态 |
| 57   | ut_chatbox_btnColor_emit         | ChatBox     | 点击 btnColor 触发（按钮点击）信号                           |
| 58   | ut_chatbox_btnFileSend_emit      | ChatBox     | 点击 btnFileSend 触发（按钮点击）信号                        |
| 59   | ut_chatbox_btnSave_emit          | ChatBox     | 点击 btnSave 触发（按钮点击）信号                            |
| 60   | ut_chatbox_btnClean_emit         | ChatBox     | 点击 btnClean 触发（按钮点击）信号                           |
| 61   | ut_chatbox_btnExit_emit          | ChatBox     | 点击 btnExit 触发（按钮点击）信号                            |
| 62   | ut_chatbox_btnSend_emit          | ChatBox     | 点击 btnSend 触发（按钮点击）信号                            |
| 63   | ut_chatbox_msgTextBrowser        | ChatBox     | msgTextBrowser 为只读状态                                    |
| 64   | ut_chatbox_msgTextEdit           | ChatBox     | msgTextEdit 允许写入                                         |
| 65   | ut_chatbox_init_cbxFontSize      | ChatBox     | 初始字体为 12                                                |
| 67   | ut_chatbox_cbxFontSize_min_max   | ChatBox     | 最小字体为 10，最大为 28                                     |
| 68   | ut_chatbox_closeEvent_emit       | ChatBox     | 用户关闭窗口（退出）时触发关闭事件                           |
|      | ut_chatbox_tbUser                | ChatBox     | 用户列表的宽度为 150-350                                     |
|      | ut_chatbox_lbNumberOnlineUse     | ChatBox     | 在线人数的 UI 初始化为正确格式                               |

## 集成（接口）测试

|      | 测试名                             | 所属模块 | 说明（预期结果）                                       |
| ---- | ---------------------------------- | -------- | ------------------------------------------------------ |
|      | mt_login_init_succ                 | Login    | 登陆成功，本地用户信息被正确初始化                     |
|      | mt_chatlist_getNewBtn              | ChatList | 根据传入的参数，所创建的新按钮对象中拥有正确的信息     |
|      | mt_chatlist_getRandomPort          | ChatList | 生成的随机端口介于 PORT_MIN 与 PORT_MAX                |
|      | mt_chatlist_setChatState           | ChatList | 正常设置聊天窗口为打开或者关闭                         |
|      | mt_chatbox_userjoin_list           | ChatBox  | 用户加入（用户列表正确刷新）                           |
|      | mt_chatbox_userjoin_conter         | ChatBox  | 用户加入（计数器正确增加，并且ui的显示格式及内容正确） |
|      | mt_chatbox_userjoin_msgTextBrowser | ChatBox  | 用户加入（msgTextBrowser 被刷新）                      |
|      | mt_chatbox_userleft_list           | ChatBox  | 用户离开（用户列表正确刷新）                           |
|      | mt_chatbox_userleft_conter         | ChatBox  | 用户离开（计数器正确增加，并且ui的显示格式及内容正确） |
|      | mt_chatbox_userleft_msgTextBrowser | ChatBox  | 用户离开（msgTextBrowser 被刷新）                      |

## 系统/端到端测试



