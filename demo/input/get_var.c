//./main '^\s*int.*;.*' < main.c//匹配int型
//'^\s*int.*(.*).*'//匹配函数
//按字节读  ，读到{ a+1   读到} a-1 ，之后如果a=0的话就代表函数结尾
//记录a！=0的行数和lno比较，如果相等则continue后继续匹配
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
// char a[]={"^\s*int.*(.*).*","^\s*char.*(.*).*"}//匹配函数


char *save_type[10] = {};//保存结构体类型
char stbuf[256];//变量定义
char *save_buf[1024];//结构体定义
int kk = 0;

char *function_save[128] ;
int fun_i = 0;
/* 取子串的函数 */
static char* substr(const char*str, unsigned start, unsigned end)
{
  unsigned n = end - start;
  // static char stbuf[256];
  strncpy(stbuf, str + start, n);
  
  stbuf[n] = 0;
  return stbuf;
}
/* 主程序 */
int get_var_state(char **argv)
{
  char * pattern;
  char *action[] = {"^\s*int.*;.*","^\s*char.*;.*","^\s*float.*;.*","^\s*double.*;.*","^\s*void.*;.*",
					"^#define.*",".*(.*)*;*$"};
  char *bbn="^\s*int.*;.*";
  printf("%s\n",bbn);
  char buf[20]={};
  int num = 1;
  int size = 0;
  int i = 0,j = 0;
  int m;

  int x, z, lno = 0, cflags = 0;
  char ebuf[128], lbuf[256]; 
  regex_t reg;//参数preg指向一个声明为regex_t的数据结构，用来保存编译结果
  regmatch_t pm[10];
  const size_t nmatch = 10;    
  FILE* fp = fopen(argv[1],"r");
  if(fp == NULL)
  {
	perror("open file fail");
	return 0;
  }
  int st_buf[1024] = {};
  int end_buf[1024] = {};
  /*逐个字节查找，记录函数头和尾所在行数*/
  while(fread(buf,1,1,fp) > 0)
  {
	  if(strcmp(buf,"\n") == 0)//记录行数
	  {
		num++;
	  }
	  if(strcmp(buf,"{") == 0)//记录{数
	  {
		size++;
		if(size == 1)//记录函数头的行数
		{
			st_buf[i++] = num;
			//记录行数
		}	  
	  }
	  if(strcmp(buf,"}") == 0)//记录}数
	  {
		size--;		
		if(size == 0)//此时代表函数结尾
		{
			end_buf[j++] = num;//记录行数
		}	  
	  }
	  memset(buf,0,sizeof(buf));
  }
  for(m = 0;m <= 6;m++)
  {
	rewind(fp);
	z = regcomp(&reg,action[m], cflags);
	if (z != 0){
	regerror(z, &reg, ebuf, sizeof(ebuf));
	fprintf(stderr, "%s: pattern '%s' \n", ebuf, pattern);
	return 1;
	} 
	int k = 0; 
	int i = 1;	


	char save[128] = {};
	int name = 0;
	int struct_i = 0;
	/*  逐行处理输入的数据 */
	while(fgets(lbuf, sizeof(lbuf), fp)) {
		++lno;
		/*获取文件中的结构体存到save_buf中*/
		if(m == 0)	//只存储一次
		{
			if((strncmp(lbuf,"struct",6) == 0) || (strncmp(lbuf+8,"struct",6) == 0) || (i == 0))//i是代表是否进入了结构体
			{
				if(i == 1)
				{
					if(name == 0)
						printf("结构体类型有:\n");
					bzero(save, strlen(save)+1);//将内容清零
					// if(strncmp(lbuf+8,"struct",6) == 0)
					if(strncmp(lbuf,"struct",6) == 0)
						sscanf(lbuf," %[^{]",save);//结构体类型名保存
					if(strncmp(lbuf+8,"struct",6) == 0)
						sscanf(lbuf,"typedef %[^{]",save);
					save_type[kk] = (char *)malloc(strlen(lbuf)+1);
					strcpy(save_type[kk],save);
					printf("%s\n",save);
					name = 1;
					kk++;
				}
				
				i = 0;//标志位：表明已经进入struct结构体的开头了，直到遇到"}"代表结尾
				if(strncmp(lbuf,"}",1) == 0)
				{
				/*
					bzero(save, strlen(save)+1);//将内容清零
					// if(strncmp(lbuf+8,"struct",6) == 0)
					sscanf(lbuf,"}%[^;]",save);//结构体类型名保存
					save_type[kk] = (char *)malloc(strlen(lbuf)+1);
					strcpy(save_type[kk],save);
					// printf("save_type=%s\n",save_type[kk]);
					kk++;*/
					i=1;
					struct_i++;
				}
			}
			// if(strncmp(lbuf,"struct",6) == 0)
			// {
				// sscanf(lbuf,"[^{]",save);
			// }
		}
		/*局部变量不进行匹配（即程序上面记录了函数开始和结束的行数，下面把相应的行排除了continue）*/
		if(lno ==end_buf[k])
		{
			k++;
		}
		if(lno >= st_buf[k] && lno <= end_buf[k])//如果函数的开头行数 == 现在读到的行数，则这行不进行匹配(即排除局部变量）
		{
			continue;
		}
		
		/*进行正则匹配*/
		
		if ((z = strlen(lbuf)) > 0 && lbuf[z-1] == '\n')
		  lbuf[z - 1] = 0;	 
		/* 对每一行应用正则表达式进行匹配 */
		z = regexec(&reg, lbuf, nmatch, pm, 0);
		if (z == REG_NOMATCH) continue;
		else if (z != 0) {
		  regerror(z, &reg, ebuf, sizeof(ebuf));
		  fprintf(stderr, "%s: regcom('%s')\n", ebuf, lbuf);
		  return 2;
		}
		/* 输出处理结果 */
		for (x = 0; x < nmatch && pm[x].rm_so != -1; ++ x) {
		  // if (!x) printf("%04d: %s\n", lno, lbuf);
		  // printf("  $%d='%s'\n", x, substr(lbuf, pm[x].rm_so, pm[x].rm_eo));
		  if(m == 6)
		  {
			  function_save[fun_i] = (char *)malloc(strlen(substr(lbuf, pm[x].rm_so, pm[x].rm_eo)));
			  
			  strcpy(function_save[fun_i],substr(lbuf, pm[x].rm_so, pm[x].rm_eo));
			  // printf("function_save=%s\n",function_save[fun_i]);
			  fun_i++;
		  }
		}
	}
	/* 释放正则表达式  */
	regfree(&reg); 
	
  }	
  fclose(fp);
  return 0;
} 