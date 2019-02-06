The eSpeak homepage is:  http://espeak.sourceforge.net

## Compiling

```
mkdir build
cd build
cmake ..
make
```

The CMake build process produces the following binaries:

1.  "libspeak.so" library.  This is a shared library which contains
    the text to speech engine.  Its API is described in the file "speak_bin.h".

2.  "espeak" binary.  This is a small command-line program which is a front
    end to the libespeak library, which it needs.

3.  "speak"  binary.  This is a command-line program which contains the
    text to speech engine. It does not use the libespeak library.

When run, the "speak" and "espeak" programs expect to find the `espeak-data`
directory either in the user's home directory, or if not there, in /use/share.

## Compiling Data

You can modifiy spelling-to-phoneme rules and exceptions and re-compile this
data with the  speak  program (see  docs/dictionary.html). In summary, edit
the `dictsource/**_rules and dictsource/**_list` files and compile using:

```
espeak --compile=**
```

where `**` is the language code (eg. en for English).

In order to modify the sounds or other characteristics of phonemes, or
to add additional phonemes, another program "espeakedit" is needed. This
is provided in a separate package.


## PowerPC and Big-Endian Processors

The compiled data in the espeak-data directory is not binary compatible between
processors with different byte ordering.  For a PowerPC or other big-endian
processor use must use data which has been compiled (using espeakedit) on
a big-endian processor.

