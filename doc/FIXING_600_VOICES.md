Fixing the "600 Voices for the DX7" SYX files
---------------------------------------------

October 25, 2015

On the [Dave Benson DX7
Page](http://homepages.abdn.ac.uk/mth192/pages/html/dx7.html#books)
at, I found a link to patches for "600 Voices for the DX7" at
http://homepages.abdn.ac.uk/mth192/pages/dx7/DX7600.ZIP

Downloading and unzipping gives complaints from unzip for me.  Loading
them in emacs in hexl-mode shows that they are _close_ to SYX files,
but have a missing status byte and an extra couple bytes at the end.
So, they wouldn't load in the dexed synth with those differences.

Fixing them turned out to be simple with dxsyx after I turned some
errors into warnings.

I put the following code into a shell script called `fix1.sh`.  This
reads the .SND files, ignores the issues and writes out a
configuration file describing the data inside.

```
Build/Debug/dxsyx.exe syxs/600DXSND/AAAHGOOD.SND > AAAHGOOD.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/AAHGOOD2.SND > AAHGOOD2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/BASSES1.SND > BASSES1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/BASSES2.SND > BASSES2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/BRASS1.SND > BRASS1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/BRASS2.SND > BRASS2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/FX1.SND > FX1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/FX2.SND > FX2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/GTRS.SND > GTRS.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/HARPBELL.SND > HARPBELL.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/NEWAGE.SND > NEWAGE.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/ORGANS.SND > ORGANS.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/PERC1.SND > PERC1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/PERC2.SND > PERC2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/PIANOS1.SND > PIANOS1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/PIANOS2.SND > PIANOS2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/STRINGS1.SND > STRINGS1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/STRINGS2.SND > STRINGS2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/STUDIO.SND > STUDIO.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/SYNTHS1.SND > SYNTHS1.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/SYNTHS2.SND > SYNTHS2.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/SYNTHS3.SND > SYNTHS3.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/SYNTHS4.SND > SYNTHS4.CFG
Build/Debug/dxsyx.exe syxs/600DXSND/WIND.SND > WIND.CFG
```

Then, I created another script called `fix2.sh` that writes new .SYX
files based on the (unmodified) config files I just created with
`fix1.sh`.

```
mkdir -p syxs/DX7600FIX
Build/Debug/dxsyx.exe -s AAAHGOOD.CFG syxs/DX7600FIX/AAAHGOOD.SYX
Build/Debug/dxsyx.exe -s AAHGOOD2.CFG syxs/DX7600FIX/AAHGOOD2.SYX
Build/Debug/dxsyx.exe -s BASSES1.CFG syxs/DX7600FIX/BASSES1.SYX
Build/Debug/dxsyx.exe -s BASSES2.CFG syxs/DX7600FIX/BASSES2.SYX
Build/Debug/dxsyx.exe -s BRASS1.CFG syxs/DX7600FIX/BRASS1.SYX
Build/Debug/dxsyx.exe -s BRASS2.CFG syxs/DX7600FIX/BRASS2.SYX
Build/Debug/dxsyx.exe -s FX1.CFG syxs/DX7600FIX/FX1.SYX
Build/Debug/dxsyx.exe -s FX2.CFG syxs/DX7600FIX/FX2.SYX
Build/Debug/dxsyx.exe -s GTRS.CFG syxs/DX7600FIX/GTRS.SYX
Build/Debug/dxsyx.exe -s HARPBELL.CFG syxs/DX7600FIX/HARPBELL.SYX
Build/Debug/dxsyx.exe -s NEWAGE.CFG syxs/DX7600FIX/NEWAGE.SYX
Build/Debug/dxsyx.exe -s ORGANS.CFG syxs/DX7600FIX/ORGANS.SYX
Build/Debug/dxsyx.exe -s PERC1.CFG syxs/DX7600FIX/PERC1.SYX
Build/Debug/dxsyx.exe -s PERC2.CFG syxs/DX7600FIX/PERC2.SYX
Build/Debug/dxsyx.exe -s PIANOS1.CFG syxs/DX7600FIX/PIANOS1.SYX
Build/Debug/dxsyx.exe -s PIANOS2.CFG syxs/DX7600FIX/PIANOS2.SYX
Build/Debug/dxsyx.exe -s STRINGS1.CFG syxs/DX7600FIX/STRINGS1.SYX
Build/Debug/dxsyx.exe -s STRINGS2.CFG syxs/DX7600FIX/STRINGS2.SYX
Build/Debug/dxsyx.exe -s STUDIO.CFG syxs/DX7600FIX/STUDIO.SYX
Build/Debug/dxsyx.exe -s SYNTHS1.CFG syxs/DX7600FIX/SYNTHS1.SYX
Build/Debug/dxsyx.exe -s SYNTHS2.CFG syxs/DX7600FIX/SYNTHS2.SYX
Build/Debug/dxsyx.exe -s SYNTHS3.CFG syxs/DX7600FIX/SYNTHS3.SYX
Build/Debug/dxsyx.exe -s SYNTHS4.CFG syxs/DX7600FIX/SYNTHS4.SYX
Build/Debug/dxsyx.exe -s WIND.CFG syxs/DX7600FIX/WIND.SYX
```

After running the two scripts, I copied the new syxs/DX7600FIX files
to the right spot for Reaper and now I can listen to those synths--and
they are pretty fantastic!
