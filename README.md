---------------
-Free64DOOM README-
---------------


Updated 2023/09/05


The Freedoom IWAD was made thanks to [the Freedoom team](https://freedoom.github.io), a [free and open-source](https://github.com/freedoom/freedoom/blob/master/COPYING.adoc) game based on the [Doom engine](https://doomwiki.org/wiki/Doom_engine).

-------------------------
-git repository contents-
-------------------------

`README.md` is this file

`src` directory contains all 64Doom C source, header files, menu graphics and Makefile used to build 64Doom

`doc` directory contains many text files including

`  ORIGINAL_README.TXT` is the original `README.TXT` from the 1990s DOOM open source release
  
`  DOOMLIC.TXT` is the DOOM open source license documentation
  
`  CREDITS.TXT` which provides attribution for various components and code contributors
  
`  LICENSE_generic-hashtable.TXT`, `README_generic-hashtable.TXT` required attribution for Hashtable implementation used
  
`  GPLV3.TXT` is a copy of the GPL V3 License as required

--------------
-how to build-
--------------
Setup libdragon: https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon

Get a copy of a supported version of Doom (Doom shareware, retail, Ultimate Doom, Doom II, Plutonia, TNT).

Export two environment variables:

`IWAD_DIRECTORY` -- the path of the directory that contains your IWAD file

`IWAD_PREFIX` -- the actual IWAD filename prefix (one of `DOOM1`, `DOOM`, `DOOMU`, `DOOM2`, `PLUTONIA`, `TNT`)

And run make.

Result is $(IWAD_PREFIX).z64 ready to copy to dev cart or run in emulator.

Example for building DOOM2 from repo directory, with IWAD files located in your home directory under `IWADS`:

`cd src`

`env IWAD_DIRECTORY="~/IWADS" IWAD_PREFIX="DOOM2" make`

resulting in `DOOM2.z64`

If you want to build for a different game version, be sure to run make clean first. Environment variables must be set for make clean as well.

`env IWAD_DIRECTORY="." IWAD_PREFIX="." make clean`

----------
-CONTROLS-
----------

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

Z :: toggle run on/off

A :: shoot

B :: use (open doors, flip switches)

START :: ESCAPE key

L + R :: enable/disable debug output and GOD MODE

Special Thanks

eightbit - Starting the Free64Doom Project.

Immorpher - Figuring out how to convert the music.

jnmartin who made the 64doom engine this project would not be possible without it!


Enjoy.
