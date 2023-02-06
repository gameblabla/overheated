This is an improved version based on a game by BrokenTestType.
It got a Dreamcast port as well as other ports such as OpenDingux.

It still works on PC but given that it uses SDL 1.2,
it might need a switch to SDL2.

That will probably never happen :P

About the Dreamcast port :
This port was a bit challenging to do at first but mostly from a performance point of view.
A few shortcuts had to be taken, especially in regards to sound.
SDL_mixer wasn't going to cut it so had to use the native KOS functions,
leaving the graphics to SDL 1.2, unfortunately entirely done on the CPU but
still manages to run smoothly at 60 FPS and 240p, barely...

Music is being streamed from the CD as CDDA.
Even ADX was enough to add slowdowns to the game so i did not go that route.

Here's the original readme by BrokenTestType

-------------------------------------------------------------------------
Overheated 0.2.0 (2017-05-13)(YYYY-MM-DD)
-------------------------------------------------------------------------

-------------------------------------------------------------------------
Copyright (C) 2016-2017 BrokenTestType
d3592b10bc20b427ca29cd2ff9dbeb4087a3cbfbf423d72d5d8bb5433b8d2f4b
-------------------------------------------------------------------------

OverHeated source code is licensed under GPL2
OverHeated assets are licensed under Creative Commons licenses.

Thank you so much for downloading and playing OverHeated.
I sincerely hope that you enjoy this game.

-------------------------------------------------------------------------
*** About the Game ***
-------------------------------------------------------------------------

OverHeated is a homebrew caravan shooter game.
The objective of this game is to score as many points as you can before the time runs out.
play over and over again and try to get the highest score possible.
-Getting a bonus ball will give you 1000 extra points
-If you get a power-up cube and your weapon level is already maxed out you will get extra points
-The faster you destroy enemies the faster newer ones will spawn
 The more enemies you destroy the more points you get!

*Currently there are no bonus for the number of remaing lives or remaing time.

-------------------------------------------------------------------------
*** Running on GNU/Linux ***
-------------------------------------------------------------------------

You will need to install SDL1.2 and SDL_mixer from the repositories.
On debian:

$sudo apt-get install libsdl1.2debian
$sudo apt-get install libsdl-mixer1.2

If you want to execute the 32bit version (overheated.x86) on a 64bit distribution 
be sure to install the 32bit version of the libraries (if you are using a multiarch distro)
(*On debian libsdl1.2debian:i386 and libsdl-mixer1.2:i386)

If there are any conflicts between your version of the libraries and the executable I recommend you to build from source.
*As a last resort and in case that nothing else works, just use WINE, the game runs great on WINE.

*NOTE: 
    If you launch the game from a terminal you should be in the same working directory than the game executable
    also the game executable should be on the same directory where "data/" is located.

-------------------------------------------------------------------------
*** Running on Windows ***
-------------------------------------------------------------------------

Just execute the executable!
It should work on Windows 32 and 64bits.

-------------------------------------------------------------------------
*** Influences (and some random babble and opinions from the author) ***
-------------------------------------------------------------------------

This game is heavily influenced by Super Star Shooter and all the soldier games from hudsonsoft.

/blog
I been playing Super Star Shooter(Wii Version) over and over for almost 4 years, It just can't get bored from it.
Half of the game appeal is the music, SSS music is soo damn good, pure FM awesomeness
I also tried SSS Gaiden on IOS, the arranged soundtrack is phantastic and the touch controls are actually not bad.
You can also play the x68000 version (or the GBA version) on your computer.
You have 5 or 2 minutes of free time? Just play SSS!!
in conclusion Super Star Shooter is an EXCELLENT GAME, one of my favs, highly recommended!
If you happen to like my game you can try all the soldier games by hudsonsoft.
All the shooters by hudsonsoft are really excelent and the music is really good, specially soldier blade which I consider the pinnacle of the series.
You can also try Zero Rei/Zero Master Striker,Soldier Force ,Caravan Star II, Nezxr, GunHead and Alzadick
There is also Blade Buster, a homebrew game for the NES that is just INCREDIBLE at the technical department( and it actually works on real hardware).
Blade Buster is freeware(IIRC), so just try it.

*imho my game is not bad at all but I consider that almost all the games that I cited are better than mine (-.-')

-------------------------------------------------------------------------
*** A Few words from me to the people that are willing to read my code ***
-------------------------------------------------------------------------

First of all, I want to apology for all my flaws and evil doings:
-The code is full of broken english comments.
-I didn't stick to any code conventions at all, I just kinda made my own on the go.
-I didn't use any vcs (I just made new directories for each version -.-| ).
-My knowledge of make is poor so the makefiles are not very good.
-There are a few source modules that are reused from really old projects,
 they "just work" but Im sure that I could have written something better instead
 (for example SDL_Text.c and str_utils.c)
-There were a lot of times when I was just being lazy and writting shit code
 (especially when I was working on the enemies logic)
-As the development progressed loadData.c turned into a bloated omnipotent spaghetti.
-Also apology for A LOT of other stupid design choices due my current lack of skill and experience.

I'm very conscious of all these issues and I'm working hard to solve all of them and improve myself.
I want to git gud in the future, My next project will be far better than this one!

-------------------------------------------------------------------------
**** Building for Linux/Unix ****
-------------------------------------------------------------------------

To build this on GNU+systemd+linux and other Unixes you will need the development packpages
for SDL1.2 and SDL_mixer.

On Debian based distros you will need to install:

libsdl-mixer1.2-dev
libsdl1.2-dev

then just use the makefile "make.UnixPC":
$make -f make.UnixPC

In case that the makefile gives you trouble, just:
$cc *.c `pkg-config --cflags --libs SDL_mixer sdl` -lm -o overheated

*If you use other nondebian based distros with other package managers, BSD or some esoteric Unixes,
the name of the packpages may differ (Maybe your system don't even have a package manager to begin with).
In that case, well, You probably know more than me anyways so you can figure out how to build from source 
on your own (this project is pretty simple after all).

-------------------------------------------------------------------------
**** Building for Windows 32bits ****
-------------------------------------------------------------------------

*I cross-compiled everything on GNU/Linux so I don't know what you have
to do to build from source using Windows.
You can try to install mingw32 on Windows and see if everything just werks.
Maybe you can just import everything in VisualStudio (or any other fancy IDEs)
and then set the value of CFLAGS,LIBS or whatever other env variables your IDE uses.

All the headers and library objects that you need are inside the subdirectory "src/win/"
just install mingw32 and use the makefile "make.win32"

*Don't forget to copy the dlls that are inside "win/dll/" in the same folder where the executable is located

-------------------------------------------------------------------------
**** Building for GCW-Zero on Linux/Unix ****
-------------------------------------------------------------------------

Extract the toolchain in your /opt folder
Then use the makefile "make.gcw0"

$make -f make.gcw0

-------------------------------------------------------------------------
**** Porting ****
-------------------------------------------------------------------------

This game is written in C and only uses SDL1.2 and SDL_mixer
So porting it to pretty much anything that runs embedded Linux or Windows should be fairly trivial
I actually compiled the game for GP2X WIZ and it works pretty slow with the CPU at 500mhz
(maybe at 700mhz it would run fine)
Porting to pandora and pyra should be just a matter of recompilate the code.
Porting to Dreamcast should be simple but I don't know how good SDL performs on Dreamcast.
(^I want to try to port this to Dreamcast to see how good it runs)
As far as I know SDL implementation on Wii and GC have some features missing 
and the performance is not very good.

-------------------------------------------------------------------------
**** Credits ****
-------------------------------------------------------------------------

Developed by Dangerous Delusions Levels homebrew group

DDL Staff:
BrokenTestType (Only member at the moment (-.-')

Code & Graphics:
    BrokenTestType"

Music:
    sawsquarenoise
    rolemusic.sawsquarenoise.com

CC assests:

    Sound effects by SubspaceAudio
    Licensed under CC0 (Public domain)
    https://juhanijunkala.com/
    https://opengameart.org/content/512-sound-effects-8-bit-style

    mig68000_8x16 font by zingot
    Licensed under CC-BY 3.0
    http://www.zingot.com/
    https://opengameart.org/users/zingot
    https://opengameart.org/content/bitmap-font-pack

    ASCII Bitmap Font "cellphone" by domsson
    Licensed under Public domain (CC0)
    https://opengameart.org/users/domsson
    https://opengameart.org/content/ascii-bitmap-font-cellphone

    *If I included any asset in the game that is not CC
     please notify me at brokentesttype@gmail.com

-------------------------------------------------------------------------
**** Contact ****
-------------------------------------------------------------------------

If you want to ask me anything (or yell at me for anything I did wrong)
you can do so at brokentesttype@gmail.com
It's not guaranteed that I will reply but you can always try!

*Also, please send me bug reports, with replay files attached if possible.










