ARMGNU = aarch64-elf

TARGET = kernel8.bin
SDTARGET = /Volumes/boot/kernel8.img
DEPS = *.h

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : $(TARGET) 

cp : clean $(TARGET)
	cp $(TARGET) $(SDTARGET)

%.o : %.s
	$(ARMGNU)-as $< -o $@

%.o : %.c $(DEPS)
	$(ARMGNU)-gcc $(COPS) -c $< -o $@

OBJECTS := $(patsubst %.s,%.o,$(wildcard *.s)) $(patsubst %.c,%.o,$(wildcard *.c))

$(TARGET) : $(OBJECTS)
	$(ARMGNU)-ld $(OBJECTS) -T memmap -o kernel8.elf
	$(ARMGNU)-objdump -D kernel8.elf > kernel8.list
	$(ARMGNU)-objcopy kernel8.elf -O binary $(TARGET)

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.auto


