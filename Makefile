TARGET = loader
LIBS = -lprussdrv
CC = gcc

$(TARGET) : loader.c
	$(CC) loader/loader.c $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
