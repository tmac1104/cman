#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <malloc.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/times.h>
#include <sys/uio.h>
#include <sys/file.h>
#include <sys/utsname.h>
#include <sys/socket.h>

#define		SENDBUF_SIZE		512
#define		RCVBUF_SIZE		4200000     

#define	LOG	printf
void	_err_head( char * sFileName, int iLine );
#define	LOGERR _err_head( __FILE__, __LINE__ ); printf
#define max( x, y ) x>=y?x:y

#define PF_FALSE    0
typedef char * PSTR;

#define SIZE_UNIT ( 1024*1024 )
#define MAX_FILE_LEN 128

static char*	g_szReplyBuf = NULL;



void  GetCommonCurrentDate(char *sCurrentDate)
{  
   time_t current;
   struct tm *tmCurrentTime;
   
   time(&current);
   tmCurrentTime = localtime(&current);
   sprintf(sCurrentDate, "%4d%02d%02d", tmCurrentTime->tm_year + 1900, 
           tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday);

}

char *GetCurTime()
{
	time_t tt;
	time(&tt);
	return((char *)ctime(&tt));
}
void GetCommonCurrentTime(char *sCurrentTime)
{
   time_t current;
   struct tm *tmCurrentTime;

   tzset();
   time(&current);
   tmCurrentTime = localtime(&current);
   sprintf(sCurrentTime, "%4d%02d%02d%02d%02d%02d",
				 tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
				 tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
           tmCurrentTime->tm_min, tmCurrentTime->tm_sec);
}

void CommonGetCurrentDate(char *sCurrentDate)
{
   time_t current;
   struct tm *tmCurrentTime;

   time(&current);
   tmCurrentTime = localtime(&current);
   sprintf(sCurrentDate, "%4d%02d%02d", tmCurrentTime->tm_year + 1900,
           tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday);
}
void APISendLog(char *apilogfile, char *fmt, ...)
{
	va_list ap;
	FILE 	*fp = NULL;
	char 	filename[MAX_FILE_LEN+1], bakfilename[MAX_FILE_LEN+1];
	int  	nRet;
	char	*ptr = NULL;
	struct 	stat statbuf;
	time_t  systime;
	struct  tm *ptime;
	char    s[128];
	long	base_overflow;
	char	szDateTime[15];
	char	szLogSize[4];


	base_overflow = 5;

    systime=time(NULL);
    ptime=localtime(&systime);

    memset(s, 0, sizeof(s));
    strftime(s, sizeof(s), 	"%Y-%m-%d %H:%M:%S", ptime);

   	memset(szDateTime,0,sizeof(szDateTime));
    GetCommonCurrentTime( szDateTime );

    memset(filename, 0, sizeof(filename));
	sprintf(filename,"%s/%s.send", getenv("APILOG_PATH"),apilogfile);

    if ( ( fp = fopen(filename, "a+") ) == NULL )
    	{
			fp = stderr;
			/*HtLog( logfile, __FILE__, __LINE__,
				"filename=[%s]\n",filename);*/
			LOG( "filename=[%s]\n",filename );
		}
	else
	{
			LOG( "filename=[%s]\n",filename );
			memset(&statbuf, 0, sizeof(statbuf));
			nRet = stat(filename, &statbuf);
			if ( nRet == 0 && statbuf.st_size >= SIZE_UNIT * base_overflow )
			{
				fclose(fp);
				memset(bakfilename, 0, sizeof(bakfilename));
				sprintf(bakfilename, "%s.%10.10s", filename,szDateTime+2);
				rename(filename, bakfilename);
      			if ( ( fp = fopen(filename, "a+") ) == NULL )
					fp = stderr;
			}
   	}

	
	fprintf(fp, "[%s] ", s);

	va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    
    fprintf(fp, "\n", s);
    fprintf(fp, "\n", s);
    
	fflush(fp);

    if ( fp != stderr )
		fclose(fp);

	return ;
}
void APIRecvLog(char *apilogfile, char *fmt, ...)
{
	va_list ap;
	FILE 	*fp = NULL;
	char 	filename[MAX_FILE_LEN+1], bakfilename[MAX_FILE_LEN+1];
	int  	nRet;
	char	*ptr = NULL;
	struct 	stat statbuf;
	time_t  systime;
	struct  tm *ptime;
	char    s[128];
	long	base_overflow;
	char	szDateTime[15];
	char	szLogSize[4];


	base_overflow = 5;

    systime=time(NULL);
    ptime=localtime(&systime);

    memset(s, 0, sizeof(s));
    strftime(s, sizeof(s), 	"%Y-%m-%d %H:%M:%S", ptime);

   	memset(szDateTime,0,sizeof(szDateTime));
    GetCommonCurrentTime( szDateTime );

    memset(filename, 0, sizeof(filename));
	sprintf(filename,"%s/%s.recv", getenv("APILOG_PATH"),apilogfile);

    if ( ( fp = fopen(filename, "a+") ) == NULL )
    	{
			fp = stderr;
			/*HtLog( logfile, __FILE__, __LINE__,
				"filename=[%s]\n",filename);*/
			LOG( "filename=[%s]\n",filename );
		}
	else
	{
			LOG( "filename=[%s]\n",filename );
			memset(&statbuf, 0, sizeof(statbuf));
			nRet = stat(filename, &statbuf);
			if ( nRet == 0 && statbuf.st_size >= SIZE_UNIT * base_overflow )
			{
				fclose(fp);
				memset(bakfilename, 0, sizeof(bakfilename));
				sprintf(bakfilename, "%s.%10.10s", filename,szDateTime+2);
				rename(filename, bakfilename);
      			if ( ( fp = fopen(filename, "a+") ) == NULL )
					fp = stderr;
			}
   	}

	
	fprintf(fp, "[%s] ", s);

	va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    
    fprintf(fp, "\n", s);
    /*fprintf(fp, "\n", s);*/
    
	fflush(fp);

    if ( fp != stderr )
		fclose(fp);

	return ;
}

void APICfmLog(char *apilogfile, char *fmt, ...)
{
	va_list ap;
	FILE 	*fp = NULL;
	char 	filename[MAX_FILE_LEN+1], bakfilename[MAX_FILE_LEN+1];
	int  	nRet;
	char	*ptr = NULL;
	struct 	stat statbuf;
	time_t  systime;
	struct  tm *ptime;
	char    s[128];
	long	base_overflow;
	char	szDateTime[15];
	char	szLogSize[4];


	base_overflow = 5;

    systime=time(NULL);
    ptime=localtime(&systime);

    memset(s, 0, sizeof(s));
    strftime(s, sizeof(s), 	"%Y-%m-%d %H:%M:%S", ptime);

   	memset(szDateTime,0,sizeof(szDateTime));
    GetCommonCurrentTime( szDateTime );

    memset(filename, 0, sizeof(filename));
	sprintf(filename,"%s/%s.recv", getenv("APILOG_PATH"),apilogfile);

    if ( ( fp = fopen(filename, "a+") ) == NULL )
    	{
			fp = stderr;
			/*HtLog( logfile, __FILE__, __LINE__,
				"filename=[%s]\n",filename);*/
			LOG( "filename=[%s]\n",filename );
		}
	else
	{
			LOG( "filename=[%s]\n",filename );
			memset(&statbuf, 0, sizeof(statbuf));
			nRet = stat(filename, &statbuf);
			if ( nRet == 0 && statbuf.st_size >= SIZE_UNIT * base_overflow )
			{
				fclose(fp);
				memset(bakfilename, 0, sizeof(bakfilename));
				sprintf(bakfilename, "%s.%10.10s", filename,szDateTime+2);
				rename(filename, bakfilename);
      			if ( ( fp = fopen(filename, "a+") ) == NULL )
					fp = stderr;
			}
   	}

	
	fprintf(fp, "[%s] ", s);

	va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    
    fprintf(fp, "\n", s);
    fprintf(fp, "\n", s);
    
	fflush(fp);

    if ( fp != stderr )
		fclose(fp);

	return ;
}
void HtLog(char *logfile, char *file, int line, char *fmt, ...)
{
	va_list ap;
	FILE 	*fp = NULL;
	char 	filename[MAX_FILE_LEN+1], bakfilename[MAX_FILE_LEN+1]; 
	int  	nRet;
	char	*ptr = NULL;
	struct 	stat statbuf;
	time_t  systime;
	struct  tm *ptime;
	char    s[128];
	long	base_overflow;
	char	szDateTime[15];
	char	szLogSize[4];


	base_overflow = 5; 

    systime=time(NULL);
    ptime=localtime(&systime);

    memset(s, 0, sizeof(s));
    strftime(s, sizeof(s), 	"%Y-%m-%d %H:%M:%S", ptime);

   	memset(szDateTime,0,sizeof(szDateTime));
    GetCommonCurrentTime( szDateTime );

    memset(filename, 0, sizeof(filename));
	sprintf(filename,"%s/%s.log", getenv("LOGDIR"),logfile);
/*	sprintf(filename,"%s.log", logfile);*/
	   printf("filename=[%s]\n",filename);
    if ( ( fp = fopen(filename, "a+") ) == NULL )
			fp = stderr;
	else
	{
			memset(&statbuf, 0, sizeof(statbuf));
			nRet = stat(filename, &statbuf);
			if ( nRet == 0 && statbuf.st_size >= SIZE_UNIT * base_overflow )
			{
				fclose(fp);
				memset(bakfilename, 0, sizeof(bakfilename));
				sprintf(bakfilename, "%s.%10.10s", filename,szDateTime+2);
				rename(filename, bakfilename);
      			if ( ( fp = fopen(filename, "a+") ) == NULL )
					fp = stderr;
			}
   	}

	fprintf(fp, "Date:[%s] File:[%s] Line:[%d]\n", s, file, line); 

	va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
	
	fprintf(fp, "\n");

	memset(s, 0, sizeof(s));
	/*memset(s, '-', 80);*/
	memset(s, '-', 40);
	fprintf(fp, "%s\n", s);
	fflush(fp);

    if ( fp != stderr ) 
		fclose(fp);

	return ;
}
short DebugString( char *psBuf, int iLength, int iLine)
{
   int i,j=0;
   char s[100], temp[5];

   printf( "Debug Information from Line: %04d\n", iLine);

   for (i=0; i<iLength; i++)
   {
      if (j==0)
      {
         memset( s, ' ', 84);
         sprintf(temp,   " %03d:",i );
         memcpy( s, temp, 5);
         sprintf(temp,   ":%03d",i+15 );
         memcpy( &s[72], temp, 4);
      }
      sprintf( temp, "%02X ", (unsigned char)psBuf[i]);
      memcpy( &s[j*3+5+(j>7)], temp, 3);
      if ( isprint( psBuf[i]))
      {
         s[j+55+(j>7)]=psBuf[i];
      }
      else
      {
         s[j+55+(j>7)]='.';
      }
      j++;
      if ( j==16)
      {
         s[76]=0;
         printf( "%s\n", s);
         j=0;
      }
   }
   if ( j)
   {
      s[76]=0;
      printf( "%s\n", s);
   }
}

main()
{
    char            currenttime[15];
    char            currentdate[9];
    char            sCurDate[9];
    char			sPktId[20 + 1];
    char			sSmpNo[3 + 1];
    char			sSendBuff[1024*30];
	char            sRetCode[4];
	char			pRetStr[60 + 1];
    char            logfile[200];

    memset(sPktId, 0, sizeof(sSendBuff));
    memset(sPktId, 0, sizeof(sPktId));			
	memset(sSmpNo, 0, sizeof(sSmpNo));
    memset(sRetCode, 0, sizeof(sRetCode));
    memset(pRetStr, 0, sizeof(pRetStr));			
    memset(logfile, 0, sizeof(logfile));

	memcpy(sSendBuff, "{1:0000372B209    564290000010  309290000012    56429000001000000133                    2013041120130509154319          9                    }{2::CLZ:91304112090056429000001000000133:WD0:20130411:SBN:564290000010  :52A:564290000010  :RBN:309290000012  :IFT:0:72A:您好，贵行组201和202报文时查询807报文，明细序号tag的赋值应该有误，请检查}{C:                                }", 372);
	memcpy(sPktId, sSendBuff+48, 20);	
	memcpy(sSmpNo, sSendBuff+11, 3);
	memcpy(sRetCode, "000", 3);	
	memcpy(pRetStr, "报文发送成功", 60);
 /*   printf("current time is %s\n",GetCurTime());
    GetCommonCurrentTime(currenttime);
    CommonGetCurrentDate(currentdate);
    printf("time:%s\n",currenttime);
    printf("date:%s\n",currentdate);*/
    CommonGetCurrentDate(sCurDate);
   
	strcpy(logfile,"//home/smprun/smpferun/log/fromsmp123");
   /* APISendLog(sCurDate, "发送报文:报文标识号为:[%s],报文内容如下:[SMP%s:%s],处理结果返回码为:[%s],详细返回结果描述为:[%s]", sPktId, sSmpNo, sSendBuff, sRetCode, pRetStr);*/
   	HtLog(logfile, __FILE__, __LINE__, 		"\nfromsmp : fromsmp  start at %s\n", GetCurTime());
	printf("dir=[%s]\n",getenv("LOGDIR"));

}
