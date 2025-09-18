#export PATH=/opt/riscv/bin:$PATH

TOOLSET     ?= riscv32-unknown-linux-gnu-
CC           = $(TOOLSET)gcc
LD           = $(TOOLSET)ld
AR           = $(TOOLSET)gcc-ar
OBJCOPY      = $(TOOLSET)objcopy
OBJDUMP =$(TOOLSET)objdump
OPTFLAGS    ?= -Og
 

RM = rm -f
fixpath = $(strip $1)

 
CFLAGS = -march=rv32i -mabi=ilp32 -ffreestanding -nostartfiles  -fno-builtin   -O1
INCLUDES     =  -I ../lib
#LDSCRIPT     =  test/link.ld -static
LDFLAGS = -T  link.ld 


OBJDIR        = obj
START         = $(wildcard ../lib/boot.S) $(wildcard ../lib/intr.S) $(wildcard main.c)  $(wildcard ../lib/*.c)  
STARTOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(START)))))


DOUT         = rv32


SRCPATH = $(sort $(dir $(START) ))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)


 
$(OBJDIR):
	@mkdir $@


rv32:  $(DOUT).bin
		$(OBJDUMP) -d rv32.elf > rv32.list


$(DOUT).bin : $(DOUT).elf
	@echo building $@
	@$(OBJCOPY) -O binary $< $@



$(DOUT).elf : $(OBJDIR) $(STARTOBJ) 
	@echo building $@
	@$(CC) $(CFLAGS) $(LDFLAGS)  $(STARTOBJ)  -o $@

	
clean: $(OBJDIR)
	clear
	$(MAKE) --version
	@$(RM) $(DOUT).*
	@$(RM) $(call fixpath, $(OBJDIR)/*.*)


$(OBJDIR)/%.o: %.S
	@echo assembling $<
	@$(CC) $(CFLAGS)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS)  $(INCLUDES) -c $< -o $@

