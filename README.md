# ProtocolLearn

![ProtocolLearn's Logo](https://github.com/ProtocolLearn/ProtocolLearn/blob/master/Images/Logo.png)

## Overview
ProtocolLearn is a C++ Network library for linux that pointed in Low level networking. ProtocolLearn is also providing an API for creating of new protocols.

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

## ProtocolLearn Parts:
ProtoclLearn has three general parts:
* [Utilities](https://github.com/ProtocolLearn/ProtocolLearn-Utilities): Contains the core of ProtocolLearn: Types, Addresses (eg. Ipv4Address), helpers (StringFunctions, VectorFunctions, ByteOrder) and more.
* [Protocols](https://github.com/ProtocolLearn/ProtocolLearn-Protocols): Contains implomented protocols (eg. Ethernet and Ipv4).
* [Examples](https://github.com/ProtocolLearn/ProtocolLearn-Examples) and [Libraries](https://github.com/ProtocolLearn/ProtocolLearn-Libraries): In "Examples", there are examples for the usage of ProtocolLearn's implomented protocols. "Libraries" contains examples too, but with an option to add the example into your project.

## Build
ProtocolLearn based on [qmake](https://en.wikipedia.org/wiki/Qmake) (qt projects).
First of all, you must have Linux machine, qmake, and gcc.

Open ProtocolLearn directory and run the build script:
```console
$ cd ~/ProtocolLearn/Build/
$ ./build.sh
```
It will take a few minutes.
Your executable files will be in the build directory.

Also, you can use Qt Creator to open, edit and config the project:
```console
$ qtcreator ProtocolLearn.pro
```

Set the build path to ProtocolLearn Path/Build (With the upper case 'B', the image isn't updated)
![Config ProtocolLearn in Qt Creator](https://github.com/shrek0/ProtocolLearn/blob/master/images/ProtocolLearnConfigQtCreator.png)

To compile your project, Just press CTL+b.
