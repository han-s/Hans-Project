#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<strings.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<stdbool.h>
#include<wait.h>
#include<dirent.h>
#include<ctype.h>

#define DIRECTORY_SIZE MAXNAMLEN
void main_R(char filename[]);
void main_W(char filename[]);
void main_RW(char filename[]);
int fd_unlock(int fd);
int fd_lock(int fd);
int compare_Modify(time_t before, char filename[]);
void t_option(char filename[]);
void t_result(int modify);
void s_option(off_t be_size, char filename[]);
void tmpf(char filename[]);
void d_option(char filename[]);
void W_RW(char filename[], char rw[]);
int search_demon();
int isnum(char *val);
void network_demon(int dpid, char filename[]);
static void ssu_signal_handler(int signo, siginfo_t *info, void *uarg);

int chfd;
char f_name[1024];
char op_name[10];
char *tmp;
time_t before;
struct stat statbuf, newstat;
bool option[3];
int dpid=0;

int main(int argc, char* argv[])
{	
	strcpy(f_name, argv[1]);
	strcpy(op_name, argv[2]);

	int pid;
	int modify;
	int status;
	int fd;
	char buf[1024];
	option[0]=false;
	option[1]=false;
	option[2]=false;
	dpid=search_demon();

	for(int i=0; i<argc; i++)
	{
		if(!strcmp(argv[i],"-t"))
			option[0]=true;
		else if(!strcmp(argv[i], "-s"))
			option[1]=true;
		else if(!strcmp(argv[i], "-d"))
			option[2]=true;
	}
	if(option[2]==true)
		tmpf(argv[1]);

	if(argc<3){
		fprintf(stderr, "aaa\n");
		exit(1);
	}
	
	if((stat(argv[1], &statbuf))<0){
		fprintf(stderr, "stat error\n");
		exit(1);
	}
				
	if(!strcmp(argv[2],"-r"))
	{	
		if(option[0]==true)
			t_option(argv[1]); 
		
		main_R(argv[1]);
	}
	else if(!strcmp(argv[2],"-w"))
	{	
		network_demon(dpid, argv[1]);
	}
	else if(!strcmp(argv[2],"-rw"))
	{
		char c;
		main_R(argv[1]);
		printf("Would you like to modify '%s'? (yes/no) : ",argv[1]);
		scanf("%c", &c);
		if(c=='y' || c=='Y')
			network_demon(dpid, argv[1]);
		else
			printf("finish\n");
	}

}

void main_R(char filename[])
{	
	FILE *fp;
	char buf[1000];
	if((access(filename, F_OK))==-1)
	{
		fprintf(stderr, "%s is not exist\n", filename);
		exit(1);
	}

	if((access(filename, R_OK))==-1)
	{
		fprintf(stderr, "%s is not read\n", filename);
		exit(1);
	}
	
	if((fp=fopen(filename, "r")) == NULL)
	{	
		fprintf(stderr, "%s open error\n", filename);
		exit(1);
	}

	bzero(buf, sizeof(buf));

	while(fgets(buf, sizeof(buf), fp)!=NULL)
	{
		for(int i=0; buf[i]!='\n' ; i++)
		{
			printf("%c", buf[i]);
		}
		printf("\n");
		bzero(buf, sizeof(buf));
	}
}

void main_W(char filename[])
{	
	int pid;
	int status;
	int fifo_fd;
	char buf[1024] = { 0 };
	strcpy(buf, getcwd(NULL, 0));
	strcat(buf,"/");
	strcat(buf, filename);
	struct flock lock;
	
	lock.l_type=1;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len=0;

	if((chfd=open(filename,O_RDWR))<0)
	{
		fprintf(stderr,"open error");
		exit(1);
	}

	
	fcntl(chfd, F_SETLK, &lock);
	if((pid=fork())<0){
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid==0){
		execl("/usr/bin/vim", "/vim", buf, (char *)0);	
	}
	if(wait(&status) != pid){
		fprintf(stderr, "wait error\n");
		exit(1);
	}
	fd_unlock(chfd);
}

int fd_lock(int fd)
{
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len=0;

	return 	fcntl(fd, F_SETLK, &lock);

}

int fd_unlock(int fd)
{
	struct flock lock;

	lock.l_type=F_UNLCK;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len = 0;

	return fcntl(fd, F_SETLK, &lock);
}

int compare_Modify(time_t before, char filename[])
{
	if((stat(filename, &newstat))<0){
		fprintf(stderr, "stat error\n");
		exit(1);
	}
	
	if(newstat.st_mtime==before)
		return 0;
	else
		return 1;

	exit(1);
}
void t_option(char filename[])
{
	time_t ct;
	struct stat statbuf;
	struct tm *tminfo;
	struct tm *ctm;
	char timebuf[1024];
	if((stat(filename, &statbuf))<0){
		fprintf(stderr, "stat error\n");
		exit(1);
	}
	tminfo=localtime(&(statbuf.st_mtime));
	strftime(timebuf, 1024, "[%F %T]", tminfo);

	printf("##[Modification Time]##\n");
	printf("Last Modification time of '%s' : %s\n", filename, timebuf);		
	
	ct=time(NULL);
	ctm=localtime(&ct);
	strftime(timebuf, 1024, "[%F %T]", ctm);
	printf("Current time: %s\n", timebuf);
	sleep(1);
	
}

void t_result(int modify)
{
	printf("##[Modification Time]##\n");
	
	if(modify==1)
		printf("-  There was modification.\n");
	else if(modify==0)
		printf("-  There was no modification.\n");
}

void s_option(off_t be_size, char filename[])
{		
	if((stat(filename, &newstat))<0){
		fprintf(stderr, "stat error\n");
		exit(1);
	}
		
	printf("##[File size]##\n");
	printf("--  Before modification : %ld(bytes)\n", be_size);
	printf("--  After modification : %ld(bytes)\n", newstat.st_size);
}

void tmpf(char filename[])
{
	FILE *fp;
	FILE *tmpfp;
	tmp=tmpnam(NULL);
	char buf[1024];
	if((fp=fopen(filename, "r")) == NULL)
	{	
		fprintf(stderr, "%s open error\n", filename);
		exit(1);
	}

	if((tmpfp=fopen(tmp, "w")) == NULL)
	{
		fprintf(stderr, "%s open error\n", tmp);
		exit(1);
	}
	
	bzero(buf, sizeof(buf));

	while(fgets(buf, sizeof(buf), fp)!=NULL)
	{	
		fputs(buf, tmpfp);
		bzero(buf, sizeof(buf));
	}

	fclose(fp);
	fclose(tmpfp);
}

void d_option(char filename[])
{	
	printf("##[Compare with Previous File]##\n");
	char buf[1024];
	printf("%s",tmp);
	printf("%s", filename);
	strcpy(buf, getcwd(NULL, 0));
	strcat(buf,"/");
	strcat(buf, filename);
	execl("/usr/bin/diff", "/diff", tmp, buf,(char *)0);
}
	
void W_RW(char filename[], char rw[])
{	
	int status;
	int modify;
	int pid;

	
		main_W(filename);
	
	fd_unlock(chfd);
	kill(dpid, SIGUSR2);
	modify=compare_Modify(statbuf.st_mtime, filename);
	if(option[0]==true)
		t_result(modify);

	if(option[1]==true && modify ==1)
		s_option(statbuf.st_size,filename);
	
	if(option[2]==true && modify ==1)
		d_option(filename);
}
	
int search_demon()
{
	struct dirent *dentry;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE+1];
	char buf[1024];
	char temp[1024];
	char real[1024];
	char d[10];
	int fd;
	DIR *dirp;
	char cwd[1024];
	getcwd(cwd,1024);
	if((dirp = opendir("/proc"))==NULL || (chdir("/proc"))==-1){
		fprintf(stderr, "opendir, chdir error for /proc\n");
		exit(1);
	}

	while((dentry = readdir(dirp)) != NULL){
		if(dentry->d_ino == 0)
			continue;

		memcpy(filename, dentry->d_name, DIRECTORY_SIZE);

		if(stat(filename, &statbuf) == -1){
			fprintf(stderr, "stat error for %s\n", filename);
			break;
		}
		if(isnum(dentry->d_name))
		{	
			bzero(temp,sizeof(temp));
			bzero(real, sizeof(real));
			strcpy(temp,"/proc/");
			strcat(temp, filename);
			strcat(temp,"/cmdline");
				
			char *demon=(char *)malloc(strlen(temp+1));
			strcpy(demon, temp);
			
			if((fd=open(demon, O_RDONLY))<0){

			}
					
			read(fd, buf, sizeof(buf));
			for(int i =0; i<sizeof(buf); i++)
			{	
				real[i]=buf[i];
				if(buf[i]=='\0' && buf[i+1]!='\0')
					break;
				
			}
			if(!strcmp(real,"./ssu_ofm"))
			{
				strcpy(d,dentry->d_name);
				break;
			}
		}
else
			continue;
	}
	chdir(cwd);
	return atoi(d);
}

int isnum(char *val)
{
	int flag;

	for(int i=0; i<strlen(val); i++)
	{
		if(!isdigit(val[i]))
			return 0;
	}
	return 1;
}
void network_demon(int dpid, char filename[])
{
	sigset_t pendingset;
	int fd;
	char buf[1024];
	sigset_t old_set, sig_set;
	
	if(dpid==0){
		fprintf(stderr, "where is ssu_ofm?\n ssu_vim error\n");
		exit(1);
	}

	if((fd=open("/tmp/fifo", O_WRONLY))<0){
		fprintf(stderr, "aa open error\n");
		exit(1);
	}

	getcwd(buf, sizeof(buf));
	strcat(buf, "/");
	strcat(buf, filename);
	write(fd, buf, sizeof(buf));

	struct sigaction sig_act;

	sig_act.sa_flags=SA_SIGINFO;
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_sigaction = ssu_signal_handler;
	sigaction(SIGUSR1, &sig_act, NULL);
	
	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGUSR1);
	sigaddset(&sig_set, SIGALRM);
	
	if(option[0]==true)
		t_option(filename);
	
	kill(dpid, SIGUSR1);
	
	sigprocmask(SIG_BLOCK, &sig_set, &old_set);
	while(1)
	{	
		char timebuf[1024];
		time_t ct;
		struct tm *ctm;
		ct=time(NULL);
		ctm=localtime(&ct);
		strftime(timebuf, 1024, "[%F %T]", ctm);
		if(option[0]==true)
			printf("Waiting for Token...%s%s\n",filename, timebuf);
		else
			printf("Waiting for Token...%s\n",filename);

		sleep(1);
		if(sigpending(&pendingset) == 0){
			if(sigismember(&pendingset, SIGUSR1)){
				break;
			}

		}
	}
	sigsuspend(&old_set);

}
static void ssu_signal_handler(int signo, siginfo_t *info, void *uarg){
		 W_RW(f_name, op_name);
	}



















		







