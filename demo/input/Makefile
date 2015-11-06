OBJ += get_var.o
OBJ += main.o 
target = ./test 	
FLAGS = -Wall
CC = gcc

$(target):$(OBJ)  	
	$(CC) $(OBJ) -o $@ $(FLAGS) 

%.o:%.c	
	$(CC) -c $^ -o $@ $(FLAGS)

clean:

	rm $(target) *.o -rfv	