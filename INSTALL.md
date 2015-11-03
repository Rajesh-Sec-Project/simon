### Source tree
The source is organized in two main folders :
+ **drone** : contains the source code for all embedded stuff
+ **host** : contains the source code for all offline stuff (including GUIs)

### Build system
The build system uses standard GNU Makefiles.
A makefile is built by first defining some variables :
+ **SRC_DIR** : directory containing source (*.cpp*) files, may have any level of subfolders
+ **INC_DIR** : directory containing header files
+ **TMP_DIR** : temporary directory to place object and dependency files into
+ **BIN_DIR** : directory to place the final binaries
+ **PRODUCT** : the final binary name
+ **PROJECT** : the sub-project's name
Then comes the specific compilation options :
+ **CX_FLAGS** : flags to pass to $(CX) (*see below*)
+ **LD_FLAGS** : flags to pass to $(LD) (*see below*)

Note that one can specify specific options for release / debug targets. Include paths are automatically appended to CX_FLAGS.

The compilation tools can be specified by modifying the following variables :
+ **CX** : the C++ compiler to use
+ **LD** : the linker to use ($(CX) recommended)

In the used makefiles all this stuff is defined in the *conf.mk* file. The actual Makefile includes *conf.mk*, then all the construction files from the *mk.d* folder.
In order :
+ **colors.mk** : defines some useful functions to output colored text on a terminal (uses *tput*)
+ **functions.mk** : defines the *invoke* function to permit a verbose switch (see the *Compilation* section)
+ **sources.mk** : extracts source files from $(SRC_DIR) and builds the object and dependency files lists
+ **patch.mk** : performs some patching to $(CX_FLAGS)
+ **toplevel.mk** : defines some top-level rules (see the *Compilation* section)
+ **production.mk** : defines the basic translation rules

### Compilation

Any sub-priject Makefile defines several top-level targets :
+ **release** : generate the final binary in release mode
+ **debug** : generate the final binary in debug mode
+ **clean** : remove the $(TMP_DIR) and $(BIN_DIR) folders (and therefore any build-generated file)

By default, our Makefiles output hides the real compiler / linker invocations behind colored text (for the sake of readability), if you really need to see them,
set the *VERBOSE* variable to 1, for example :
```bash
VERBOSE=1 make release
```

The top-level Makefile simply calls recursively GNU Make within every sub-project it finds. It also defines the **release**, **debug** and **clean** targets.
