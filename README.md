# dxsyx

dxsyx - a C++ library for manipulating DX7 SysEx files.

I created this after using the Dexed VST in Reaper.  See https://github.com/asb2m10/dexed
I wanted an easier way to create my own syx files after finding it difficult to keep track of the voices I found in the .syx files I found.
One good source is http://homepages.abdn.ac.uk/mth192/pages/html/dx7.html

Plus, I was looking for a C++ project to check out C++11 coding.

## Installation

Build via the Xcode project or Makefile.  Tested on Mac OS X, but should work with any C++11 compiler. 

## Options

     $ dxsyx [options] [file1.syx file2.syx] ...
     
     (none)                   : print out voices
     -h                       : help (this message)
     -v                       : print full data as YAML
     -s config.txt output.syx : select specific voices, output to new syx file
     -b config.txt output.syx : select specific voices, breed 32 random voices

## Usage

If you want to get a list of all voices in a syx file or files:

    $ dxsyx file1.syx file2.syx

The output from this command is the basis for all the config.txt files used as input.  It is a CSV-formatted line for each voice containging first, the 10-character ASCII name (any chars < 32 are changed to '_'), next the voice number and finally the full path to the syx file containing the voice.

    OEOEOE    ,0,/Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/things.syx
    TAKINGOFF?,1,/Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/things.syx
    CHOPPER   ,2,/Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/things.syx
    ...

If you want to see YAML-formatted individual settings for each voice:

    $ dxysx -v file1.syx file2.syx

Here is a snippet:

    filename: /Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/things.syx
      voice_name: OEOEOE    
        op6_eg_rate_1: 99
        op6_eg_rate_2: 99
        op6_eg_rate_3: 99
        op6_eg_rate_4: 99
        op6_eg_level_1: 99
        op6_eg_level_2: 99
        op6_eg_level_3: 99
        op6_eg_level_4: 0
        ...

If you want to select voices and create your own syx file:

    $ dxsyx file1.syx file2.syx file3.syx > config.txt

Now, edit the lines of config.txt to leave only the 32 voices you desire (do not change any text or add any comments).

    $ dxsyx -s config.txt my.syx 
    
This creates your custom my.syx file with your selected voices.

Next, how about "breeding" your own sounds?  Create a config.txt file with the "parents" of the voices you would like to blend together.

    $ dxsyx -b parents.txt mutants.syx

Each of the 32 "mutants" you create will be a random blend of 2 random parents in the config.txt file.

If parents.txt contains:

    DL RHODES3,30,/Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/road2.syx
    METAL BOW ,19,/Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/things.syx
    RUMBLE    ,7,/Users/rallen/Music/VST/dexed-0.8.0/Dexed_cart/Sysex/things.syx

Mutants.txt might contain something like:

    12MBLEBO  ,0,mutants.syx
    01 ALODOW3,1,mutants.syx
    20 BLE E 3,2,mutants.syx
    ...
    12TALEB   ,31,mutants.syx

Where the first two characters of the name indicate the parents of that voice.  These voices are completely random blends and might sound great or might sound awful.  Listen to the results, keep breeding from your best sounds and evolve your own, unique sounds!

## License

Copyright © 2015 Roger Allen

Dxsyx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dxsyx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dxsyx.  If not, see <http://www.gnu.org/licenses/>.
