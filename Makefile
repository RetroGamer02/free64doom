IWAD = FREEDOOM1
# Options: FREEDOOM1 FREEDOOM2

ROOTDIR			= $(N64_INST)
GCCN64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CHKSUM64PATH = $(ROOTDIR)/bin/chksum64
MKDFSPATH = $(ROOTDIR)/bin/mkdfs
MKSPRITEPATH = $(ROOTDIR)/bin/mksprite
HEADERPATH = $(ROOTDIR)/mips64-elf/lib
N64TOOL = $(ROOTDIR)/bin/n64tool
N64_SYM = $(ROOTDIR)/bin/n64sym
N64_DSO = $(ROOTDIR)/bin/n64dso
N64_DSOEXTERN = $(ROOTDIR)/bin/n64dso-extern
N64_DSOMSYM = $(ROOTDIR)/bin/n64dso-msym
HEADERNAME = header
LIBS = -ldragon -lc -lm -ldragonsys
PROG_NAME = $(IWAD)
OPTFLAG = -O3

# function/data sections + gc-sections saves about ~200kb from output binary
CFLAGS = -ffunction-sections -fdata-sections -falign-functions=32 -ffile-prefix-map=$(CURDIR)= -DN64 -fdiagnostics-color=always -Wno-error=switch -Wall -g -mabi=o64 -mno-shared -mno-abicalls -mno-branch-likely -mno-llsc -mno-check-zero-division -mfix4300 -std=gnu99 -march=vr4300 -mtune=vr4300 -mips3 -G4 $(OPTFLAG) -I$(ROOTDIR)/mips64-elf/include
#-DRANGECHECK
LINK_FLAGS = --gc-sections -G4 -g -L$(ROOTDIR)/mips64-elf/lib $(LIBS) -Tn64.ld

CC = $(GCCN64PREFIX)gcc
AS = $(GCCN64PREFIX)as
LD = $(GCCN64PREFIX)ld
OBJCOPY = $(GCCN64PREFIX)objcopy
OBJDUMP = $(GCCN64PREFIX)objdump

SRC_DIR=src
BUILD_DIR=build

OBJS=						\
		$(BUILD_DIR)/i_sound.o			\
		$(BUILD_DIR)/endoom.o			\
		$(BUILD_DIR)/dll.o			\
		$(BUILD_DIR)/hash.o			\
		$(BUILD_DIR)/lzfx.o			\
		$(BUILD_DIR)/i_video.o			\
		$(BUILD_DIR)/i_input.o			\
		$(BUILD_DIR)/doomdef.o			\
		$(BUILD_DIR)/doomstat.o			\
		$(BUILD_DIR)/dstrings.o			\
		$(BUILD_DIR)/i_system.o			\
		$(BUILD_DIR)/i_net.o			\
		$(BUILD_DIR)/r_draw.o			\
		$(BUILD_DIR)/v_video.o			\
		$(BUILD_DIR)/f_finale.o			\
		$(BUILD_DIR)/f_wipe.o 			\
		$(BUILD_DIR)/d_main.o			\
		$(BUILD_DIR)/d_net.o			\
		$(BUILD_DIR)/d_items.o			\
		$(BUILD_DIR)/g_game.o			\
		$(BUILD_DIR)/p_saveg.o			\
		$(BUILD_DIR)/m_menu.o			\
		$(BUILD_DIR)/m_misc.o			\
		$(BUILD_DIR)/m_argv.o  			\
		$(BUILD_DIR)/m_bbox.o			\
		$(BUILD_DIR)/m_cheat.o			\
		$(BUILD_DIR)/m_random.o			\
		$(BUILD_DIR)/am_map.o			\
		$(BUILD_DIR)/p_ceilng.o			\
		$(BUILD_DIR)/p_doors.o			\
		$(BUILD_DIR)/p_enemy.o			\
		$(BUILD_DIR)/p_floor.o			\
		$(BUILD_DIR)/p_inter.o			\
		$(BUILD_DIR)/p_lights.o			\
		$(BUILD_DIR)/p_map.o			\
		$(BUILD_DIR)/p_maputl.o			\
		$(BUILD_DIR)/p_plats.o			\
		$(BUILD_DIR)/p_pspr.o			\
		$(BUILD_DIR)/nodereorder.o		\
		$(BUILD_DIR)/p_setup.o			\
		$(BUILD_DIR)/p_sight.o			\
		$(BUILD_DIR)/p_spec.o			\
		$(BUILD_DIR)/p_switch.o			\
		$(BUILD_DIR)/p_mobj.o			\
		$(BUILD_DIR)/p_telept.o			\
		$(BUILD_DIR)/p_tick.o			\
		$(BUILD_DIR)/p_user.o			\
		$(BUILD_DIR)/r_bsp.o			\
		$(BUILD_DIR)/r_data.o			\
		$(BUILD_DIR)/r_main.o			\
		$(BUILD_DIR)/r_plane.o			\
		$(BUILD_DIR)/r_segs.o			\
		$(BUILD_DIR)/r_sky.o			\
		$(BUILD_DIR)/r_things.o			\
		$(BUILD_DIR)/w_wad.o			\
		$(BUILD_DIR)/wi_stuff.o			\
		$(BUILD_DIR)/z_zone.o			\
		$(BUILD_DIR)/st_lib.o			\
		$(BUILD_DIR)/st_stuff.o			\
		$(BUILD_DIR)/hu_stuff.o			\
		$(BUILD_DIR)/hu_lib.o			\
		$(BUILD_DIR)/s_sound.o			\
		$(BUILD_DIR)/info.o			\
		$(BUILD_DIR)/sounds.o			\
		$(BUILD_DIR)/i_main.o

$(PROG_NAME).z64: $(BUILD_DIR)/$(PROG_NAME).elf
	$(OBJCOPY) $(BUILD_DIR)/$(PROG_NAME).elf $(BUILD_DIR)/$(PROG_NAME).bin -O binary
	$(N64_SYM) $(BUILD_DIR)/$(PROG_NAME).elf $(BUILD_DIR)/$(PROG_NAME).sym
	$(N64_DSOMSYM) $(BUILD_DIR)/$(PROG_NAME).elf $(BUILD_DIR)/$(PROG_NAME).msym
	rm -f $(PROG_NAME).z64
	rm -f $(BUILD_DIR)/$(PROG_NAME).dfs
	rm -f filesystem/identifier
	rm -f filesystem/*.wad
	cp iwads/$(IWAD).WAD filesystem
	echo $(IWAD).WAD > filesystem/identifier
	$(MKDFSPATH) $(BUILD_DIR)/$(PROG_NAME).dfs ./filesystem/
	$(N64TOOL) \
        --header $(HEADERPATH)/$(HEADERNAME) --title $(PROG_NAME) --toc --output $(PROG_NAME).z64 $(BUILD_DIR)/$(PROG_NAME).bin --align 8 $(BUILD_DIR)/$(PROG_NAME).sym --align 8 $(BUILD_DIR)/$(PROG_NAME).msym --align 16 $(BUILD_DIR)/$(PROG_NAME).dfs
	chmod ugo=rw $(PROG_NAME).z64
	$(CHKSUM64PATH) $(PROG_NAME).z64

$(BUILD_DIR)/$(PROG_NAME).elf : $(OBJS)
	$(LD) -o $(BUILD_DIR)/$(PROG_NAME).elf $(OBJS) $(LINK_FLAGS)

copy: $(PROG_NAME).z64
	cp $(PROG_NAME).z64 ~/

all: $(PROG_NAME).z64

clean:
	rm -f *.z64 *.elf *.msym *.sym *.bin *.dfs
	rm -f $(BUILD_DIR)/*

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
