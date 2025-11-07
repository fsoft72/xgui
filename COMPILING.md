COMPILING

In order to compile XGui, at the moment, you have to tinker some files.
We are not providing the standard "./configure && make && make install" process at this time.

## Platform-Specific Documentation

For detailed macOS compilation instructions, see [COMPILING_MACOS.md](COMPILING_MACOS.md)

## Dependencies

### Linux/GTK+
	1 - libgtk+-2.0-dev
	2 - libxml2-dev
	3 - swig (optional for Python bindings)
	4 - python-dev (optional for Python bindings)

### macOS
	1 - Xcode Command Line Tools (includes clang, libxml2)
	2 - swig (optional for Python bindings, via Homebrew)
	3 - python3 (optional for Python bindings, via Homebrew)

## Available Makefiles

We are providing different Makefiles:

	1 - Makefile.gnustep	 - for compiling for GNU Step
	2 - Makefile.gtk	 - for compiling for GTK+
	3 - Makefile.osx	 - for compiling for macOS (latest versions supported)

you can start compiling using:

	make -f (your makefile name here)

or, since I think you'll just compile for one single platform,
create a symlink of your desider makefile, for example:

	ln -s Makefile.gtk Makefile

and then just issue: 

	make

If everything goes out smoothly, you can also try:

	make python

to create python bindings.

