/* Sea Server */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <iostream>
#ifndef WIN32

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <dlfcn.h>

#else
#define HAVE_STRUCT_TIMESPEC 1
#include <winsock2.h>

#endif

#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <string>

#include <pthread.h>

#include <logger.h>

typedef unsigned int my_uint32_t;
#define uint32_t my_uint32_t

uint32_t game_port = 12501;  
const size_t NS = 1024; 
const size_t CNT = 128; 

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
map < int, string > clients; 

long int getNext()
{
    pthread_mutex_lock( &mutex );

    FILE * fp = fopen("game.id","r+");
    long int prev;
    fscanf(fp, "%ld", &prev);

    prev++;
    rewind(fp);
    fprintf(fp, "%ld", prev);

    fclose(fp);

    pthread_mutex_unlock( &mutex );
    return prev;
}

void PANIC (const char *msg)
{
    perror(msg);
}

bool writeToClient(int client, const char * mes, const int mc)
{

    int len = sizeof(uint32_t) + mc;
    uint32_t mescnt = htonl(mc);

    char * message = (char *) malloc(len);
    char *p = message;
    char *n = (char *) & mescnt;

    for(int i =0; i< sizeof(uint32_t); ++i)
        *p++ = *n++;

    for(int i=0; i< mc; ++i)
        *p++ = *mes++;

    ssize_t  rcount = send(client, message, len,0);
    write(1, message, len);
    free(message);

    printf(" LEN %d\n", len);
    qDebug(" LEN %d", len);
    return rcount == len;
}

void analyze(char *mes, int size, int client)
{
    mes += sizeof(uint32_t);
    size -= sizeof(uint32_t);

    map<int, string>::iterator it; 

    char * cmd = mes;
    char * p = mes;
    char * e = mes + size;

    while(p < e && *p != ' ') p++;
    if(p == e) return;
    *p='\0';

    if(!strcmp(cmd, "LOGIN"))
    {
        char *n = ++p;
        while(p < e && *p != ' ') p++;
        *p ='\0';

        for ( it=clients.begin() ; it != clients.end(); it++ )
        {
            string st  = string(n) + " is OnLine!";
            writeToClient((*it).first, st.c_str(), st.size());
        }

        pthread_mutex_lock( &mutex );
        clients[client] = n;
        pthread_mutex_unlock( &mutex );

        const char * welcome = "Welcome to the Sea Batle server!";
        writeToClient(client, welcome, strlen(welcome));

    }
    else if(!strcmp(cmd, "LIST"))
    {
    size_t mlen = 0;
    string users = "LIST\n";
    for ( it=clients.begin() ; it != clients.end(); it++ )
        if((*it).first != client)
            users += (*it).second + "\n";

    writeToClient(client, users.c_str(), users.size());
    }
    else if(!strcmp(cmd, "SEND"))
    {
        char *n = ++p;
        while(p < e && *p != ' ') p++;
        *p ='\0';

        bool ok = false;
        for ( it=clients.begin() ; it != clients.end(); it++ )
        {
            if((*it).second == n)
            {
                p++;
                writeToClient((*it).first, p, e-p);
                ok = true;
            }
        }
    if(!ok)
        {
            const char * out = "Пользователь не подключен к серверу.";
            writeToClient(client, out, strlen(out));
        }
    }
    else if(!strcmp(cmd, "WELCOME") || !strcmp(cmd, "ACCEPT") || !strcmp(cmd, "REJECT"))
    {
        char *n = ++p;
        while(p < e && *p != ' ') p++;
        *p ='\0';

        bool ok = false;
        int ic=-1;

        for ( it=clients.begin() ; it != clients.end(); it++ )
        {
            if((*it).second == n)
            {
                p++;
        string st  = string(cmd) + " " + clients[client];
                writeToClient((*it).first, st.c_str(), st.size());
                ic = (*it).first;
                ok = true;
            }
        }
    if(!ok)
        {
            const char * out = "Пользователь не подключен к серверу.";
            writeToClient(client, out, strlen(out));
        }
        else
        {
            if(!strcmp(cmd, "ACCEPT"))
            {
                long int next = getNext();
                char s[255];
                sprintf(s,"GAMEID %ld", next);
                writeToClient(client,s, strlen(s));
                writeToClient(ic, s, strlen(s));
            }
        }
    }
}


void * Child (void *arg)
{
    int client = *(int *) arg;
    qDebug("New client!");
    printf("New client!\n");
    map<int, string>::iterator it;

    char buffer[CNT]; 
    char data[NS]; 

    char * i = data; 
    char * e = data + NS;  

    ssize_t rcount;  
    uint32_t mescnt = 0; 

    while((rcount = recv(client, buffer, CNT, 0)) > 0) 
    {

        char * p = buffer;
        for(int q=0; q< rcount && i< e; q++) 
            *i++ = *p++;

        if(mescnt == 0) 
        {
            if(rcount >= sizeof(uint32_t)) 
            {
                uint32_t mc = * (uint32_t *) data; 
                mescnt = ntohl(mc); 
            }
        }
        if(mescnt > 0) 
        {
            char *s = data;
            while(true)
            {
                char * real_data = s + sizeof(uint32_t);

                int already_read = i - real_data;
                if(already_read == mescnt) 
                {
                    analyze(real_data, mescnt, client);
                    mescnt = 0;
                    i = data;
                    break;
                }
                else 
                {
                    if(already_read < mescnt) 
                        break;

                    analyze(real_data, mescnt, client);
                    s = real_data + mescnt;

                    if(i - s < sizeof(uint32_t)) 
                    {
                        int q =0;
                        for( ; s < i; ++q)
                            data[q] = *s++;

                        i = data + q;
                        mescnt = 0;
                        break;
                    }


                    uint32_t mc = * (uint32_t *) s; 
                    mescnt = ntohl(mc); 

                }
            }
        }

    }


    close (client);

    string user = clients[client];

    pthread_mutex_lock( &mutex );
    clients.erase(client);
    pthread_mutex_unlock( &mutex );

    for ( it=clients.begin() ; it != clients.end(); it++ )
    {
        string st  = user + " closed the session.";
        writeToClient((*it).first, st.c_str(), st.size());

    }
    qCritical("Client died!");
    printf("Client died!\n");
}

int main (int argc, char **argv)
{
    qInstallMessageHandler(logMessagesInFile);
    QCoreApplication app(argc, argv);

    int t_port;
    printf("Input port number: ");
    scanf("%d", &t_port);

    if(t_port > 0 && t_port < 65536)
    {
      game_port = t_port;
      qDebug("Started server on port with number %d", game_port);
      printf("Started server on port with number %d.", game_port);
    }
    else
    {
        qDebug("Illegal port. Started on 12501.");
        printf("Illegal port. Started on 12501.\n");
    }



#ifdef WIN32
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR) {
        printf("Error creating socket.");
        qFatal("Error creating socket");
    }

#endif

    int sd;
    struct sockaddr_in addr;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        PANIC ("Socket");
        qFatal("Socket");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons (game_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind (sd, (struct sockaddr *) &addr, sizeof (addr)) != 0)
    {
        PANIC ("Bind");
        qFatal("Bind");
    }
    if (listen (sd, 20) != 0)
    {
        PANIC ("Listen");
        qFatal("Listen");
    }

    while (1) 
    {
        int client, addr_size = sizeof (addr);
        client =
#ifdef WIN32
            accept (sd, (struct sockaddr *) &addr,  & addr_size);
#else
            accept (sd, (struct sockaddr *) &addr,  (socklen_t *)& addr_size);
#endif
        if(accept >=0)
        {
            pthread_t thread;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

            pthread_mutex_lock( &mutex );
            pthread_create(&thread, &attr, Child, &client);
            pthread_mutex_unlock( &mutex );
        }
    }

#ifdef WIN32
    WSACleanup();
#endif

    return 0;
}