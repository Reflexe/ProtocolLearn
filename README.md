# ProtocolLearn

![ProtocolLearn's Logo](https://github.com/ProtocolLearn/ProtocolLearn/blob/master/Images/Logo.png)

## Overview
ProtocolLearn is a C++ Network library for linux that pointed in Low level networking. ProtocolLearn also provides a API for creating of new protocols.

## Get ProtocolLearn:
Note: ProtocolLearn uses [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules), therefore, the regular 'git clone [url]' command will download only the base repository without the submodules. 

To download ProtocolLearn, run the following lines:

```console
git clone --recursive git@github.com:ProtocolLearn/ProtocolLearn.git
```

## Getting Started
You can see the [Getting Started](https://github.com/ProtocolLearn/ProtocolLearn/wiki/Getting-Started) article in the wiki.
If you want to see examples of the using of ProtocolLearn, you can open the [Examples](https://github.com/ProtocolLearn/ProtocolLearn-Examples) repository.
If you interesting in a specific type of ProtocolLearn, please visit [ProtocolLearn's Wiki](https://github.com/ProtocolLearn/ProtocolLearn/wiki).

## Build
ProtocolLearn based on [qmake](https://en.wikipedia.org/wiki/Qmake) (qt projects).
First of all, you must have Linux machine, qmake, and gcc.

Open ProtocolLearn directory and run the build script:
```console
$ cd ~/ProtocolLearn
$ ./build/build.sh
```
It will take a few minutes.
Your executable files will be in the build directory.

Also, you can use Qt Creator to open, edit and config the project:
```console
$ qtcreator ProtocolLearn.pro
```

Set the build path to ProtocolLearn Path/build:
![Config ProtocolLearn in Qt Creator](https://github.com/shrek0/ProtocolLearn/blob/master/images/ProtocolLearnConfigQtCreator.png)

To compile your project, Just press CTL+b.
