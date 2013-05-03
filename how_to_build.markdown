How To Build
=================
Axe and Pick is built in C++ using the Qt libraries. The interface is entirely written in QML (Javascript).

#### Overview

You will need three basic things

  1. A C++ compiler of any kind.
  2. Qt Creator and Qt Libraries version 5.0 or above
  3. The source code from GitHub

There are a several ways to build this project. The instructions below describe what I consider to be the easiest way to get started.


Windows
------------

### Download Qt 5.0

Download [Qt 5.0 from qt-project.org][1].

If you don't plan to deploy (only to develop), you can choose any version. The only difference is that the MinGW package comes with the MinGW gcc compiler, so you can skip the compiler step below.

If you plan to deploy, pick the VS 2010 version.

### Choose a Compiler

If you chose the MinGW version, you can skip this step. MinGW is the gcc compiler and it comes packaged with Qt Creator.

If you chose the Visual Studio version of Qt, download and install [Visual Studio 2010][2].

### Get Source Code

The source code is available on GitHub. To copy it to your computer, you need to "clone" it.

Download [GitHub for Windows]. Follow the instructions. Details are available on the [GitHub website][5].

The repository is located here: `https://github.com/jmbeck15/axe-and-pick`

### Build

Open the `AxeAndPick.pro` file in Qt Creator. Press the Run button and Qt Creator will build and run the project.


Linux (KDE)
------------

This guide will focus on compiling for Kubuntu. The instructions should be roughly the same for other distributions.

### Get the gcc compiler

On the command line, run:

`sudo apt-get update`
`sudo apt-get install build-essential`.

### Download Qt 5.0

Download and install [Qt 5.0 from qt-project.org][1].

### Get Source Code

On the command line, run `sudo apt-get install git-core` to get git.
On the command line, run `git clone https://github.com/jmbeck15/axe-and-pick' to clone the code onto your computer.

### Build
In the cloned folder, open the `AxeAndPick.pro` file in Qt Creator. Press the Run button to build and run.


Apple OSX
------------

### Get the compiler command-line tools
Open Safari and go to the [Apple Developer website][6]. There should be a link called "Looking for additional developer tools? View Downloads". This will ask you to log in with your Apple ID.

Next, download and install the most recent version of the "Command Line Tools for Xcode" for your system. For example, "Command Line Tools (OS X Mountain Lion) for Xcode". This will give you all the tools required to build applications for OS X.

### Download Qt 5.0
Download and install [Qt 5.0 from qt-project.org][1].

### Get Source Code
The code is stored on GitHub, so you can choose to clone the code to your computer as you wish. However, the easiest way is to download GitHub for Mac. Once it's downloaded:

1. Run GitHub. Click through the Wizard without entering anything.
2. File -> Create New Repository. The Name should be "AxeAndPick", Description can be anything, and Local Path should not contain spaces.
3. Select View -> Repositories, to see your new repository. Double-click on it to open the repository properties, and select the Settings option on the left.
4. Under Primary remote repository, enter the path of the official axe-and-pick repository: https://github.com/jmbeck15/axe-and-pick.git
5. Pull the code to your computer by running Repository -> Pull. After this completes (it could take a minute or so), you should see a bunch of entries in your History.

Done!

### Build
Open Qt Creator and open the `AxeAndPick.pro` project file from your repository. Press the Run button to build and run.


Deployment
==========
This is a tricky topic, because there are a ton of different systems out there, and targeting them all with a single download can be tough.


[1]: http://qt-project.org/downloads "Qt Downloads"
[2]: http://www.microsoft.com/visualstudio/eng/downloads "Visual Studio Downloads"
[3]: http://windows.github.com/ "GitHub for Windows"
[4]: http://mac.github.com/ "GitHub for Mac"
[5]: https://github.com/ "GitHub"
[6]: https://developer.apple.com/xcode