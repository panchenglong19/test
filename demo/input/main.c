#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include"get_var.h"
#define INPUT_PATH "../path/input_path/"

char *type_j[100];

//把变量解析出来
void *get_val(char *lbuf,char *type_i)
{
	char val_buf[100];
	if(strchr(lbuf,'[') != NULL) 
	{
		sscanf(lbuf,"%*[^ ] %[^[]",type_i);
	}
	if(strchr(lbuf,'*') != NULL) 
	{
		sscanf(lbuf,"%*[^*]*%[^;]",type_i);
	}
	if(strstr(lbuf,"struct") != NULL || strstr(lbuf,"enum") != NULL)
	{
		sscanf(lbuf,"%*s%*s %[^;]",type_i);
	}
	if(((strchr(lbuf,'[') != NULL) || (strchr(lbuf,'*') != NULL)) && (strstr(lbuf,"struct") != NULL))
	{
		
		sscanf(lbuf,"%*s%*s %[^;]",type_i);
		if(strchr(type_i,'[') != NULL)
		{	
			sscanf(type_i,"%[^[]",val_buf);
			memset(type_i,0,strlen(type_i));
			strcpy(type_i,val_buf);
		}
		if(strchr(type_i,'*') != NULL)
		{
			sscanf(type_i,"*%s",val_buf);
			memset(type_i,0,strlen(type_i));
			strcpy(type_i,val_buf);
		}
	}
	if((strchr(lbuf,'[') == NULL) && (strchr(lbuf,'*') == NULL) && (strstr(lbuf,"struct") == NULL) && (strstr(lbuf,"enum") == NULL))
	{
		sscanf(lbuf,"%*[^ ] %[^;]",type_i);			
	}
	return NULL;
}

	/**************************************结构体定义的打印************************************/
int input_data(FILE *fp,int j)
{	
	char lbuf[100] = "";
	int type_lev = 1,type_row = 1;

	char type_i[100];
	
	int val_num = 0;//变量个数
	int x = 0,y = 0;
	while(fgets(lbuf,sizeof(lbuf),fp))
	{
		if(strncmp(save_type[j-1],lbuf,strlen(save_type[j-1])) == 0 || type_lev == 0)
		{
			type_lev = 0;
			printf("%s\n",lbuf);
			if(x > 0 && strncmp(lbuf,"}",1) != 0)
			{
			/******************把每行的变量定义的变量取出来，放入type_i中**************************************************/
				get_val(lbuf,type_i);				
				type_j[val_num] = (char *)malloc(strlen(type_i)+1);
				strcpy(type_j[val_num],type_i);
		/****************************************************************************************************************************/
				val_num++;//记录变量的个数
			}
			if(strncmp(lbuf,"}",1) == 0)
			{
				type_lev = 1;	
			}
			x++;
		}
		if((strncmp(save_type[j-1],lbuf+8,strlen(save_type[j-1])) == 0) || type_row == 0)
		{
			type_row = 0;
			printf("%s\n",lbuf);
			if(y > 0 && strncmp(lbuf,"}",1) != 0)
			{
				get_val(lbuf,type_i);
				type_j[val_num] = (char *)malloc(strlen(type_i)+1);
				strcpy(type_j[val_num],type_i);
			/****************************************************************************************************************************/
				val_num++;//记录变量的个数
			}
			y++;
			if(strncmp(lbuf,"}",1) == 0)
			{
				type_row = 1;	
			}		
		
		}
	}
	return val_num;
	
}


void write_test_file(int j,int val_num)
{
	char ev_type[40] = "",val[20]="",insert_buf[40];
	int val_str = 0,i = 0;
	strcpy(ev_type,INPUT_PATH);
	strcat(ev_type,save_type[j-1]);	
	ev_type[strlen(ev_type)-1] = 0;
	strcat(ev_type,".txt");
	FILE *fp_write=fopen(ev_type,"w+");
	if(fp_write == NULL)
	{
		perror("fopen file fail");
		return ;
	}
	printf("请输入结构体类型.变量:值1，值2.....\n");
	printf("如果想要退出输入，请输入break\n");
	char buf_val[128] = "";
	char p[128] = "";
	int i_sscanf = 0;
	while(1)
	{
		
		memset(insert_buf,0,sizeof(insert_buf));
		fgets(insert_buf,100,stdin);		
		
		if(strcmp(insert_buf,"break\n") != 0)
		{
			i_sscanf = sscanf(insert_buf,"%*[^.].%[^:]",val);
			// printf("%d\n",i_sscanf);
			// printf("%s\n",val);
			if(i_sscanf == -1)
			{
				printf("输入格式错误 请重新输入\n");
				continue;
			}
			for(i = 0;i < val_num;i++)
			{
				if(strcmp(val,type_j[i]) == 0)
				{
					val_str = 1;
				
				}
				else
				{
					val_str = 0;
				}
			}
			if(val_str == 0)
			{
				printf("没有这个变量,请重新输入\n");
				continue;
			}
		/********************把数字换为结构体名*************************/
			strcpy(p,insert_buf+1);
			insert_buf[1]=0;
			j = atoi(insert_buf);
			strcpy(buf_val,save_type[j-1]);
			strcat(buf_val,p);
		}
		else
		{
			//设置个标志位，判断是哪个结构体写完了
			break;
		}
		fwrite(buf_val,100,1,fp_write);
		// sleep(1);
	}
	fclose(fp_write);
}	





void test_function_action(FILE* fp)
{
	FILE *fun_fp;
	char fun_path[40] = "";
	char insert[40] = "",fun_name[40],return_val[20],save_fun_name[40] = "";
	char *fun_p;
	char *parameter_val[40];
	char *parameter_type[40];
	int i,j,num=0;
	printf("请输入您要测试的函数名称:\n");
	fgets(insert,sizeof(insert),stdin);
	insert[strlen(insert)-1] = 0;
	//,+1个参数
	// char *buf[20] = {"int","char","void",....};
	for(i=0;i<fun_i;i++)
	{
		fun_p = strstr(function_save[i],insert);
		if(fun_p != NULL)
		{
			strcpy(fun_path,INPUT_PATH);
			strcat(fun_path,insert);
			strcat(fun_path,".txt");
			fun_fp = fopen(fun_path,"w+");
			if(fun_fp == NULL)
			{
				perror("fopen file fail");
				return ;
			}
			printf("函数定义为：\n");
			printf("%s\n",function_save[i]);
			memset(fun_name,0,sizeof(fun_name));
			memset(return_val,0,sizeof(return_val));
			sscanf(function_save[i],"%*[^ ] %[^(]",fun_name);//函数名
			sscanf(function_save[i],"%[^ ]",return_val);//返回值类型
			// printf("%s===%s\n",fun_name,return_val);
			
			//参数个数=num+1
			while(*fun_p != 0)
			{
				if(*fun_p == ',')
				{
					
					num++;
				}
				fun_p++;
			}			
			//把函数的参数用strtok切割成类型parameter_type、变量parameter_val的形式分别存起来
			strtok(function_save[i],"(");
			for(i=0,j=0;i<num&&j<num;i++,j++)
			{
				parameter_type[i] = strtok(NULL," ");				
				parameter_val[j] = strtok(NULL,",");
			}
			parameter_type[i] = strtok(NULL," ");
			parameter_val[j] = strtok(NULL,")");

		/******************------------------------------------------------***********************************************/
			for(i=0;i<=j;i++)
				printf("%s\n",parameter_val[i]);
			
			break;
		
		}
	}
	char get_buf[20],buf[40];
	int num_i,sign_val;
	printf("请输入您要传给函数参数的值:\n");
	printf("模式为参数：值1，值2，值3......\n");
	printf("输入break为退出输入\n");
	while(1)
	{
		memset(insert,0,strlen(insert)+1);
		memset(save_fun_name,0,sizeof(save_fun_name));
		fgets(insert,sizeof(insert),stdin);
		if(strcmp(insert,"break\n") != 0)
		{
			sscanf(insert,"%[^:]",get_buf);
			printf("%s\n",get_buf);
			for(num_i=0;num_i<=j;num_i++)
			{
				if(strcmp(parameter_val[num_i],get_buf) == 0)
				{
				//有这个变量sign_val置为1，并且把函数的参数存入文件中
					memset(buf,0,sizeof(buf));
					strcpy(buf,parameter_type[num_i]);
					strcat(buf," ");
					strcat(buf,parameter_val[num_i]);
					strcat(buf,"\n");
					fwrite(buf,strlen(buf),1,fp);
					sign_val = 1;
					break;			
				}
				else
					sign_val = 0;
			}
			if(sign_val == 1)
			{
				strcpy(save_fun_name,fun_name);
				strcat(save_fun_name,".");
				strcat(save_fun_name,insert);
				fwrite(save_fun_name,strlen(save_fun_name),1,fun_fp);	
			}
			else
			{
				printf("没有这个参数，请重新输入:\n");
				continue;
			}
		}
		else
			break;
	}
	fclose(fun_fp);
	return ;	
}

	
	
int main(int argc,char **argv)
{
	int val_num = 0;
	// sscanf(buf,"#include "%[^"]",heard);//取出头文件的位置及名称	
	FILE *fp = fopen(argv[1],"r+");//打开需要获取的结构体定义的.h文件
	if(fp == NULL)
	{
		perror("fopen file fail");
		return 0;
	}
	FILE *variable_type = fopen("./variable_type.txt","w+");
	if(variable_type == NULL)
	{
		perror("fopen file fail");
		return 0;
	}

	
	
	get_var_state(argv);	
	char buf[10] = "";
	printf("请输入1或者2代表要测试的是结构体或函数:\n");
	fgets(buf,sizeof(buf),stdin);
	buf[strlen(buf)-1] = 0;
	if(strcmp(buf,"1") == 0)
	{
	/**************************************结构体定义的打印************************************/
		char insert[100];
		printf("1、2、3....代表对应的结构体\n");
		printf("请输入要测试的结构体类型\n");
		fgets(insert,10,stdin);
		insert[strlen(insert)-1] = 0;
		int j = atoi(insert);	
		val_num = input_data(fp,j);	
	/**************************************************************************************/
		write_test_file(j,val_num);
	}
	
	if(strcmp(buf,"2") == 0)
	{
		test_function_action(variable_type);
	}
	fclose(fp);
	fclose(variable_type);
	return 0;
}

