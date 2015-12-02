#!/bin/bash
# echo $1
# echo "#!/bin/bash" >>ucli.init
# [ -f *.init ]&&rm *.init
# [ -d case ]&&rm case -rf
[ -d case ]||mkdir case
print_help()
{
	cat <<EOF

******************************************************************************************
* Help: 命令行参数1等于1时，按照模块输出(即输出多个文件),否则输出一个总文件              *
*       命令行参数2:代表前置参数                                                         *
*	命令行参数3:代表后置参数                                                         * 
*       命令行参数4-n:代表生成对应命令行参数的模块文件                                   *
******************************************************************************************

EOF
}
print_help
cat pict_out.txt|sed 's/\t/./g'|awk -F "." 'BEGIN{ORS=" "
        # for( i=0; i<ARGC ; i++)
        # {
                # str[i]=ARGV[i] # 依次印出awk所记录的参数
                # print str[i]"======"ARGV[i]
        # }
}
{
        if(NR==1)
        {
                split($0,arr,".")
                split("'"$*"'",var," ")
                j=1
                k=1
                for(i=1;arr[i]!=NULL;i++)
                {
                        if(i%2==1)
                        {
                        fun[j]=arr[i]
                        j++
                        }
                        else
                        {
                        type[k]=arr[i]           
                        k++
                        }
                }
        }
        else
        {
                num_j=1
                num_i=1
                type_i=1
                num_total=0
                in_i=1
                while(fun[num_j]!=NULL)
                {
                        if(fun[num_j] == fun[num_j+1])
                        {
                                num_i++
                        }
                        else
                        {
                                if(num_i != 1)
                                        save_num=num_i
                                else
                                        save_num=1
                                num_total+=num_i
                                num_i=1           
                        }
                        if(num_i==1)
                        {    
                                if("'"$1"'" == "1")#按照模块输出
                                {
                                        if("'"$4"'" != NULL)
                                        {
        #***************************************************************************************************************************************                                
                                                if(fun[num_j] == "'"$4"'")
                                                {
                                                        # 加前置       
                                                        if(NR == 2)
                                                        {
                                                                print "在case目录下生成文件:\n\tfun_" fun[num_j] ".init\n"
                                                                print "#!/bin/bash\n" >"./case/fun_"fun[num_j] ".init"
                                                        }
                                                        print "'"$2"'" >>"./case/fun_"fun[num_j] ".init"
                                                        type_i=num_total-save_num+1
                                                        print fun[num_j] >>"./case/fun_"fun[num_j] ".init"
                                                        while(type_i<=num_total)
                                                        {
                                                            print $type_i >>"./case/fun_"fun[num_j] ".init"
                                                            type_i++
                                                        }
                                                        # 加后置
                                                        print "'"$3"'" >>"./case/fun_"fun[num_j] ".init"
                                                        print "\n" >>"./case/fun_"fun[num_j] ".init" 
                                                      
                                                }
                                                
                                                if(fun[num_j] == "'"$5"'")
                                                {
                                                        # 加前置        
                                                        if(NR == 2)
                                                        {
                                                                print "在case目录下生成文件:\n\tfun_" fun[num_j] ".init\n"
                                                                print "#!/bin/bash\n" >"./case/fun_"fun[num_j] ".init"
                                                        }
                                                        print "'"$2"'" >>"./case/fun_"fun[num_j] ".init"
                                                        type_i=num_total-save_num+1
                                                        print fun[num_j] >>"./case/fun_"fun[num_j] ".init"
                                                        while(type_i<=num_total)
                                                        {
                                                            print $type_i >>"./case/fun_"fun[num_j] ".init"
                                                            type_i++
                                                        }
                                                        # 加后置
                                                        print "'"$3"'" >>"./case/fun_"fun[num_j] ".init"
                                                        print "\n" >>"./case/fun_"fun[num_j] ".init"                                                        
                                                }  
                                                
                                                if(fun[num_j] == "'"$6"'")
                                                {
                                                        # 加前置
                                                        if(NR == 2)
                                                        {
                                                                print "在case目录下生成文件:\n\tfun_" fun[num_j] ".init\n"
                                                                print "#!/bin/bash\n" >"./case/fun_"fun[num_j] ".init"
                                                        }
                                                        print "'"$2"'" >>"./case/fun_"fun[num_j] ".init"
                                                        type_i=num_total-save_num+1
                                                        print fun[num_j] >>"./case/fun_"fun[num_j] ".init"
                                                        while(type_i<=num_total)
                                                        {
                                                            print $type_i >>"./case/fun_"fun[num_j] ".init"
                                                            type_i++
                                                        }
                                                        # 加后置
                                                        print "'"$3"'" >>"./case/fun_"fun[num_j] ".init"
                                                        print "\n" >>"./case/fun_"fun[num_j] ".init" 
                                                }
        #**************************************************************************************************************************************                                       
                                        }
                                        else
                                        {
                                                if(NR == 2)
                                                        print "#!/bin/bash\n" >"./case/"fun[num_j] ".init"
                                                print "'"$2"'" >>"./case/"fun[num_j] ".init"
                                                type_i=num_total-save_num+1
                                                print fun[num_j] >>"./case/"fun[num_j] ".init"
                                                while(type_i<=num_total)
                                                {
                                                        print $type_i >>"./case/"fun[num_j] ".init"
                                                        type_i++
                                                }
                                                # 加后置
                                                print "'"$3"'" >>"./case/"fun[num_j] ".init"
                                                print "\n" >>"./case/"fun[num_j] ".init"
                                                if(NR==2)
                                                {
                                                        if(in_i==1)
                                                        {
                                                                print "在case目录下生成以下文件:\n"
                                                                print "\t"fun[num_j] ".init\n"
                                                        }
                                                        else
                                                        {                                                                                                              
                                                                print "\t"fun[num_j] ".init\n"
                                                        }
                                                        in_i++
                                                }
                                        }
                                }
                                else
                                {       if((NR == 2)&&(in_i == 1))
                                        {
                                                print "#!/bin/bash\n" >"./case/total_case.init"
                                        }
                                        # 加前置
                                        print "'"$2"'" >>"./case/total_case.init"
                                        type_i=num_total-save_num+1
                                        print fun[num_j] >>"./case/total_case.init"
                                        while(type_i<=num_total)
                                        {
                                                print $type_i >>"./case/total_case.init"
                                                type_i++
                                        }
                                        # 加后置
                                        print "'"$3"'" >>"./case/total_case.init"
                                        print "\n" >>"./case/total_case.init"
                                        if((NR == 2)&&(in_i == 1))
                                        {
                                                print "在case目录下生成文件:\n\ttotal_case.init\n"                                               
                                        }
                                        in_i++
                                }
                        }
                        num_j++
                }
                # print "\n"
        }
}'

