#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<string.h>
#include<stdbool.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>

#define BUFFER_MAX 256
#define SECOND_TO_MICRO 1000000
#define MAX_PATH_GUESSED 1024
#define DIRECTORY_SIZE MAXNAMLEN

char *pathname;
char command[LINE_MAX], grep_cmd[LINE_MAX];
char tmp[100][1000];

bool u_option=false, i_option=false,e_option=false,
	 d_option=false, p_option=false,s_option=false,
	 P_option=false, b_option=false;
void *my_memcpy(void *dest, const void *src, size_t size);
void modify(char* filename,char* SRC_STR, char* DEST_STR);
void my_grep(int argc,char* file, char* SRC_STR, char* DEST_STR);
bool my_strcmp(char a[],char b[]);
void ssu_runtime(struct timeval* begin_t, struct timeval* end_t);
int ssu_passing(char command[BUFFER_MAX], char *tok[]);
void ssu_sed(int tok_count,char *tok[]);
void my_strcpy(char a[], char b[]);
void my_strcat(char a[], char b[]);
int my_strlen(char a[]);
int main(void)
{
	char command[BUFFER_MAX];
	char *tok[256];
	char *prompt = "20122498 $ ";
	int tok_count=0;

	struct timeval begin_t;
	struct timeval end_t;

	while(1){
		fputs(prompt, stdout);
		if(fgets(command, sizeof(command), stdin) ==NULL)
			break;
			
			tok_count=ssu_passing(command, tok);
			if(tok_count>10){
				printf("too many arguments.\n");
				continue;
			}
			gettimeofday(&begin_t,NULL);
			if(!strcmp(tok[0],"ssu_sed")){
				ssu_sed(tok_count,tok);
			}
			else
				system(command);
			gettimeofday(&end_t,NULL);
			ssu_runtime(&begin_t, &end_t);
			
			
	}

	exit(0);
}

void ssu_runtime(struct timeval* begin_t, struct timeval* end_t){
	end_t->tv_sec -=begin_t -> tv_sec;

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t -> tv_sec--;
		end_t -> tv_usec += SECOND_TO_MICRO;
	}

	end_t ->tv_usec -= begin_t ->tv_usec;
	printf("time : %lu.%03d\n", end_t ->tv_sec, end_t->tv_usec/1000);
}

int ssu_passing(char command[BUFFER_MAX], char *tok[]){
	int tok_count=0;
	char str2[]=" ,\t\n";
	tok[tok_count]=strtok(command, str2);
	while(tok[tok_count] !=NULL){
		tok_count++;
		tok[tok_count]=strtok(NULL,str2);
	}
	for(int i=0; i<tok_count; ++i){
		my_strcpy(tmp[i],tok[i]);
	}

	for(int i=0; i<tok_count; ++i) {
		int check;
		for(check=0;tmp[i][check]!='\0';++check)
		{}
			if(tmp[i][check-1] == '\\'){
				tmp[i][check-1]=' ';
				tmp[i][check]=0;
				my_strcat(tmp[i],tmp[i+1]);
				for(int j=i; j<(tok_count-2);j++)
				{	
					my_strcpy(tmp[j+1],tmp[j+2]);
				}
				
				--i;
				--tok_count;
			}
	}
	return tok_count;

}

void my_strcpy(char a[],char b[]) {
	int i;
	for(i=0;b[i]!='\0';i++) {
		a[i]=b[i];
	}
	a[i]=0;
}
bool my_strcmp(char a[], char b[]){
	int aa=my_strlen(a);
	int bb=my_strlen(b);

	if(aa!=bb)
		return false;
	int i;
	bool tf=true;
	for(i=0;i<aa;i++)
	{
		if(b[i]!=a[i])
			return false;
	}
	return true;
	
}
int my_strlen(char a[]){
	int count=0;

	for(int i=0;a[i]!='\0'; i++)
		count++;

	return count;
}

void ssu_sed(int tok_count,char *tok[]){
	int count=0;
	char *TARGET=(char*)malloc(sizeof(char));
	char *SRC_STR=(char*)malloc(sizeof(char));
	char *DEST_STR=(char*)malloc(sizeof(char));
	char *str="arg";
	char *num[10]={"0 ","1 ","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 "};
	for(int i=0; i<tok_count; i++){
		
		fputs(str,stdout);
		fputs(num[i],stdout);
		puts(tmp[i]);
		fflush(stdout);
	}
	for(int i=1; i<tok_count; i++)
	{	

		if(my_strcmp(tmp[i],"-b"))
				b_option=true;
		else if(my_strcmp(tmp[i],"-u")){
				u_option=true;
		}
		else if(my_strcmp(tmp[i],"-i")){
				i_option=true;
		}
		else if(my_strcmp(tmp[i],"-e")){
				e_option=true;
		}
		else if(my_strcmp(tmp[i],"-d")){
				d_option=true;
		}
		else if(my_strcmp(tmp[i],"-p"))
				p_option=true;
		else if(my_strcmp(tmp[i],"-s"))
				s_option=true;
		else if(my_strcmp(tmp[i],"-P")){
				P_option=true;
		}
		else{
			if(count==0)
				strcpy(TARGET,tmp[i]);
			else if(count==1)
				strcpy(SRC_STR, tmp[i]);
			else if(count==2)
				strcpy(DEST_STR, tmp[i]);

			count++;
		}
	}
	my_grep(tok_count, TARGET, SRC_STR,DEST_STR);
}


void my_strcat(char a[], char b[])
{
	int c, d;
	for(c=0; c<=20; c++)
	{
		if(a[c]=='\0')
			break;
	}
	for(d=0;b[d]!='\0';d++)
	{
		a[c+d]=b[d];
	}
	a[c+d]='\0';
}
void *my_memcpy(void *dest, const void *src, size_t size)
{
	size_t i;
	for(i=0;i<size;i++)
	{
		*((char*)dest+i) = *((char*)src+i);
	}

	return dest;
}

void my_grep(int argc,char* file, char* SRC_STR, char* DEST_STR)
{
	struct dirent *dentry;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE+1];
	DIR *dirp;
	char *cwd;
	char *name;
	name=getcwd(NULL,1024);
	cwd=getcwd(NULL,1024);
	if((dirp=opendir(file))==NULL || chdir(file)==-1){
		fprintf(stderr, "opendir, chdir error for %s\n", file);
		return;
	}
	while((dentry = readdir(dirp)) !=NULL){
		if(dentry->d_ino ==0)
			continue;

		
		my_memcpy(filename, dentry->d_name, DIRECTORY_SIZE);
		
		if(my_strcmp(filename, ".")==1 || my_strcmp(filename, "..")==1)
			continue;
		
		
		if(stat(filename, &statbuf) ==-1){
			fprintf(stderr, "stat error for %s\n", filename);
			break;
		}
		if((statbuf.st_mode & S_IFMT) == S_IFREG){
			cwd=getcwd(NULL,1024);
			fputs(cwd,stdout);
			putchar('/');
			fputs(filename,stdout);
			putchar(' ');
			putchar(':');
			modify(filename,SRC_STR,DEST_STR);
			putchar('\n');
		}
		else if(S_ISDIR(statbuf.st_mode)!=0){
			my_grep(argc, filename, SRC_STR,DEST_STR);
		}
	}
	chdir(name);
	closedir(dirp);
}

void modify(char* filename,char* SRC_STR, char* DEST_STR)
{	
	int SRC = my_strlen(SRC_STR);
	int DEST = my_strlen(DEST_STR);
	int fd;
	int count2=0;
	int count=0;
	int cnt=0;
	int line=0;
	char buf[1024];
	FILE *io;
	int p;
	char *path;
	char ln[10];
	io=fopen(filename,"r+");
	fd=creat("tmp",0666);			 
	
	while(fgets(buf,1024,io)!=NULL){
	for(int i=0;buf[i]!='\n';i++)
		count++;
	
	line++;
	count++;//'\n'까지
	count2=count;
	for(int i=0;i<count2;i++)
	{
		if(buf[i]==SRC_STR[0]){
			for(int j=1;j<SRC;j++)
				if(buf[i+j]!=SRC_STR[j])
				{
					cnt++;
					break;
				}
		}
		else{
			cnt++;
		}

		if(cnt==0)
		{	
			if(SRC>DEST)
			{
				for(int j=0;j<DEST;j++)
					buf[i+j]=DEST_STR[j];

				for(int j=i+DEST; j<count;j++)
					buf[j]=buf[j+(SRC-DEST)];
					count=count-(SRC-DEST);
					
			}
			else if(SRC<DEST)
			{
				for(int j=count;j>=(i+SRC);j--)
					buf[j+(DEST-SRC)]=buf[j];
				for(int j=0;j<DEST;j++)
					buf[i+j]=DEST_STR[j];

					count=count+(DEST-SRC);
			}
			else
			{
				for(int j=0;j<DEST;j++)
					buf[i+j]=DEST_STR[j];
			}
			
		}
		cnt=0;
		}
	
	write(fd,buf,count);
	lseek(fd,0,SEEK_END);
	count=0;
	for(int i=0; i<1024;i++)
		buf[i]=0;
	}
	close(fd);
	fclose(io);
	remove(filename);
	rename("tmp",filename);
	fputs(" Success",stdout);
}
