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


inline RESULT init_server(int* listener, const int host)
{ 
    *listener = socket(AF_INET, SOCK_STREAM, 0);
    if (*listener < 0)
	{
        perror("socket");
        return SOCKET;
    }
    
   
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(host);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(*listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
        perror("bind");
        return BIND;
    }
    
    listen(*listener, 1);
    
    return NO_ERROR;
}


inline RESULT connection(int* listener)
{
    const size_t BUF_LEN = 1024;
    char buf[BUF_LEN] = {0};
    const char STOP_CHR = '#';
    
    do
    {
        int sock = accept(*listener, NULL, NULL);
        if (sock < 0)
	    { 
            perror("accept");
            return ACCEPT;
        }

        while (1)
	    {
            if (recv(sock, buf, BUF_LEN, 0) <= 0)
                break;
        
            printf(">>%s", buf);
    
            if (buf[0] == STOP_CHR)
                break;
            
            printf("<<");
            memset(buf, 0, BUF_LEN);
            fgets(buf, BUF_LEN, stdin);
            send(sock, buf, BUF_LEN, 0);
        }
        close(sock);
    } while (buf[0] != STOP_CHR);
    
    return NO_ERROR;
}


int main(int argc, char** argv)
{
    RESULT result;
    const int host = 5558;
    int listener;
    if ((result = init_server(&listener, host)) != NO_ERROR)
        return result;
    
    if ((result = connection(&listener)) != NO_ERROR)
        return result;
    
    close(listener);
    
    return result;
}
