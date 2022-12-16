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





