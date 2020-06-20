#include<MySql.h>

int InitMySql()
{
	if(mysql_init(&sock)== NULL)
	{
		perror("mysql fail..\n");
		return -1;
	}
	mysql_set_character_set(&sock,"gb2312");

	return 0;
}
int ConnectMySql(char *host,char* user,char* pass,char *db)
{

	if(!mysql_real_connect(&sock,host,user,pass,db,0,NULL,CLIENT_MULTI_STATEMENTS))
	{
		printf("%s\n",mysql_error(&sock));
		return -1;
	}
	else
	{
		printf("connect success..\n");
		return 0;
	}
//	mysql_query(&sock,"SET NAMES UTF8");

}
int UpdateMySql(char* szSql)
{
	if(!szSql) return -1;
	if(mysql_query(&sock,szSql)) return -1;
	return 0;

}
int SelectMySql(char *szSql,int nColumn,List* lststr)
{
	if(mysql_query(&sock,szSql)) return -1;
	result = mysql_store_result(&sock);
	if(result == NULL) return -1;
	while(record = mysql_fetch_row(result))
	{
		for(int i=0;i<nColumn;i++)
		{
			L_Push(lststr,(void*)record[i]);
		}
	}

	return 0;
}
void DisConnect()
{
	mysql_close(&sock);
}
