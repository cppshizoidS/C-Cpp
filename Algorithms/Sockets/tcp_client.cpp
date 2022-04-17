#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>

enum RESULT {
    NO_ERROR = 0,
    SOCKET,
    BIND,
    ACCEPT
};


inline RESULT init_client(int* sock, const int host)
{
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock < 0)
    {
        perror("socket");
        return SOCKET;
    }
     
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(host);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (connect(*sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return BIND;
    }
    
    const char mess[] = "connect!\n";
    send(*sock, mess, sizeof(mess), 0);
    
    
    return NO_ERROR;
}


inline RESULT connection(int* sock)
{
    const size_t BUF_LEN = 1024;
    char buf[BUF_LEN] = {0};
    const char STOP_CHR = '#';
    
    do
    {
        while(recv(*sock, buf, BUF_LEN, 0) <= 0);
        printf(">>%s", buf);
        if (buf[0] == '#')
            break;
        
        printf("<<");
        memset(buf, 0, BUF_LEN);
        fgets(buf, BUF_LEN, stdin);
        send(*sock, buf, BUF_LEN, 0);
    } while(buf[0] != STOP_CHR);
    
    return NO_ERROR;
}


int main(int argc, char** argv)
{
    RESULT result;
    const int host = 5558;
    int sock;
    if ((result = init_client(&sock, host)) != NO_ERROR)
        return result;
        
    if ((result = connection(&sock)) != NO_ERROR)
        return result;
    
    close(sock);

    return 0;
}
