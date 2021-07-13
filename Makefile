RM	:= rm -f
CC	:= gcc
FLG	:= -Wall -pedantic -Wextra -Wpointer-arith 
FLG += -Wshadow -Wfloat-equal -Waggregate-return

all : feeder child
	@echo "> linking finished"
	@echo "Run: ./feeder <M> <n>"
	@$(RM) *.o

clean :
	@$(RM) feeder child output.txt
	@echo "all cleaned"

functions.o: functions.c functions.h sems.h shm.h
	@$(CC) -c $(FLG) functions.c

sems.o: sems.c sems.h
	@$(CC) -c $(FLG) sems.c

shm.o: shm.c shm.h
	@$(CC) -c $(FLG) shm.c

feeder: functions.o sems.o shm.o 
	@$(CC) -o feeder feeder.c functions.o sems.o shm.o 
	@echo "- created feeder"

child: functions.o sems.o shm.o 
	@$(CC) -o child child.c functions.o sems.o shm.o 
	@echo "- created child"
