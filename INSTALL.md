### Source tree
The source is organized in two main folders :
+ **drone** : contains the source code for all embedded stuff
+ **host** : contains the source code for all offline stuff (including GUIs)

### Build system
The build system uses standard GNU Makefiles.
A makefile is built by first defining some variables (usually in a *conf.mk* file included in the Makefile) :
+ **SRC_DIR** : directory containing source (*.cpp*) files, may have any level of subfolders
+ **INC_DIR** : directory containing header files
+ **TMP_DIR** : temporary directory to place object and dependency files into
+ **BIN_DIR** : directory to place the final binaries
+ **DBG_DIR** : dirextory to place debug logs
+ **PRODUCT** : the final binary name
+ **PROJECT** : the sub-project's name

Then comes the specific compilation options :
+ **CX_FLAGS** : flags to pass to $(CX) (*see below*)
+ **LD_FLAGS** : flags to pass to $(LD) (ip address of remote platform, ports, etc.)
+ **QMAKE_FLAGS** : flags to pass to qmake (for Qt projects)

Note that one can specify specific options for release / debug targets. Include paths are automatically appended to CX_FLAGS.

The compilation tools can be specified by modifying the following variables :
+ **CX** : the C++ compiler to use
+ **LD** : the linker to use ($(CX) recommended)

In the used makefiles all this stuff is defined in the *conf.mk* file. The actual Makefile includes *conf.mk*, then some of the construction files from the *mk.d* folder.
Note that some parts are used for cross-compiled projects but also for host Qt projects.
In order :
+ **colors.mk** : defines some useful functions to output colored text on a terminal (uses *tput*)
+ **functions.mk** : defines the *invoke* function to allow a verbose switch (see the *Compilation* section)
+ **sources.mk** : extracts source files from $(SRC_DIR) and builds the object and dependency files lists
+ **patch.mk** : performs some patching to $(CX_FLAGS)
+ **toplevel.mk** : defines some top-level rules (see the *Compilation* section)
+ **remote.mk** : defines remote platform only make targets (namely *push* and *gdb*, see the *Debugging* section)
+ **production.mk** : defines the basic translation rules
+ **all.mk** : defines the *all* (default) target, include before *conf.mk*
+ **qt-toplevel.mk** : defines the top-level targets for Qt projects
+ **qt-qmake.mk** : defines the qmake-related targets for Qt projects
+ **qt-patch.mk** : performs some patching and generation of variables for Qt projects

### Compilation

Any sub-project Makefile defines several top-level targets :
+ **-> For all projects**
+ **release** : generate the final binary in release mode
+ **debug** : generate the final binary in debug mode
+ **clean** : remove the $(TMP_DIR) and $(BIN_DIR) folders (and therefore any build-generated file)
+ **help** : display some help about available targets
+ **-> For embedded projects**
+ **push** : push binary to remote platform (see the *Debugging* section)
+ **gdb** : start a remote debugging session (see the *Debugging* section)
+ **shell** : start a remote telnet session
+ **-> For Qt projects **
+ **qmake** : execute qmake and generate the Qt project's Makefile
+ **qbuild** : build the qmake'd project
+ **qclean** : clean the qmake'd project

By default, our Makefiles output hides the real compiler / linker invocations behind colored text (for the sake of readability), if you really need to see them,
set the *VERBOSE* variable to 1, for example :
```bash
VERBOSE=1 make release
```

The top-level Makefile simply calls recursively GNU Make within every sub-project it finds. It also defines the **release**, **debug** and **clean** targets.

### Debugging

The build system provides facilities to push binaries to the remote platform, and to start a debugging sessions.
Bash scripts in the top-level *debug* directory implements these functions :
+ **conf.sh** : defines some configuration variables (ip address of remote platform, ports, etc.)
+ **vars.sh** : extracts some informations (like binary name, etc.) from the *conf.mk* files, and creates needed environment variables for other scripts
+ **push.sh** : creates a TCP connection with the remote platform, and pushes the binary to the *DIST_PATH* directory
+ **debug.sh** : starts both the local and remote gdb process, logging remote server's output to the *PREFIX/gdbserver.log* file

The top-level scripts **push.sh** and **debug.sh** expects the *PREFIX* variable to be defined in order to know which sub-project to use.
Push and debug invocations are provided by every sub-project's Makefile (see the *Compilation* section).