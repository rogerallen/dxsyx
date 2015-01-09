# dxsyx

dxsyx - a C++ library for manipulating DX7 SysEx files.

I created this after using the Dexed VST in Reaper.  See https://github.com/asb2m10/dexed
I wanted an easier way to create my own syx files.  And I was looking for a C++ project.

## Installation

Build via the Xcode project or Makefile.

## Usage

If you want to get a list of all voices in a syx file or files:

    $ dxsyx file1.syx file2.syx

If you want to see every individual setting for each voice:

    $ dxysx -v file1.syx file2.syx

If you want to select voices and create your own syx file:

    $ dxsyx file1.syx file2.syx file3.syx > config.txt

Now, edit config.txt to select only the 32 voices you desire

    $ dxsyx -s config.txt my.syx 
    
This creates your custom my.syx file with your selected voices.

## Options

    $ dxsyx [options] file1.syx file2.syx ...
    
    none = print out voices
    -h = help (this message)
    -v = print full data as YAML
    -s config.txt output.syx = select specific voices, output to new syx file

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
