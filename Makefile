LOADER_SRC = loader/loader.c
TARGET = bin/loader
LIBS = -lprussdrv
CC = gcc

$(TARGET) : $(LOADER_SRC)
	$(CC) $(LOADER_SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
