#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<syslog.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<time.h>
#include<stdbool.h>
#include<pwd.h>

#define BUFFER_SIZE 1024 
int ssu_daemon_init(void);
static void ssu_signal_handler(int signo,siginfo_t *info, void *uarg);
void ssu_sigkill_handler(int signo);
int make_que();
void send_msg(int pid);
int get_que();
void ssu_sigusr2_handler(int signo, siginfo_t *info, void *uarg);
void L_option2(int pid);
FILE *fp;
void now_time(char timebuf[]);
void P_option();
void ID_option(int pid, uid_t uid);
void option_check();
int que_size=16;

struct ssu_msgbuf{
	char text[BUFFER_SIZE];
	long type;
};

FILE *log_fp;
int type_count=0;
int ffd;
int msg_qid, msg_qid2;
char ho[1024];
char logpath[1024];
char l_buf[1024];
char make_time[1024];
char arg[10][1024];
char N_option_arg[1024];
char P_option_arg[1024];
bool option[5];
struct msqid_ds questat;
int main(int argc, char* argv[])
{	
	if(argc<2){
		fprintf(stderr, "wwwww\n");
		exit(1);
	}
	
	for(int i=0; i<argc; i++)
	{
		strcpy(arg[i], argv[i]);
	}

	option_check();

	make_que();
	getcwd(l_buf,sizeof(l_buf));
	
	if(option[3]==true)
		P_option();

	strcpy(logpath, l_buf);
	strcat(logpath, "/");
	strcat(logpath, "ssu_log.txt");


	getcwd(ho,sizeof(ho));
	strcat(ho,"/");
	strcat(ho,argv[1]);
	int byte;
	struct sigaction sig_act, sigk_act, sigusr2_act;
	sigset_t old_set, sig_set;

	sig_act.sa_flags=SA_SIGINFO;
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_sigaction = ssu_signal_handler;
	sigaction(SIGUSR1, &sig_act, NULL);

	sigemptyset(&sigk_act.sa_mask);
	sigk_act.sa_flags=0;
	sigk_act.sa_handler= ssu_sigkill_handler;
	sigaction(SIGTERM, &sigk_act, NULL);
	
	sigemptyset(&sigusr2_act.sa_mask);
	sigusr2_act.sa_flags=SA_SIGINFO;
	sigusr2_act.sa_sigaction=ssu_sigusr2_handler;
	sigaction(SIGUSR2, &sigusr2_act, NULL);
	
	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGUSR1);
	sigaddset(&sig_set, SIGTERM);
	sigaddset(&sig_set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &sig_set, &old_set);
	
	
	pid_t pid;

	if(ssu_daemon_init() < 0){
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}
	
	if((log_fp=fopen(logpath, "w+"))==NULL)
	{
		fprintf(stderr, "ssu_log.txt fopen error\n");
		exit(1);
	}
	now_time(make_time);
	if(option[1]==false){
		fprintf(log_fp, "<<Deamon Process Initialized with pid : %d>>\n", getpid());
	}
	else
	{
	fprintf(log_fp, "%s<<Deamon Process Initialized with pid : %d>>\n", make_time,getpid());
	}

	if(option[2]==true)
	{
		que_size=atoi(N_option_arg);
	}
	fprintf(log_fp, "Initialized Queue Size : %d\n", que_size);
	
	fclose(log_fp);

	if(mkfifo("/tmp/fifo",0666)<0){
		fprintf(stderr,"mkfifo() error\n");
		exit(1);
	}

	if((ffd=open("/tmp/fifo",O_RDWR))<0){
		fprintf(stderr, "open error\n");
		exit(1);
	}
	while(1)
	{	
		sigsuspend(&old_set);	
	}

	exit(0);
}

int ssu_daemon_init(void){
	pid_t pid;
	int fd, maxfd;

	if((pid=fork()) < 0){
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid != 0)
		exit(0);

	pid = getpid();
	printf("process %d running as daemon\n", pid);
	setsid();
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize();
	
	for(fd = 0; fd < maxfd; fd++)
		close(fd);
	
	umask(0);
	chdir("/");
	fd=open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	return 0;
}

static void ssu_signal_handler(int signo,siginfo_t *info, void *uarg){
	int fd;
	char timebuf[1024];
	struct flock lock;
	lock.l_type=0;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len=0;
	now_time(timebuf);
	if((fd=open(ho,O_RDWR))<0){
		fprintf(stderr, "aaa open error\n");
		exit(1);
	}

	char buf[1024];
	
	read(ffd,buf, sizeof(buf));


	if(!strcmp(buf,ho))
	{	
		if((log_fp=fopen(logpath, "a+"))==NULL)
		{
			fprintf(stderr, "ssu_log.txt fopen error\n");
			exit(1);
		}
		
		if (fcntl(fd, F_GETLK, &lock) == -1) {
			fprintf(stderr, "ofm lock fail\n");
			exit(1);
		}
		
		if(option[1]==true)
		{
			fprintf(log_fp, "%sRequested Process ID : %d, Resquested Filename : %s\n",timebuf,info->si_pid, buf);
		}
		else
		{
			fprintf(log_fp, "Requested Process ID : %d, Resquested Filename : %s\n",info->si_pid, buf);
		}	
		
		if(option[4]==true){
			fclose(log_fp);
			ID_option(info->si_pid, info->si_uid);
		}
		else
			fclose(log_fp);

		if(lock.l_type == F_UNLCK){
			kill(info->si_pid ,SIGUSR1);
		}
		else
		{	

			send_msg(info->si_pid);
		}
	}
	
	
}

void ssu_sigkill_handler(int signo){
	remove("/tmp/fifo");
	exit(1);
}

int make_que()
{
	key_t key, key2;

	if((key= ftok("ssu_log.txt", 'H'))==-1){
		fprintf(stderr, " ftok error\n");
		exit(1);
	}

	if((msg_qid=msgget(key, 0644 | IPC_CREAT))==-1){
		fprintf(stderr, "msgget error\n");
		exit(1);
	}

}

void send_msg(int pid)
{	
	if(msgctl(msg_qid, IPC_STAT, &questat)==-1){
		fprintf(stderr, "msgctl error\n");
	}
	

	if(questat.msg_qnum == que_size)
	{
		printf("que is full\n");
		return ;
	}
	char strbuf[BUFFER_SIZE];
	struct ssu_msgbuf buf;
	buf.type=type_count;
	sprintf(strbuf,"%d", pid);
	strcpy(buf.text, strbuf);
	int length=strlen(buf.text);
	if(msgsnd(msg_qid, &buf,length+1, 0) == -1){
		fprintf(stderr, "msgsnd error\n");
		exit(1);
	}
	
}

int get_que(int qid)
{
	struct ssu_msgbuf buf;

	if(msgrcv(qid, &buf, sizeof(buf.text),0, 0)==-1){
		return 0;
	}

	int pid=atoi(buf.text);
	return pid;
}
void ssu_sigusr2_handler(int signo, siginfo_t *info, void *uarg){
	char timebuf[1024];
	now_time(timebuf);
	int pid=0;
	if((log_fp=fopen(logpath, "a+"))==NULL)
	{
		fprintf(stderr, "ssu_log.txt fopen error\n");
		exit(1);
	}
	
	if(option[1]==true)
	{
		fprintf(log_fp,"%sFinished Process ID : %d\n",timebuf,info->si_pid);
	}
	else
	{
		fprintf(log_fp,"Finished Process ID : %d\n", info->si_pid);
	}

	fclose(log_fp);
	
	if((log_fp=fopen(logpath, "a+"))==NULL)
	{
		fprintf(stderr, "ssu_log.txt fopen error\n");
		exit(1);
	}

	if(option[0] == true)
	{	
		
		L_option2(info->si_pid);
	}

	pid=get_que(msg_qid);

	
	
	if(pid!=0)
		kill(pid, SIGUSR1);
}
	

void L_option2(int pid)
{
	FILE *fp;
	char buf[1024];
	char buf2[1024];
	time_t ct;
	struct tm *ctm;
	char timebuf[1024];
	strcpy(buf,l_buf);
	strcat(buf,"/");
	ct=time(NULL);
	ctm=localtime(&ct);
	strftime(timebuf,1024, "[%F %T]", ctm);
	strcat(buf, timebuf);

	if((fp=fopen(buf, "a+"))==NULL)
	{
		fprintf(stderr, "%s fopen error\n", buf);
		exit(1);
	}

	if((log_fp=fopen(logpath, "a+"))==NULL)
	{
		fprintf(stderr, "%s fopen error\n", logpath);
		exit(1);
	}
	bzero(buf2, sizeof(buf2));
	while(fgets(buf2, 1024, log_fp)!=NULL)
	{
		fputs(buf2,fp);
	}

	fclose(log_fp);
	fclose(fp);
}
	
void now_time(char timebuf[])
{
	time_t ct;
	struct tm *ctm;

	ct=time(NULL);
	ctm=localtime(&ct);
	strftime(timebuf, 1024, "[%F %T]", ctm);

}

void P_option()
{	
	char buf[1024];
	mkdir(P_option_arg,0755);
	strcpy(buf, l_buf);
	strcat(buf, "/");
	strcat(buf, P_option_arg);
	bzero(l_buf, sizeof(l_buf));
	strcpy(l_buf, buf);
}

void ID_option(int pid, uid_t uid)
{
	FILE* fp;
	int i=0;
	char buf[1024];
	char temp[1024];
	char tmp[1024];
	struct passwd *psd;
	sprintf(temp,"%d", pid);
	bzero(buf, sizeof(buf));
	strcpy(buf, l_buf);
	strcat(buf,"/");
	strcat(buf, temp);
	
	if((fp=fopen(logpath, "a+"))==NULL)
	{
		fprintf(stderr, "%s fopen error\n", buf);
		exit(1);
	}
	psd=getpwuid(uid);
	fprintf(fp, "User : %s, UID : %d, GID : %d\n",psd->pw_name, psd->pw_uid, psd->pw_gid);
	fclose(fp);
}

void option_check()
{
	for(int i=2; i<10; i++)
	{
		if(!strcmp(arg[i], "-l"))
		{
			option[0]=true;
		}
		if(!strcmp(arg[i], "-t"))
		{
			option[1]=true;
		}
		if(!strcmp(arg[i], "-n"))
		{
			option[2]=true;
			strcpy(N_option_arg , arg[i+1]);
		}
		if(!strcmp(arg[i], "-p"))
		{
			option[3]=true;
			strcpy(P_option_arg , arg[i+1]);
		}
		if(!strcmp(arg[i], "-id"))
		{
			option[4]=true;
		}
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
