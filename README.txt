log-parser-mysql-client
=======================

parses logs - sends them to mysql database.
compiles with the compile.sh script located in parser/src.
unsure about dependencies.  Needs some version of mysql c api.



To set up:
	1)  Compile:
	1.1) Edit the PATH define at (or near) line 3 of parser.hpp to
			be your location of the configuration file.
	1.2) Edit your configuration file to include the locations of
			the log files.  Be sure and have the correct read/write
			permissions for the log to be able to touch them.
	1.3) Run the compile.sh script to compile.  If that doesn't work,
			you have a different version of mysql that has different settings.
			You'll need to edit the compile.sh script to include the locations
			of the libraries that you're linking to.
	2)  Running:
	2.1) just run the binary as normal.  Little to no user interaction is
			required.

