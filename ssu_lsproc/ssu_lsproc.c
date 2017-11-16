#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<limits.h>
#include<sys/wait.h>
#define OPT 13
#define PID 20
#define DIRECTORY_SIZE MAXNAMLEN
int s_cnt=0;
int opt_cnt=0;
int compare(const void *a, const void *b);
char *opt_pid[OPT][PID];
char opt_key[PID][100];
int int_opt_pid[OPT][PID];
char *attribute[5]={"FILEDES","CMDLINE","IO","STAT","ENVIRON"};
char *S_pid[1000];
char option[13];
void W_option();
void T_option(int s);
void N_option(int s);
void L_option();
void V_option();
void M_option(int s);
void E_option();
void C_option(int s);
void S_option();
void O_option();
int cnt2=0;
char * ltoa(long val, char *buf, unsigned radix);
int isnum(char *val);
void F_option(int s);
int main(int argc, char* argv[])
{	
	printf(">: ssu_lsproc start. :<\n");
	int pid;
	int status;
	int s_out=dup(1);
	int s_err=dup(2);
	int count=0;
	for(int i=0 ; i<13 ; i++)
		for(int j=0; j<20; j++)
			opt_pid[i][j]=" ";


	for(int i=0; i<13;i++)
		option[i]=0;


	for(int i=0; i<argc;i++)
	{   

		if(argv[i][0]=='-')
		{	

			switch(argv[i][1])
			{
				case 'f':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;
						
						//printf("www\n");
						opt_pid[0][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[0][count]=argv[j];
						count++;
					}
					option[0]='f';
					opt_cnt++;
					count=0;
					break;

				case 't':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[1][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[1][count]=argv[j];
						count++;
					}
					option[1]='t';
					opt_cnt++;
					count=0;
					break;
				case 'c':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[2][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[2][count]=argv[j];
						count++;
					}
					option[2]='c';
					opt_cnt++;
					count=0;
					break;
				case 'n':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[3][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[3][count]=argv[j];
						count++;
					}
					option[3]='n';
					opt_cnt++;
					count=0;
					break;
				case 'm':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[4][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[4][count]=argv[j];
						count++;
					}
					option[4]='m';
					opt_cnt++;
					count=0;
					break;
				case 'k':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[5][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[5][count]=argv[j];
						
						strcpy(opt_key[count],argv[j]);
						count++;
						cnt2++;
					}
					option[5]='k';
					count=0;
					break;
				case 'w':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[6][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[6][count]=argv[j];
						count++;
					}
					option[6]='w';
					opt_cnt++;
					count=0;
					break;
				case 'e':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[7][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[7][count]=argv[j];
						count++;
					}
					option[7]='e';
					opt_cnt++;
					count=0;
					break;
				case 'l':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[8][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[8][count]=argv[j];
						count++;
					}
					option[8]='l';
					opt_cnt++;
					count=0;
					break;
				case 'v':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[9][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[9][count]=argv[j];
						count++;
					}
					option[9]='v';
					opt_cnt++;
					count=0;
					break;
				case 'r':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[10][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[10][count]=argv[j];
						count++;
					}
					option[10]='r';
					count=0;
					break;
				case 's':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[11][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[11][count]=argv[j];
						count++;
					}
					option[11]='s';
					opt_cnt++;
					count=0;
					break;
				case 'o':
					for(int j=i+1;j<argc;j++)
					{	
						if(argv[j][0]=='-')
							break;

						opt_pid[12][count]=(char*)malloc(strlen(argv[j])+1);
						opt_pid[12][count]=argv[j];
						count++;
					}
					option[12]='o';
					opt_cnt++;
					count=0;
					break;
				default :
					break;
			}
		}
	}
	if(option[10]=='r')
	{	
		bzero(int_opt_pid,sizeof(int_opt_pid));
		for(int i=0; i<OPT ; i++)
			for(int j=0; j<16;j++)
				int_opt_pid[i][j]=atoi(opt_pid[i][j]);


		int cnt=0;
		for(int k=0; k<16;k++)
			for(int i=0; i<16; i++)
			{
				for(int j=0; j<16-i-1;j++)
				{
					if(int_opt_pid[k][j]>int_opt_pid[k][j+1])
					{	
						int temp=0;

						if(int_opt_pid[k][j+1]==0)
							continue;

						temp=int_opt_pid[k][j];
						int_opt_pid[k][j]=int_opt_pid[k][j+1];
						int_opt_pid[k][j+1]=temp;
					}
				}
			}
		for(int i=0;i<OPT;i++)
			for(int j=0; j<16;j++)
			{	
				if(int_opt_pid[i][j]!=0)
				{
					opt_pid[i][j]=(char*)malloc(sizeof(char)+1);
					ltoa(int_opt_pid[i][j],opt_pid[i][j],10);
				}
			}
		int cnt3=cnt2;
		if(cnt2>16)
			cnt3=16;
			
			
			for(int i=0; i<cnt3-1;i++)
			{
				for(int j=0; j<cnt3-1-i;j++)
				{	
					
						if(strcmp(opt_key[j],opt_key[j+1])>0)
						{	
							char te[100];
							strcpy(te,opt_key[j]);
							strcpy(opt_key[j],opt_key[j+1]);
							strcpy(opt_key[j+1],te);
						}
				}
			}
		
	}
	
	if(option[12]=='o')
	{
		int fd;
		
		fd=creat(opt_pid[12][0],0666);
		dup2(fd,1);
		dup2(fd,2);
		
		if(fd!=-1)
			printf("!--Successfully Redirected : %s--!\n",opt_pid[12][0]);
		
		fflush(stdout);

		close(fd);
		
	}
		
	
	for(int i=0; i<13;i++)
	{	
		if(option[i]==0)
			continue;

		pid=fork();
		if(pid==0)
		{	
			switch(option[i])
			{
				case 'f':
					F_option(0);
					exit(1);
					break;
				case 't':
					T_option(0);
					exit(1);
					break;
				case 'c':
					C_option(0);
					exit(1);
					break;
				case 'n':
					N_option(0);
					exit(1);
					break;
				case 'm':
					M_option(0);
					exit(1);
					break;
				case 'k':
					exit(1);
					break;
				case 'w':
					W_option();
					exit(1);
					break;
				case 'e':
					E_option();
					exit(1);
					break;
				case 'l':
					L_option();
					exit(1);
					break;
				case 'v':
					V_option();
					exit(1);
					break;
				case 'r':
					exit(1);
					break;
				case 's':
					S_option();
					exit(1);
					break;
				case 'o':
					exit(1);
					break;
				default:
					printf("don't have %s option\n",argv[1]);
					break;
			}

		}
		else if(pid>0)
		{	
			wait(&status);
		}
	}
	dup2(s_out,1);
	dup2(s_err,2);
	printf(">: ssu_lsproc terminated. :<\n");
}

void F_option(int s)
{	
	int count=0;
	if(s!=2)
	{
		for(int i=0; i<PID;i++)
		{
			if(opt_pid[0][i][0]==' ')
				break;

			count++;
		}
	}
	if(count >16){
		for(int i=16; i<count; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n",opt_pid[0][i]);

		count=16;
	}
	
	if(s==2)
	{
		count=s_cnt;
	}


	if(count==0 && s==0)
	{
		
			char my[10];
			pid_t childpid;
			childpid=getpid();

			ltoa(childpid,my,10); 
			DIR *dirp;
			struct dirent *dentry;
			struct stat statbuf;
			char temp[100];
			strcpy(temp, "/proc/");
			strcat(temp, my);
			strcat(temp,"/");
			strcat(temp,"fd");
			char* path=(char*)malloc(strlen(temp)+1);
			strcpy(path,temp);
			chdir(path);
		
		
		if(access(path,F_OK)==-1){
			printf("%s doesn't exist.\n", path);
			return;
		}
		else if(access(path,R_OK)==-1){
			printf("%s can't be read.\n", path);
			return;
		}
		dirp=opendir(path);
		while((dentry=readdir(dirp))!=NULL){
			char buf[1024];
			memset(buf, 0x00,sizeof(buf));

			char temp2[100];
			strcpy(temp2,path);
			strcat(temp2,"/");
			strcat(temp2, dentry->d_name);
			char* spath=(char*)malloc(strlen(temp)+1);//
			strcpy(spath,temp2);
			lstat(spath,&statbuf);
			if((statbuf.st_mode & S_IFMT) == S_IFLNK){
				if(readlink(spath,buf,1024)!=-1)
					printf("file Descriptor number : %s, Opened File : %s\n", dentry->d_name,buf);
			}
		}
		closedir(dirp);
	}
	for(int i=0;i<count;i++)
	{	
		DIR *dirp;
		struct dirent *dentry;
		struct stat statbuf;
		char temp[100];
		strcpy(temp, "/proc/");
		if(s==0)
		{
		strcat(temp, opt_pid[0][i]);
		}
		else if(s==2)
		{
			strcat(temp, S_pid[i]);
		}
		strcat(temp,"/");
		strcat(temp,"fd");
		char* path=(char*)malloc(strlen(temp)+1);
		strcpy(path,temp);
		chdir(path);
		if(count>1 &&s!=2)
			printf("([%s])\n", path);
		else if(s==2)
			printf("Attribute : FILEDES, Target process ID : %s ##\n",S_pid[i]);
		if(access(path,F_OK)==-1){
			printf("%s doesn't exist.\n", path);
			continue;
		}
		else if(access(path,R_OK)==-1){
			printf("%s can't be read.\n", path);
			continue;
		}
		dirp=opendir(path);
		while((dentry=readdir(dirp))!=NULL){
			char buf[1024];
			memset(buf, 0x00,sizeof(buf));

			char temp2[100];
			strcpy(temp2,path);
			strcat(temp2,"/");
			strcat(temp2, dentry->d_name);
			char* spath=(char*)malloc(strlen(temp)+1);//
			strcpy(spath,temp2);
			lstat(spath,&statbuf);
			if((statbuf.st_mode & S_IFMT) == S_IFLNK){
				if(readlink(spath,buf,1024)!=-1)
					printf("file Descriptor number : %s, Opened File : %s\n", dentry->d_name,buf);
			}
		}
		closedir(dirp);
	}

}

char * ltoa(long val, char *buf, unsigned radix)
{
	char *p;            
	char *firstdig;        
	char temp;            
	unsigned digval;    
	p = buf;
	if (radix == 10 && val < 0) { 
		*p++ = '-';
		val = (unsigned long)(-(long)val);
	}
	firstdig = p;    
	do {
		digval = (unsigned) (val % radix);
		val /= radix; 
		if (digval > 9)
			*p++ = (char) (digval - 10 + 'a');    
		else
			*p++ = (char) (digval + '0');        
	} while (val > 0);
	*p-- = '\0';    
	do {
		temp = *p;
		*p = *firstdig;
		*firstdig = temp; 
		--p;
		++firstdig; 
	} while (firstdig < p); 
	return buf;
}

void T_option(int s)
{
	int count=0;
	if(s!=3)
	{
		for(int i=0; i<PID;i++)
		{
			if(opt_pid[1][i][0]==' ')
				break;

			count++;
		}
	}
	if(count >16){
		for(int i=16; i<count; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n",opt_pid[1][i]);

		count=16;
	}
	if(s==3)
	{
		count=s_cnt;
	}

	int pid_count=0;
	int path_pid=count;

	while(1)
	{	
		char status[25][100];
		char my[10];
		FILE* fp;
		pid_t childpid;
		childpid=getpid();

		ltoa(childpid,my,10); 
		char temp[100];
		strcpy(temp, "/proc/");
		if(count==0)
			strcat(temp, my);
		else
		{	
			if(s!=3)
			{
			strcat(temp, opt_pid[1][pid_count]);
			pid_count++;
			}
			else
			{
				strcat(temp, S_pid[pid_count]);
			}
			
		}
		strcat(temp,"/");
		strcat(temp,"status");
		char* path=(char*)malloc(strlen(temp)+1);
		strcpy(path,temp);
		char* sta=(char*)malloc(sizeof(char));
		if(access(path,F_OK)==-1){
			printf("%s doesn't exist.\n",path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}
		if(access(path,R_OK)==-1){
			printf("%s can't be read.\n", path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}
		if((fp=fopen(path,"r"))<0)
		{
			fprintf(stderr,"error\n");
		}
		for(int i=0; i<25;i++)
		{
			fscanf(fp,"%s",status[i]);
		}
		if(path_pid>1&&s!=3)
			printf("([%s])\n",path);
		else if(s==3)
		{
			printf("##STATUS, target Process ID : %s ##\n",S_pid[pid_count]);
			pid_count++;
		}
		if(status[3][0]=='R')
			sta="Running";
		else if(status[3][0]=='S')
			sta="Sleeping in an interruptible wait";
		else if(status[3][0]=='D')
			sta="Waiting in uninterruptible disk sleep";
		else if(status[3][0]=='Z')
			sta="Zombie";
		else if(status[3][0]=='T')
			if(status[4][1]=='s')
				sta="Stopped";
			else
				sta="Tracing stop";
		else if(status[3][0]=='X')
			sta="Dead";

		printf("%s %s\n", status[0], status[1]);
		printf("%s %s\n", status[2], sta);
		printf("%s %s\n", status[5], status[6]);
		printf("%s %s\n", status[7], status[8]);
		printf("%s %s\n", status[9], status[10]);
		printf("%s %s\n", status[11], status[12]);
		printf("%s %s\n", status[13], status[14]);
		printf("%s %s\n", status[15], status[16]);
		printf("%s %s\n", status[20], status[21]);

		if(count!=0)
			count--;
		if(count==0)
			break;
	}

}


void C_option(int s)
{
	int count=0;
	int argv_count=0;
	if(s!=3)
	{
		for(int i=0; i<PID;i++)
		{
			if(opt_pid[2][i][0]==' ')
				break;

			count++;
		}
	}
	if(count >16){
		for(int i=16; i<count; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n",opt_pid[2][i]);

		count=16;
	}
	
	if(s==3)
	{
		count=s_cnt;
	}
	int pid_count=0;
	int path_pid=count;

	while(1)
	{	
		char buff[256];
		char status[25][100];
		char my[10];
		int fd,ret;
		pid_t childpid;
		childpid=getpid();
		bzero(buff,sizeof(buff));
		ltoa(childpid,my,10); 
		char temp[100];
		strcpy(temp, "/proc/");
		if(count==0)
			strcat(temp, my);
		else
		{	if(s!=3)
			{
				strcat(temp, opt_pid[2][pid_count]);
				pid_count++;
			}
			else
			{
				strcat(temp, S_pid[pid_count]);
			}
		}
		strcat(temp,"/");
		strcat(temp,"cmdline");
		char* path=(char*)malloc(strlen(temp)+1);
		strcpy(path,temp);
		char* sta=(char*)malloc(sizeof(char));
		
		if(access(path,F_OK)==-1){
			printf("%s doesn't exist.\n",path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}
		if(access(path,R_OK)==-1){
			printf("%s can't be read.\n", path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}
		if((fd=open(path,O_RDONLY))<0)
		{
			fprintf(stderr,"error\n");
		}
		if(s!=3 && count!=0)
			printf("([%s])\n",path);
		else if(s==3)
		{
			printf("##CMDLINE, target Process ID : %s ##\n",S_pid[pid_count]);
			pid_count++;
		}
		
			
		ret=read(fd, buff, sizeof(buff));
		if(buff[1]!='0' && buff[0]!='0')
			printf("argv[0] : ");
		for(int i=0; i<256;i++)
		{	
			if(buff[i]=='\0' && buff[i+1]!='\0'){
				printf("\n");
				argv_count++;
				printf("argv[%d] : ", argv_count);
				continue;
			}
			printf("%c",buff[i]);

		}
		printf("\n");
		argv_count=0;
		if(count!=0)
			count--;
		if(count==0)
			break;

	}
	
}
void N_option(int s)
{
	int count=0;
	if(s!=3)
		{
		for(int i=0; i<PID;i++)
		{
			if(opt_pid[3][i][0]==' ')
				break;

			count++;
		}
	}
	if(count >16){
		for(int i=16; i<count; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n",opt_pid[3][i]);

		count=16;
	}
	if(s==3)
	{
		count=s_cnt;
	}

	int pid_count=0;
	int path_pid=count;
	while(1)
	{	

		char status[14][100];
		char my[10];
		FILE* fp;
		pid_t childpid;
		childpid=getpid();

		ltoa(childpid,my,10); 
		char temp[100];
		strcpy(temp, "/proc/");
		if(count==0)
			strcat(temp, my);
		else
		{
			if(s!=3)
			{
			strcat(temp, opt_pid[3][pid_count]);
			pid_count++;
			}
			else
			{
				strcat(temp, S_pid[pid_count]);
			}

		}
		strcat(temp,"/");
		strcat(temp,"io");
		char* path=(char*)malloc(strlen(temp)+1);
		strcpy(path,temp);
		char* sta=(char*)malloc(sizeof(char));
		
		if(path_pid>1 && s!=3)
			printf("([%s])\n",path);
		if(s==3)
		{
			printf("##IO, target Process ID : %s ##\n",S_pid[pid_count]);
			pid_count++;
		}
		if(access(path,F_OK)==-1){
			printf("%s doesn't exist.\n", path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}

		if(access(path,R_OK)==-1){
			printf("%s can't be read.\n", path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}

		if((fp=fopen(path,"r"))<0)
		{
			fprintf(stderr,"error\n");
		}
		for(int i=0; i<14;i++)
		{
			fscanf(fp,"%s",status[i]);
		}
		//if(path_pid>1 && s!=3)
		//	printf("([%s])\n",path);

		printf("Characters read %s\n", status[1]);
		printf("Characters written %s\n", status[3]);
		printf("Read syscalls %s\n",status[5]);
		printf("Write syscalls %s\n", status[7]);
		printf("Bytes read %s\n", status[9]);
		printf("Bytes written %s\n", status[11]);
		printf("Cancelled write bytes %s\n", status[13]);

		if(count!=0)
			count--;
		if(count==0)
			break;
	}
}

void L_option()
{

	FILE *fp;
	char status[2][100];

	char temp[100];
	strcpy(temp, "/proc/");
	strcat(temp,"/");
	strcat(temp,"uptime");
	char* path=(char*)malloc(strlen(temp)+1);
	strcpy(path,temp);
	
	if(opt_cnt>1)
		printf("([%s])\n",path);
	if((fp=fopen(path,"r"))<0)
	{
		fprintf(stderr,"error\n");
	}

	for(int i=0; i<2;i++)
	{
		fscanf(fp,"%s",status[i]);
	}

	printf("Process worked time : %s(sec)\n", status[0]);
	printf("Process idle time : %s(sec)\n", status[1]);
}

void V_option()
{	
	FILE *fp;
	char ver[1000];
	char temp[100];

	strcpy(temp, "/proc");
	strcat(temp,"/");
	strcat(temp,"version");
	char* path=(char*)malloc(strlen(temp)+1);
	strcpy(path,temp);
	if(opt_cnt>1)
		printf("([%s])\n",path);

	if((fp=fopen(path,"r"))<0)
		fprintf(stderr, "error\n");

	fgets(ver,1000,fp);

	printf("%s",ver);

	fclose(fp);

}

void E_option()
{
	int count=0;
	FILE *fp;
	char *path="/proc/filesystems";
	if(opt_cnt>1)
		printf("([%s])\n",path);
	char *sys=(char*)malloc(sizeof(char));

	if((fp=fopen(path,"r"))<0)
		fprintf(stderr, "error\n");

	fgets(sys,1000,fp);
	while(1)
	{	

		if(sys[0]=='n' && sys[1]=='o' && sys[2]=='d' && sys[3]=='e' && sys[4]=='v')
		{	
			if(fgets(sys,1000,fp)==NULL)
			{
				printf("\n");
				break;
			}
			else
				continue;
		}

		if(count%5!=0)
			printf(", ");

		count++;


		for(int i=0; sys[i]!='\n' ; i++)
		{
			if(sys[i]==' '||sys[i]=='\t')
				continue;

			printf("%c",sys[i]);
		}
		if((count%5)==0)
		{
			printf("\n");
			if(fgets(sys,1000,fp)==NULL)
			{
				printf("\n");
				break;
			}
		}
		else if(fgets(sys,1000,fp)==NULL)
		{
			printf("\n");
			break;
		}
	}



}
void M_option(int s)
{

	int count=0;
	int argv_count=0;
	if(s!=3)
	{
		for(int i=0; i<PID;i++)
		{
			if(opt_pid[4][i][0]==' ')
				break;

			count++;
		}
	}
	if(count >16){
		for(int i=16; i<count; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n",opt_pid[4][i]);

		count=16;
	}
	if(cnt2 >16){
		for(int i=16; i<cnt2; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n",opt_pid[5][i]);

		count=16;
	}
	if(s==3)
	{
		count=s_cnt;
	}
	int pid_count=0;
	int path_pid=count;

	while(1)
	{	
		char status[25][100];
		char my[10];
		int fd,ret;
		pid_t childpid;
		childpid=getpid();
		ltoa(childpid,my,10); 
		char temp[100];
		strcpy(temp, "/proc/");
		if(count==0)
			strcat(temp, my);
		else
		{
			if(s!=3)
			{
				strcat(temp, opt_pid[4][pid_count]);
				pid_count++;
			}
			else
			{
				strcat(temp, S_pid[pid_count]);
				
			}
		}
		strcat(temp,"/");
		strcat(temp,"environ");
		char* path=(char*)malloc(strlen(temp)+1);
		strcpy(path,temp);
		char* sta=(char*)malloc(sizeof(char));
		if(count>=1 && path_pid>=2 && s!=3)
			printf("([%s])\n", path);
		else if(s==3)
		{
			printf("##ENVIRON, target Process ID : %s ##\n",S_pid[pid_count]);
			pid_count++;
		}
		if(access(path,F_OK)==-1){
			printf("%s doesn't exist.\n",path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}
		if(access(path,R_OK)==-1){
			printf("%s can't be read.\n", path);
			if(count!=0)
				count--;
			if(count==0)
				break;

			continue;
		}
		if((fd=open(path,O_RDONLY))<0)
		{
			fprintf(stderr,"error\n");
		}
		int c=0;
		int d=0;
		char tmp[50];
		char buff[10000];
		char buf[100][1000];
		bzero(buff,sizeof(buff));
		bzero(buf,sizeof(buf));
		bzero(temp,sizeof(tmp));
		ret=read(fd, buff, sizeof(buff));
		close(fd);
		for(int i=0; i<sizeof(buff);i++)
		{	
			if(buff[i]=='\0'&&buff[i+1]!='\0')
			{
				c++;
				d=0;
				continue;
			}
			buf[c][d]=buff[i];
			d++;
		}
		if(option[5]!='k')
		{
			for(int i=0; i<30;i++)
			{
				if(buf[i]==" ")
					break;
				printf("%s\n",buf[i]);
			}
		}
		else if(option[5]=='k' && opt_pid[5][0]==" ")
		{
			for(int i=0; i<30;i++)
			{
				if(buf[i]==" ")
					break;
				printf("%s\n",buf[i]);
			}
		}
		else
		{
			for(int i=0; i<16;i++)
			{
				for(int k=0; k<30;k++)
				{	
					int cnt=0;
					for(int j=0; buf[k][j]!='=';j++)
					{	
						if(opt_key[i][j]!=buf[k][j])
						{
							cnt=1;
							break;
						}
					}
					if(cnt==0)
						printf("%s\n",buf[k]);
				}
			}
		}	

		if(count!=0)
			count--;
		if(count==0)
			break;
	}
}

void S_option()
{
	uid_t uid;
	uid=getuid();
	int count=0;
	char status[17][100];
	struct dirent *dentry;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE+1];
	DIR *dirp;
	FILE *fp;
	if((dirp=opendir("/proc"))==NULL || chdir("/proc")==-1){
		fprintf(stderr, "opendir, chdir error for /proc\n" );
		exit(1);
	}

	while((dentry=readdir(dirp)) !=NULL){
		if(dentry->d_ino ==0)
			continue;

		memcpy(filename, dentry->d_name, DIRECTORY_SIZE);

		if(stat(filename, &statbuf)==-1){
			fprintf(stderr, "stat error for %s\n", filename);
			break;
		}
		if(isnum(dentry->d_name))
		{
			char path[30]="/proc/";
			strcat(path, dentry->d_name);
			strcat(path, "/");
			strcat(path, "status");

			if((fp=fopen(path,"r"))<0)
			{
				fprintf(stderr,"error\n");
			}
			bzero(status[16], sizeof(status[16]));
			
			for(int i=0; i<=16;i++)
			{
				fscanf(fp,"%s",status[i]);
			}
			int cnt=0;
			char buf[10];
			
			bzero(buf,sizeof(buf));
			ltoa(uid,buf,10);
			if(strcmp(buf,status[16]))
			{
				cnt++;
			}
			if(cnt==0)
			{	
				char *temp=dentry->d_name;
				S_pid[count]=temp;
				count++;
				s_cnt++;
				
			}
			cnt=0;
		}
	}
	for(int i=0; i<5; i++)
	{	
		for(int j=0; j<16; j++)
		{	
		
			if(!(strcmp(attribute[i],opt_pid[11][j])))
			{	
				
				switch(attribute[i][0])
				{
					case 'F':
						F_option(2);
						break;
					case 'C':
						C_option(3);
						break;
					case 'I':
						N_option(3);
						break;
					case 'S':
						T_option(3);
						break;
					case 'E':
						M_option(3);
						break;
					default:
						break;
				}

			}
		}
	}
}

void W_option()
{	
	int offset;
	char *path="/proc/interrupts";
	FILE *fp;
	int avg=0;
	int count=0;
	char name[3];
	char *buf=(char*)malloc(sizeof(buf)*1024);
	char in[100];
	if((fp=fopen(path, "r"))==NULL)
	{
		fprintf(stderr, "%s open error \n", path);
	}
	fgets(buf,1024,fp);
	for(int i=0; i<1024;i++)
	{
		if(buf[i]=='C')
			count++;
	}
	bzero(buf,sizeof(buf));
	char cpu[count][10];
	int num[count];
	offset=0;
	printf("---Number of CPUs : %d---\n",count);
	printf("      [Average] : [Description]\n");
	fclose(fp);
}
int isnum(char *val)
{
	int flag;

	for (int i = 0; i < strlen(val); i++)       
	{
		if (!isdigit(val[i]))   return 0;
	}
	return 1;
}

int compare(const void *a, const void *b){
		return strcmp((char*)a, (char*)b);
}
