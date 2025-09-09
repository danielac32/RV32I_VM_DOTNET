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

 
CFLAGS = -march=rv32ima -mabi=ilp32 -ffreestanding -nostdlib -Os
INCLUDES     =  -I /usr/include
#LDSCRIPT     =  test/link.ld -static
LDFLAGS = -Wl,-Bstatic,-T,sections.lds,--strip-debug


OBJDIR        = obj
START         = $(wildcard start.S) $(wildcard main.c)
STARTOBJ      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(START)))))


DOUT         = kernel


SRCPATH = $(sort $(dir $(START) $(SOURCES) $(SRCLIB) $(SRCBASIC) $(FLOATLIB) $(SRCFAT32) $(SRCMONKEY) $(FSLIB) $(KERNEL) $(SRCSHELL) $(SRCDEVICE)))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)


 
$(OBJDIR):
	@mkdir $@


kernel:  $(DOUT).bin
		$(OBJDUMP) -d kernel.elf > kernel.list


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

