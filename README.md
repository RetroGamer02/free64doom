# Free64Doom

(Updated 2023/09/10)

## How to Build
Setup libdragon (unstable branch): https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon

Create a build directory in repo directory.

Select the IWAD you wish to use by editing Makefile and setting `$(IWAD)` (either `FREEDOOM1` or `FREEDOOM2`).

And run make.

Result is `$(IWAD).z64` ready to copy to flash cartridge or run in emulator.

Example for building FreeDoom Phase 1 from repo directory:

`mkdir build`

`IWAD = FREEDOOM1`

`make`

resulting in `FREEDOOM1.z64`.

## SAVEGAMES

Free64Doom uses the Controller Pak to save and load your game progress from the Save Game / Load Game menu options.

One savegame slot is presented, mapped to a single note on the Controller Pak.

The name of the note is the same as the game version (`$IWAD`) you are playing.

When you save or load a game, you will see a message in the Doom HUD if it is successful:

`SAVED GAME TO MEMPAK`

`LOADED GAME FROM MEMPAK`

Savegames are compressed using the lzfx library. However, the compressed saves are still large relative to the capacity of a fully-formatted Controller Pak. 

In the event that there is not enough free space on the Controller Pak to save the game, you will see a message in the Doom HUD:

`NOT ENOUGH SPACE FOR SAVE (NEED #, HAVE #)`

It is very likely you will see this message if you have notes from other games on your Controller Pak, so it is desirable to play with a dedicated, initially empty
Controller Pak. 

It is possible that there are rare occasions where this may happen even with an empty Controller Pak. 

If it does, kill some more enemies, pick up some more items, try again. :-)

## CONTROLS

D-PAD UP / ANALOG STICK UP :: move forward

D-PAD DOWN / ANALOG STICK DOWN :: move backward

D-PAD LEFT / ANALOG STICK LEFT :: turn left

D-PAD RIGHT / ANALOG STICK RIGHT :: turn right

L TRIGGER :: strafe left

R TRIGGER :: strafe right

C LEFT :: switch to previous weapon

C RIGHT :: switch to next weapon

C UP :: toggle auto-map

C DOWN :: ENTER key

Z :: toggle run on/off (defaults to on)

A :: shoot

B :: use (open doors, flip switches)

START :: ESCAPE key
