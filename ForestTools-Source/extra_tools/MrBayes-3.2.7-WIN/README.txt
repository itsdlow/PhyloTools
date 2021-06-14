# (Provisional) Windows version of MrBayes 3.2.7a

- Last modified: Wed Mar 20, 2019  05:17PM
- Sign: JN

## Description

The Windows executable (32bit or 64bit) is the serial version of MrBayes. It 
runs on a single core and is primarily intended for users who want to get 
familiar with the program. It is going to be slow if you try to run a large 
analysis with many parallel chains. For such analyses, please use the MPI 
version of MrBayes on a Unix or Macintosh machine or on a computer cluster.

If you locate the executable and double-click it, the working directory will be 
the same directory where the executable is located. To change the directory 
used by MrBayes, use the `set` command. For instance, if you want to use a 
folder on the Desktop called "MyAnalyses" instead of the default folder, type 
`set dir=<path-to-my-desktop>\MyAnalyses` in the MrBayes command window, where 
`<path-to-my-desktop>` is the path to your desktop directory.

## Files

- Executable files (ending in `.exe`) are in folder `bin`.
- Documentation are in folder `doc`.
- Examples are in folder `examples`.


## Notes

The C code for MrBayes 3.2.7a (without BEAGLE) was cross compiled
on Linux (gcc version 7.3.0) for MS Windows.

Github code version:

    Commit: ec103e5fc6cf6e8bac20f2830aa39d53eb677662
    Date:   Wed Mar 6 17:45:03 2019 +0100

Compile commands:

    ./configure --build=i686-pc-linux-gnu --host=x86_64-w64-mingw32 \
                --prefix=~/tmp/MrBayes-3.2.7-WIN --program-suffix=.3.2.7-win64 \
                --without-beagle --without-readline
    make
    make install
    make clean
    ./configure --build=i686-pc-linux-gnu --host=i686-w64-mingw32 \
                --prefix=~/tmp/MrBayes-3.2.7-WIN --program-suffix=.3.2.7-win32 \
                --without-beagle --without-readline
    make
    make install

