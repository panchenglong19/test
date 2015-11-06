OBJ += fun_test.o
target = ./output 	
FLAGS = -Wall
CC = gcc

$(target):$(OBJ)  	
	$(CC) $(OBJ) -o $@ $(FLAGS) 

%.o:%.c	
	$(CC) -c $^ -o $@ $(FLAGS)

clean:

	rm $(target) *.o -rfv	