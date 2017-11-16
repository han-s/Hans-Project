# Hans-project
1. [리눅스 명령어 구현 프로젝트](#리눅스_명령어_구현_프로젝트)

## 리눅스_명령어_구현_프로젝트
### 프로젝트 설명
ssu_shell : 사용자로부터 명령을 받아서 쉘 프로그램 실행
ssu_sed : 파일 혹은 디렉터리의 하위 파일에서 문자열을 검색하여 치환하는 명령어
(ssu_shell 과 ssu_sed 는 ssu_shell.c 파일에 함께 구현되어있음)

ssu_lsproc : 리눅스 시스템 상에 마운트 되어있는 proc 파일시스템을 참조해서 사용자가원하는 정보를 출력해주는 명령어 (ssu_lsproc.c 에 구현)

ssu_vim : Reader와 Writer의 역할을 수행할 명령어 (ssu_vim.c에 구현)ssu_ofm : 디몬 프로세스를 생성하며, 이 디몬 프로세스는 공유 파일의 Reader-Writer문제를FIFO파일과 시그널을 이용해서 제어하는 관리자 역할을 수행 (ssu_ofm.c에 구현)

### 프로젝트 개발환경
* OS : Linux
* Tools : vim, gcc
