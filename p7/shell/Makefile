ARMGNU = arm-none-eabi

DIR = $(shell basename `pwd`)
TARGET = $(DIR).bin
SDTARGET = /Volumes/boot/$(TARGET)
DEPS = ../*.h

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding

all : $(TARGET) 

cp : $(TARGET)
	cp $(TARGET) $(SDTARGET)

copy : clean $(TARGET)
	cp $(TARGET) $(SDTARGET)

%.o : %.s
	$(ARMGNU)-as $< -o $@

%.o : %.c $(DEPS)
	$(ARMGNU)-gcc $(COPS) -c $< -o $@

OBJECTS := $(patsubst %.s,%.o,$(wildcard *.s)) $(patsubst %.c,%.o,$(wildcard *.c))

$(TARGET) : $(OBJECTS)
	$(ARMGNU)-ld $(OBJECTS) -T memmap -o app.elf
	$(ARMGNU)-objdump -D app.elf > app.list
	$(ARMGNU)-objcopy app.elf -O binary $(TARGET)

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.auto


