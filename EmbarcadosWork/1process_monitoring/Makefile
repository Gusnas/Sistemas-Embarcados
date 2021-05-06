TARGET = native

CONTIKI_PROJECT =  hello-world
all: $(CONTIKI_PROJECT)

BLINKLED_DIR = ./
BLINKLED_SRC = blinkled.c

SENSORS_DIR = ../sensors
SENSORS_SRC = sensors.c

PROJECTDIRS += $(BLINKLED_DIR) $(SENSORS_DIR)
PROJECT_SOURCEFILES += $(BLINKLED_SRC) $(SENSORS_SRC)

CONTIKI =../../../contiki-ng
include $(CONTIKI)/Makefile.include

