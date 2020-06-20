#include<mysql.h>
#include<List.h>
#include<errno.h>

MYSQL      sock;
MYSQL_RES *result;
MYSQL_ROW  record;


int   InitMySql();
int   ConnectMySql(char *host,char *user,char* pass,char* db);
int   SelectMySql(char* szSql,int nColumn,List *lststr);
int   UpdateMySql(char* szSql);
void  DisConnect();





