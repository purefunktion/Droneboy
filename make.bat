set local
SET PATH=%PATH%;C:\path\to\gdbk\bin
lcc -Wp-Iinclude -o droneboy.gb src/main.c src/tilesandbackgrounds.c src/intro.c src/volume.c src/duty.c src/frequency.c src/common.c
lcc -mgbz80:ap -Wp-Iinclude -o droneboy.pocket src/main.c src/tilesandbackgrounds.c src/intro.c src/volume.c src/duty.c src/frequency.c src/common.c
