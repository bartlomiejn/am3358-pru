LOADER_SRC = loader/loader.c
TARGET = bin/loader
LIBS = -lprussdrv
HEADERS = -Iinclude
CC = gcc

$(TARGET) : $(LOADER_SRC)
	$(CC) $(LOADER_SRC) $(LIBS) $(HEADERS) -o $(TARGET)

clean:
	rm -f $(TARGET)
