//============================================================================
// Name        : SpliceXml.cpp
// Author      : cy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "xmysql.h"
#include "config.h"
using EncapMysql::CEncapMysql;
using namespace std;
char dbip[32]={0};
char dbuser[32]={0};
char dbpass[32]={0};
char name[32]={0};
char pass[32]={0};
char path_out[0x100]={0};
int interval=10;
int main() {
	printf("start\n");
		if(getconfigstr("user","name",name,32,"config.ini")){
				printf("get user name error\n");
				return -1;
			}
		if(getconfigstr("mysql","ip",dbip,32,"config.ini")){
				printf("get mysql ip error\n");
				return -1;
			}
		if(getconfigstr("mysql","user",dbuser,32,"config.ini")){
				printf("get mysql user error\n");
				return -1;
			}
		if(getconfigstr("mysql","pass",dbpass,32,"config.ini")){
				printf("get mysql pass error\n");
				return -1;
			}
		if(getconfigstr("user","pass",pass,32,"config.ini")){
				printf("get user pass error\n");
				return -1;
			}
		if(getconfigstr("path","out",path_out,0x100,"config.ini")){
				printf("get out path error\n");
				return -1;
			}
		if(getconfigint("opt","interval",&interval,"config.ini")){
					printf("get opt interval error\n");
					return -1;
				}
	string text="";
	string sname=name;
	string spass=pass;
	string header="<?xml version=\"1.0\" encoding=\"UTF-8\" ?><zyczs guid=\"2E39ED2D7001D42FE050A8C0420857D1\"><deployment><code>deploy0001</code><name>"+sname+"</name><password>"+spass+"</password></deployment>";
	string middle="";
	string end="</zyczs>";
	string guid="";
	//string path="/app/datasync/tradenet/adatper/out/";
	FILE *fp;
	CEncapMysql *con;
	//printf("connecting!!!!\n");
	con=new CEncapMysql;
	//cout<<"connecting!!!!"<<endl;
	printf("connecting!!!!\n");
    if(con->Connect(dbip, dbuser, dbpass)==-1)//mysql数据库连接失败
		{
		//	cout<<"mysql数据库连接失败"<<endl;
			printf("mysql failure\n");
			 return false;
		}
    //cout<<"mysql connect succeed"<<endl;
    printf("mysql connect succeed\n");
		char sqlbuf[0x200]={0};
		//sprintf(sqlbuf,"SELECT	erpdb.erp_user.user_loginname,	erpdb.erp_user.user_loginpass,	erpdb.erp_boxtask.tabletboxitemnum,  erpdb.erp_boxtask.tabletboxnum,	erpdb.erp_boxtask.printstate FROM	erpdb.erp_boxtask INNER JOIN erpdb.erp_user ON erpdb.erp_boxtask.orgid = erpdb.erp_user.orgid WHERE	erpdb.erp_boxtask.boxtaskcode = '%s';",printtask.c_str());

		con->ModifyQuery("SET NAMES UTF8");
		con->ModifyQuery("use tcmbpspdb");
		//for(int i=0;i<1;i++){
		while(1){
		con->SelectQuery("select guid ,xmlcontent from tcm_xml limit 100");


		char** r=NULL;


		while(r = con->FetchRow()){
				guid=r[0];
				middle=r[1];
				 time_t t;

				   time(&t);
				   printf("operate time: %s", ctime(&t));
				//cout<<r[2]<<end;
				//cout<<r[0]<<endl;
				text=header+middle+end;
				string path_tmp=path_out+guid;
				cout<<path_tmp<<endl;
				fp=fopen(path_tmp.c_str(),"w+");
				if(fp==NULL){
					perror("create Xml file Failure");

					break;
				}
				//cout<<text<<endl;
				size_t num=fwrite(text.c_str(),text.length(),1,fp);
				if(num==EOF){
					perror("write Failure");
					fclose(fp);
					break;
				}
				char sql[0x100]={0};
				sprintf(sql,"delete from tcm_xml where guid='%s'",guid.c_str());
				con->ModifyQuery(sql);
				fclose(fp);
				r=NULL;
		}
		sleep(interval);
		}
		con->CloseConnect();
	wcout << "hello"<< endl; // prints Hello World!!!
	return 0;
}
