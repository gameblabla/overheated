Overheated for Sega Dreamcast v1.0
===================================

Game by BrokenTestType
Port by gameblabla

Here's a port Dreamcast of a nice carnival schmup by BrokenTestType.
Back in 2017, i helped him on the GCW0 port of it and now it's also playable on the Dreamcast with some changes,
including VMU support for replays. (Saved on A1 slot)

The Dreamcast port also makes use of a stripped down version of SDL DreamHAL to allow it to run as fast as possible
as well as using more aggressive opts.
Using the stock port of SDL will result in extra slowdowns and increased binary size.

Controllers, Mouse and keyboard are supported.
However, the mouse might be a bit finicky to some people's likings so if that's an issue, use a controller instead.
I also have to note that you can play the game on its own with just a mouse but you may still want to have a 
controller or keyboard plugged in to navigate the menu as doing so with the mouse can be finicky.

Game has been tested on GDEMU but should also work on CD.
Game might work on the SD card adapter but because of its nature, you won't get any CDDA music.
Sorry guys but even ADX has a performance hit... :/

==============================
Technical details/development
==============================

Originally, this game used SDL for input, graphics and audio.
For the Dreamcast version, input and audio both have been switched to native, leaving the graphics side of things to SDL still.
The reason why this was done this way is because the game does actually hit the CPU pretty hard when it comes to spawing enemies on screen,
so we want to leave as much CPU time as possible to the SH-4 CPU.
Ideally, this game should be using the PVR chip directly for graphics, as SDL is software rendered only but that would be a lot more work than it is worth for that game.
(Perhaps it is worth considering in the future. Mybe do so within SDL itself or a wrapper of sorts?)

For this reason, CDDA music is used as the AICA chip takes care of decoding it without CPU intervention.
ADX was another consideration but tho it has a 2-4% CPU usage, it is high enough to cause slowdowns in the game.
The AICA hardware also has hardware decoding for ADPCM so we are taking advantage of this for the sound samples.
At the time i'm writing this, KOS's tool had an issue with converting ADPCM samples due to not working properly on 64-bits architectures.
This has been fixed with my PR (https://github.com/KallistiOS/KallistiOS/pull/65) and it is my hope it will get merged eventually.

OPUS is not recommended unless size is of utter importance and you can leave some 20% of CPU time to it and OGG
should not be used at all, as it can spike up to 60% of CPU usage.
I'm still surprised some homebrew games keep using it as even OPUS outperforms OGG and has a smaller size.
(I knew this way back in 2017 when OPUS was made available and i used it for Hydra Castle Labyrinth)
There is no reason to use OGG anymore on the Dreamcast. (or even on modern platforms for that matter)

Filesystem access and modifications related to it can be a huge pain on the Dreamcast.
For example, if you are currently streaming music from the CD and you then attempt to load files from the CD without pausing the music,
the game will either crash or mute itself.
Additionally, if you are using ADX, you must make sure to leave the first 4 channels available and use the rest for sound effects etc...
as otherwise it will conflict with the ADX player and poop itself in various ways.

In general for this game, this was my strategy :
- /cd/ for Read only files
- /ram for files that need write access and/or used for saves to be pushed to the VMU
- /rd faster RAM access. (but you cannot unload the file)

For sound files, it's recommended that you just load them straight from the CD.
For saves, i use a temporary copy of it in /ram and then push it to the VMU when i need to save.
This way, we barely have to deal with the VMU and we can still keep most of the code as is.

For the controls, this actually took quite a bit of time to get it right.
Controllers are actually quite simple to support.
The mouse can be tricky to implement depending on the game, especially if the game only has digital controls in mind.
For this game, i had no choice but to tweak the speed of the ship when the mouse is being used.
Even then, it's not quite perfect as the ship moves in "steps" rather than in fluid movements therefore i had to limit the speed of the ship as well to a +3.
Keyboard is easy but the documentation was all over the place. In a nutshell, matrix should be used for repeated keys (sort of like a game controller).
If you don't care for repeated keys, you can use kbd_get_key or the other function. (but those might not be appropriate)
if you only use a bunch of keys, you probably only want to poll some of them to reduce CPU usage, not all 255 keys.

SDL itself also needed to be modified. 
Given that i was only using it for graphics, i disabled pretty much everything except threads, timer and video.
I also took out the code for showing a cursor on screen as it was not useful and can actually have a small performance hit on software rendering.
Same for the input code, which was trying to pull all 255 keys, so it was disabled as well.
The end result is much decreased binary size and a noticeable speedup as well. Less code to run means more for the game, right ? :P
SDL was also recompiled with Ofast as well. I suspect this and the above is why i saw a noticeable change in performance.
