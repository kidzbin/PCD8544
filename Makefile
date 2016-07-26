# LCD compile test

all:
	gcc TestPCD.c PCD8544.c -o TestPCD -lwiringPi

clean:
	rm -f LCDrun
