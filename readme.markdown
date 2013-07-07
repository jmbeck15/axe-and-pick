Axe and Pick
============
A delightful saved-game hacker for [Timber and Stone][1].

![Screenshot](http://i.imgur.com/PUh7DHPl.png "Axe and Pick main screen")

How To Run
---------------------------
1. Unpack anywhere you want. Run the executable.
2. If it doesn't run in Windows, install the [Microsoft Visual C++ 2010 Redistributable Package][2].

How to Use
---------------------------
1. When the program first runs, it'll as you for the location of the saves.sav file.
2. Everything else should make sense. Click the save icon to save.
3. When you save, you have to reload the game in Timber and Stone.

How to Build
---------------------------
See the `how_to_build.markdown` document for details.

Change Log
---------------------------
#### Version 1.8 (for T&S v1.3)
    Added new Goblins (Marauder, Archer, Mounted)
    Added new Spiders (the larger one and the Matriarch)
    Added new Skeletons (Bloodthirsty and Mace-equipped)
    Added new Wolves (type 1 and 2, yet unnamed)
    Added Seedlings, Flaming arrows, Copper Spear, Iron Voulge, Bronze Glaive, Steel Halberd
    Doubled resource cap at 60k
    Added female units randomly (40% chance), and added female name file
    When adding human units, experience level is set to an "expert level" based on profession (finally!)
    New human experience levels were bumped up slightly
    Bad guys spawn as a leader 5% of the time
    Added random rotation for new bad guys (aids initial behavior when playing)
    Bad guys spawn with 100% health (instead of being wounded, earlier)

#### Version 1.7 (for T&S v1.2)
    Fixed Adventurer and updated some fields.

#### Version 1.6 (for T&S v1.1)
    Fixed patrol settings for new unit save format.

#### Version 1.5 (for T&S v1.1)
    Fixed obscure crash.
    Updated icon for OSX and file details for Windows

#### Version 1.4 (for T&S v1.1)
    Updated for Timber and Stone v1.1
    Changed names.csv to names.txt for clarity

#### Version 1.3 (for T&S v1.0)
    Added coffee. Also, new units are automatically fed coffee to improve performance.

#### Version 1.2 (for T&S v1.0)
    Fixed animals dying for no good reason.

#### Version 1.1 (for T&S v1.0)
    Updated for T&S v1.0 release.

#### Version 1.0 | April 2, 2013
    Updated for Timber and Stone v0.9

#### Version 1.0 | April 2, 2013
    Updated for Timber and Stone v0.9
    Added support for adding and removing all unit types.
    Added error handling to prevent accidental save corruption for older versions.
    New icon, tweaked visuals

#### Version 0.5 | March 8, 2013
    Updated for Timber and Stone v0.5
    Added Animal Hide and Spider Silk
    Fixed Hoes.

#### Version 0.4 | February 24, 2013
    Added a heartbeat animation to the save button
    Nevermind! I changed it to a fading green save icon.

#### Version 0.3 | February 24, 2013
    Tested with Beta 0.3
    Added fishing poles.
    The search string is: <name> <type> <quantity>
    Added regular expressions to search, eg. typing: steel.*armor will give you all steel armor.


#### Version 0.2 | February 23, 2013
    Fixed some distribution problems.
    Fixed god-awful bug that happened because I "wanted to make it pretty". That'll show me.

I got it to work on Windows 7, 32 and 64 bit. There is ONE dependancy that you need to have installed: it's called the "[Microsoft Visual C++ 2010 Redistributable Package][2]".

This does NOT run on Windows XP. I have absoutly no idea why, but I'll get it eventually.

#### Version 0.1 | February 17, 2013
    Works with Beta 0.2 and under (I think)
    First Version! Everything is new!

[1]: http://www.timberandstonegame.com/ "Timber and Stone"
[2]: http://www.microsoft.com/en-us/download/details.aspx?id=5555 "Microsoft Visual C++ 2010 Redistributable Package"
