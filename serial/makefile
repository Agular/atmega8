PRG            = main
OBJ            = main.o 
AVRDUDE        = /usr/bin/avrdude -c USBasp -p m8 -P usb -p m8

F_CPU          =  8000000UL
#F_CPU          = 11059200UL
#MCU_TARGET     = at90s2313
#MCU_TARGET     = at90s2333
#MCU_TARGET     = at90s4414
#MCU_TARGET     = at90s4433
#MCU_TARGET     = at90s4434
#MCU_TARGET     = at90s8515
#MCU_TARGET     = at90s8535
#MCU_TARGET     = atmega128
#MCU_TARGET     = atmega1280
#MCU_TARGET     = atmega1281
#MCU_TARGET     = atmega16
#MCU_TARGET     = atmega163
#MCU_TARGET     = atmega164p
#MCU_TARGET     = atmega165
#MCU_TARGET     = atmega165p
#MCU_TARGET     = atmega168
#MCU_TARGET     = atmega169
#MCU_TARGET     = atmega169p
#MCU_TARGET     = atmega32
#MCU_TARGET     = atmega324p
#MCU_TARGET     = atmega325
#MCU_TARGET     = atmega3250
#MCU_TARGET     = atmega329
#MCU_TARGET     = atmega3290
#MCU_TARGET     = atmega48
#MCU_TARGET     = atmega64
#MCU_TARGET     = atmega640
#MCU_TARGET     = atmega644
#MCU_TARGET     = atmega644p
#MCU_TARGET     = atmega645
#MCU_TARGET     = atmega6450
#MCU_TARGET     = atmega649
#MCU_TARGET     = atmega6490
MCU_TARGET     = atmega8
#MCU_TARGET     = atmega8515
#MCU_TARGET     = atmega8535
#MCU_TARGET     = atmega88
#MCU_TARGET     = attiny2313
#MCU_TARGET     = attiny24
#MCU_TARGET     = attiny25
#MCU_TARGET     = attiny26
#MCU_TARGET     = attiny261
#MCU_TARGET     = attiny44
#MCU_TARGET     = attiny45
#MCU_TARGET     = attiny461
#MCU_TARGET     = attiny84
#MCU_TARGET     = attiny85
#MCU_TARGET     = attiny861
OPTIMIZE       = -O2

DEFS           = -D F_CPU=$(F_CPU)
LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text #eeprom

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# dependency:
main.o: main.c

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

#eeprom: ehex ebin esrec

#ehex:  $(PRG)_eeprom.hex
#ebin:  $(PRG)_eeprom.bin
#esrec: $(PRG)_eeprom.srec

#%_eeprom.hex: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
#	|| { echo empty $@ not generated; exit 0; }

#%_eeprom.srec: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
#	|| { echo empty $@ not generated; exit 0; }

#%_eeprom.bin: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
#	|| { echo empty $@ not generated; exit 0; }

# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.

FIG2DEV                 = fig2dev
EXTRA_CLEAN_FILES       = *.hex *.bin *.srec disasm

dox: eps png pdf

eps: $(PRG).eps
png: $(PRG).png
pdf: $(PRG).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	$(FIG2DEV) -L png $< $@

disasm: main.elf
	avr-objdump -d main.elf  > $@

flash:  all
	$(AVRDUDE) -e -B8 -U flash:w:main.hex
#	$(AVRDUDE) -B8 -U eeprom:w:main_eeprom.hex

################################################################################
# !!! Diser Teil von Makefile sollte nicht geaendert werden !!!                #
################################################################################

# Fuse high byte:
#m0xd9 = 1 1 0 1   1 0 0 1
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (don't preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (full output swing)
#        | | +---------------- SPIEN (allow serial programming)
#        | +------------------ WDTON (WDT not always on)
#        +-------------------- RSTDISBL (reset pin is enabled)
# Fuse low byte:
#m0xe1 = 1 1 1 0   0 0 0 1
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external >8M crystal)
#        | |  +--------------- SUT 1..0 (crystal osc, BOD enabled)
#        | +------------------ BODEN (BrownOut Detector enabled)
#        +-------------------- BODLEVEL (2.7V)
#fuse:
#	$(UISP) --wr_fuse_h=0xc9 --wr_fuse_l=0x9f

#CKSEL3..0	0001;	CKOPT	1 (unprogrammed)
1mhz:
	$(AVRDUDE) -B8 -U lfuse:w:0xe1:m -U hfuse:w:0xd9:m -V
#$(UISP) --wr_fuse_h=0xd9 --wr_fuse_l=0xe1


#CKSEL3..0	0010;	CKOPT	1 (unprogrammed)
2mhz:
	$(AVRDUDE) -B8 -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -V
#$(UISP) --wr_fuse_h=0xd9 --wr_fuse_l=0xe2

#CKSEL3..0	0011;	CKOPT	1 (unprogrammed)
4mhz:
	$(AVRDUDE) -B8 -U lfuse:w:0xe3:m -U hfuse:w:0xd9:m -V
#$(UISP) --wr_fuse_h=0xd9 --wr_fuse_l=0xe3

#CKSEL3..0	0100;	CKOPT	1 (unprogrammed)
8mhz:
	$(AVRDUDE) -B8 -U lfuse:w:0xe4:m -U hfuse:w:0xd9:m -V
#$(UISP) --wr_fuse_h=0xd9 --wr_fuse_l=0xe4

#CKSEL3..0	1111;	CKOPT	0 (programmed)
ext_11mhz:
	$(AVRDUDE) -B8 -U lfuse:w:0xef:m -U hfuse:w:0xc9:m -V
#$(UISP) --wr_fuse_h=0xc9 --wr_fuse_l=0xef

