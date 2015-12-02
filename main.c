#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<jansson.h>
#define PATH_NAME "./sed_src.txt"
size_t hand_data(void *buffer,size_t buflen,void *data)
{
        FILE *fp_val = (FILE *)data;
        char *buf = (char *)buffer;
        if(fgets(buf,256,fp_val))
        {
                printf("%s\n",buf);
                return strlen(buf);
        }
        else
                return 0;


}
int run_tests(char *save_key,FILE *fp_val)
{
        json_t *object;
        json_t *value;
        json_error_t error;
        fseek(fp_val,0,SEEK_SET);
        object = json_load_callback(hand_data,fp_val,0,&error);
        value = json_string(save_key);
        if(json_object_set(object,save_key,value) != 0)
                printf("set key fail\n");
        // else
                // printf("set key success\n");
        json_decref(object);
        return 0;
}

int main(int argc,char **argv)
{
        int start = 0,end = 0;
        char file_buf[256];
        char save_key[256];
        FILE *fp = NULL;
        FILE *fp_val = NULL;
        fp = fopen(PATH_NAME,"r+");
        if(fp == NULL)
        {
                perror("fopen sed.txt file fail");
                return -1;
        }
        fp_val = fopen("./test.txt","w+");
        if(fp_val == NULL)
        {
                perror("fopen test file fail");
                return -1;
        }
        memset(file_buf,0,256);
        while(fgets(file_buf,256,fp))
        {
                // file_buf[strlen(file_buf)-1] = 0;
                if(strstr(file_buf,"<dir>:"))
                {
                        start = 0;
                        continue;
                }
                else if(strstr(file_buf,"<json>:"))
                {
                        file_buf[strlen(file_buf)-strlen(" <json>:")-2] = 0;
                        memset(save_key,0,256);
                        strcpy(save_key,file_buf);
                        start = 0;
                }
                else if(strcmp(file_buf,"{\n"))
                {
                        start = 1;   
                        end =0;
                }
                else if(strcmp(file_buf,"}\n"))
                {
                        if(!strstr(file_buf,"\n"))
                                strcat(file_buf,"\n");
                        end = 1;
                }
                if(start == 1)//标记读到{，代表开始
                {
                        fwrite(file_buf,1,strlen(file_buf),fp_val);   
                        if(end == 1)
                        {
                                // printf("*******************************%s\n",save_key);
                                run_tests(save_key,fp_val);
                                fclose(fp_val);
                                fp_val = fopen("./test.txt","w+");
                        }
                }
        }
        fclose(fp_val);
        fclose(fp);
        return 0;
}