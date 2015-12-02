#!/bin/bash
echo >pict_in.txt
for src in `ucli | sed '1d' | sed  's/module://g' | sed 's/,/\n/g'`
do
	ucli $src | sed 's/command args ...//g' | sed '2d' | sed 's/\t/ /g' | sed 's/\[/#/g' | sed 's/\]/@/g' | awk '{
		if(NR == 1)
		{
			name = $0
		}
		else
		{
			if(NF > 1)
			{
				
				var = 0
				for(i=2;i<=NF;i++)
				{	
					j = 0
					if(($i ~ "#") && ($i !~ "@"))
					{
						var = $i
						j = 1
					}
					if(j == 0)
					{
						if(var == 0)
							print name $1 "." $i ":"
						else
						{
							print name $1 "." var " " $i ":"
							var = 0
							
						}
					}
				}
				
			
			}
		}
	
	}' | sed 's/#/\[/g' | sed 's/@/\]/g' >>pict_in.txt
	echo >>pict_in.txt
	echo >>pict_in.txt
done
cat pict_in.txt
echo "*****************************************************************************************************"
echo "按照格式为:  ucli module command.参数:参数的值1,值2,值3 ... 输入"
echo "例如:        ucli node remove.[NODE-ID]:3,44,5"
echo "输出文件名:  pict_in.txt"
echo "*****************************************************************************************************"
