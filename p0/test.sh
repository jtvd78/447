#!/bin/bash
if diff from_website/1_boot.o code/1_boot.o; then 
	echo "1_boot.o Passed"
else
	echo "1_boot.o Failed"
fi

tmp1=$(mktemp)
tmp2=$(mktemp)

#####################################################################

aarch64-elf-objdump -d from_website/kernel.o > "$tmp1"
aarch64-elf-objdump -d code/kernel.o > "$tmp2"

if diff <(tail -n +3 "$tmp1") <(tail -n +3 "$tmp2"); then
	echo "kernel.o Passed"
else 
	echo "kernel.o Failed"
fi

#################################################################

aarch64-elf-objdump -d from_website/led.o > "$tmp1"
aarch64-elf-objdump -d code/led.o > "$tmp2"

if diff <(tail -n +3 "$tmp1") <(tail -n +3 "$tmp2"); then
	echo "led.o Passed"
else
	echo "led.o Failed"
fi

######################################################################

# aarch64-elf-objdump -d from_website/led.o > "$tmp1"
# aarch64-elf-objdump -d code/led.o > "$tmp2"

if diff from_website/time.o   code/time.o; then
	echo "time.o Passed"
else
	echo "time.o Failed"
fi

if diff from_website/utils.o  code/utils.o; then
	echo "utils.o Passed"
else
	echo "utils.o Failed"
fi

rm "$tmp1"
rm "$tmp2"
