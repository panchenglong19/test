// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// int main()
// {
        // char *buf[100];
		// char buf_name[1024];
		// int i= 0;
		// FILE* fp=fopen("pub.h","r+");
        // char *name = "namesdafasdf\nslkdfjsldkf";
		// char *name2 = "aaaaaaaaaaaf\nslkdfjsldkf";
		// while(fgets(buf_name,1024,fp))
		// {
			// buf[i] = (char *)malloc(4);
			// strcat(buf[i],buf_name);
			// strcat(buf[i],name);
			// printf("%s\n",buf[i]);
			// i++;
		// }
        // return 0;
// }
#include <stdio.h>;
#include <sys/types.h>;
#include <regex.h>;

/* 取子串的函数 */
static char* substr(const char*str, unsigned start, unsigned end)
{
  unsigned n = end - start;
  static char stbuf[256];
  strncpy(stbuf, str + start, n);
  stbuf[n] = 0;
  return stbuf;
}
/* 主程序 */
int main(int argc, char** argv)
{
  char * pattern;
  int x, z, lno = 0, cflags = 0;
  char ebuf[128], lbuf[256];
  regex_t reg;
  regmatch_t pm[10];
  const size_t nmatch = 10;
  /* 编译正则表达式*/
  pattern = argv[1];//{'\s*int.*\s'};
  z = regcomp(&reg, pattern, cflags);
  if (z != 0){
    regerror(z, &reg, ebuf, sizeof(ebuf));
    fprintf(stderr, "%s: pattern '%s' \n", ebuf, pattern);
    return 1;
  }
  /*  逐行处理输入的数据 */
  while(fgets(lbuf, sizeof(lbuf), stdin)) {
    ++lno;
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
      printf("  $%d='%s'\n", x, substr(lbuf, pm[x].rm_so, pm[x].rm_eo));
    }
  }
  /* 释放正则表达式  */
  regfree(&reg);
  return 0;
} 
