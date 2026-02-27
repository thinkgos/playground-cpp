#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

void die(const char *msg)
{
    perror(msg);        // 打印你传入的字符串，后面自动跟上系统报错原因
    exit(EXIT_FAILURE); // 异常退出程序
}

void msg(const char *msg)
{
    printf("%s\n", msg);
}

static void do_something(int connfd)
{
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0)
    {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main(int argc, char const *argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // SO_REUSEADDR 允许地址重用

    // 绑定地址和端口
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr)); rv < 0)
    {
        die("bind()");
    }

    // 监听端口

    if (int rv = listen(fd, SOMAXCONN); rv < 0)
    {
        die("listen()");
    }

    while (true)
    {
        // 接受链接
        struct sockaddr_in client_add;
        socklen_t addrlen = sizeof(client_add);
        int connfd = accept(fd, (struct sockaddr *)&client_add, &addrlen);
        if (connfd < 0)
        {
            continue;
        }
        do_something(connfd);
        close(connfd);
    }

    return 0;
}
