#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

void show_Error() {
    printf("something has wrong please check\n");
}

int main(int argc, char* argv[]){
    std::string to_email(argv[1]);
    std::string auth_code(argv[2]);
    //开启网络权限
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    
    //指定服务器IP地址和端口号
    sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(25);
	addr.sin_addr.S_un.S_addr = inet_addr("220.181.15.161");

    //连接SMTP服务器
    int ret = connect(sock, (sockaddr*)&addr, sizeof(sockaddr_in));
    
    //\r\n表示语句结束，需要加上
    char buffer[1024] = "";
    ret = recv(sock, buffer, 1024, 0);
    
    std::string msg = "HELO MSG!\r\n";
    ret = send(sock, msg.c_str(), msg.length(), 0);
    
    memset(buffer, 0, sizeof(buffer));
    ret = recv(sock, buffer, sizeof(buffer), 0);
    

    //认证登录
    msg = "AUTH LOGIN\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);

    //账号密码为经过base64加密后的账号和smtp授权码
    //发送经过加密后的账号
    msg = "MTg4MDM4OTc5NjJAMTYzLmNvbQ==\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    //发送经过加密后的授权码
    msg = "RE5CV1ZTTUVCTlVSS0ZWQg==\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    //发送发件人邮箱
    msg = "MAIL FROM:<18803897962@163.com>\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    //发送收件人邮箱
    msg = "RCPT TO:<"+to_email+">\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    //发送'DATA\R\N' 表明要开始发送数据
    msg = "DATA\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    //发送邮件正文 正文格式要求为："FROM:发件人\r\nSubject:标题\r\n\r\n正文\r\n\r\n\r\n.\r\n"
    //QQ采用了不同的邮件协议，因此需要对不同类型的邮箱进行判别

    for (size_t i = 0; i < to_email.size();i++) {
        if (to_email[i] == '@') {
            if (to_email.substr(i + 1, 2) == "qq") {
                msg = "FROM:Admin<xxx@163.com>\r\nSubject:XJTU二手交易平台\r\n\r\n【XJTU二手交易平台】验证码：";
				msg += auth_code + "。请勿将验证码提供给他人，如非本人操作，请忽略。\r\n\r\n\r\n.\r\n";
            }
            else {
                msg = "FROM:Admin\r\nSubject:XJTU二手交易平台\r\n\r\n【XJTU二手交易平台】验证码：";
				msg += auth_code + "。请勿将验证码提供给他人，如非本人操作，请忽略。\r\n\r\n\r\n.\r\n";
            }
        }
    }
    send(sock, msg.c_str(), msg.length(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    return 0;
}

