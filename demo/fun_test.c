//可以再返回值处做出判断，看是否是你想要的返回值，
// 如果是指测试通过（把此时的数据、返回值存储到文件中），	
// if(返回值==true){i++;测试通过}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SAVE_FILE_PATH "./path/"
#define FUNCTION_PATH "./path/output_path/fun.txt"
#define STRUCT_PATH "./path/output_path/str.txt"
int get_and_cut(char *struct_type,char *buf_save,FILE *fp)
{
	
	char row_buf[128] = "";
	int val_j = 0,i,total = 0,row_sign;
	
	
	char *row_result[100];
	//放全局/结构体
	int column = 0;
	// char *struct_type[100];//结构体类型
	char *column_val[100];//结构体变量
	char *save_val[1024];//变量对应的值
	int val_i = 0;//值的个数
	row_sign = 0;
	int len = sizeof(buf_save);
	memset(buf_save,0,len);
	while(fgets(row_buf,sizeof(row_buf), fp))
	{
		//切割/t		
		if(row_sign == 0)
		{
			row_result[column]=strtok(row_buf,"\t");
			while(row_result[column]!=NULL)
			{
				column++;
				row_result[column]=strtok(NULL,"\t");
			}
			printf("row=%d\n",column);
			/*********按.切割存储在指针数组中(将结构体类型保存  结构体变量保存)*/
			sscanf(row_result[0],"%[^.]",struct_type);//结构体类型
			for(i=0;i<column;i++)
			{
				column_val[i] = (char *)malloc(sizeof(column_val[i]));
				sscanf(row_result[i],"%*[^.].%s",column_val[i]);//结构体变量				
			}
		}
		else
		{
			save_val[val_i]=strtok(row_buf,"\t");
			while(save_val[val_i]!=NULL)
			{
				val_i++;
				save_val[val_i]=strtok(NULL,"\t");
			}
			for(val_j=0;val_j<column;val_j++)
			{				
				if(val_j == 0)
					strcat(buf_save,"\t{\n");
				strcat(buf_save,"\t\t");
				strcat(buf_save,column_val[val_j]);
				strcat(buf_save,":");
				strcat(buf_save,save_val[val_j+column*total]);
				if(val_j == (column-1))
				{
					strcat(buf_save,"\n");
					strcat(buf_save,"\t},\n");
					total++;
				}
				else
					strcat(buf_save,",\n");
			}	
		}
		row_sign++;
	}
	
	return row_sign;
}


void strcat_function()
{


}
void strcat_struct(char *struct_type,char *save_buf,char **argv,int row_sign,int type_fun)
{
	char write_move[1024],save_path[40],write_buf[300];
	// char path_name[40]="";
	memset(save_path,0,sizeof(save_path));
	memset(write_move,0,sizeof(write_move));
	strcpy(write_move,struct_type);
	strcat(write_move,".h");
	strcpy(save_path,SAVE_FILE_PATH);
	strcat(save_path,write_move);
	FILE *file_opint_h = fopen(save_path,"w+");
	if(file_opint_h == NULL)
	{
		perror("fopen file fail");
		return ;
	}
	char h_buf_write[1024] = "";
	
	
	memset(save_path,0,sizeof(save_path));
	char c_file_name[20];
	strcpy(c_file_name,struct_type);
	strcat(c_file_name,".c");
	strcpy(save_path,SAVE_FILE_PATH);
	strcat(save_path,c_file_name);
	FILE *c_fp = fopen(save_path,"w+");
	if(c_fp == NULL)
	{
		perror("fopen file fail");
		return ;
	}
	char buf_save[1024];
	memset(buf_save,0,sizeof(buf_save));
	//C写头文件
	strcat(buf_save,"#include");
	strcat(buf_save,"\"");
	strcat(buf_save,argv[1]);
	strcat(buf_save,"\"\n");
	
	//H写头文件
	strcpy(h_buf_write,buf_save);

	
	
	
	
	//加判断
	if(type_fun == 1)
	{
		
		FILE *fp = fopen("./input/variable_type.txt","r");
		if(fp == NULL)
		{
			perror("file open fail");
			return ;
		}
		strcat(h_buf_write,"struct fun {\n");
		memset(write_buf,0,sizeof(write_buf));
		while(fgets(write_buf,sizeof(write_buf), fp))
		{
			
			write_buf[strlen(write_buf)-1] = 0;
			strcat(h_buf_write,"\t");
			strcat(h_buf_write,write_buf);
			strcat(h_buf_write,";\n");
			memset(write_buf,0,sizeof(write_buf));
		}
		strcat(h_buf_write,"};\n");
		fclose(fp);
		strcat(h_buf_write,"struct fun");
		strcat(h_buf_write," fun_msg[");
	}
	else
	{
		strcat(h_buf_write,struct_type);
		strcat(h_buf_write," test_msg[");
	}
	
	
	
	char string[20] = "";
	printf("row=%d\n",row_sign);
	sprintf(string,"%d",(row_sign-1));//把数字转化为字符串
	strcat(h_buf_write,string);
	strcat(h_buf_write,"];");
	fwrite(h_buf_write,strlen(h_buf_write),1,file_opint_h);

	//C	
	strcat(buf_save,"#include");
	strcat(buf_save,"\"");
	strcat(buf_save,struct_type);
	strcat(buf_save,".h");
	strcat(buf_save,"\"\n");
	
	if(type_fun == 1)
	{
		strcat(buf_save,"struct fun");
		strcat(buf_save," fun_msg[");	
	}
	else
	{
		strcat(buf_save,struct_type);
		strcat(buf_save," test_msg[");	
	}
	
	strcat(buf_save,string);
	strcat(buf_save,"] = {\n");
	strcat(buf_save,save_buf);
	
	strcat(buf_save,"};\n");
	fwrite(buf_save,strlen(buf_save),1,c_fp);
	fclose(c_fp);
	fclose(file_opint_h);
	return ;

}




int main(int argc,char *argv[])
{
	char save_buf[1024],fun_buf[1024];
	// long int len=0;
	int row_sign = 0;
	FILE *struct_fp=fopen(STRUCT_PATH,"r");
	if(struct_fp==NULL)
	{
		perror("fopen file fail");
		return 0;
	}	
	FILE *function_fp=fopen(FUNCTION_PATH,"r");
	if(function_fp==NULL)
	{
		perror("fopen file fail");
		return 0;
	}
	char struct_type[20],fun_type[20];

	row_sign = get_and_cut(struct_type,save_buf,struct_fp);	
	strcat_struct(struct_type,save_buf,argv,row_sign,0);
		
	row_sign = get_and_cut(fun_type,fun_buf,function_fp);	
	strcat_struct(fun_type,fun_buf,argv,row_sign,1);
	
	fclose(struct_fp);
	fclose(function_fp);
	return 0;
}
	