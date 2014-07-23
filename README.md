SOL Compiler & Machine
======================

- How to Compile
	
  There is only one makefile for both programs:

	- to compile only the compiler: 

            make compiler

	- to compile only the machine: 
	
            make machine

	- to compile both:
	  
            make

- Install/Uninstall

  - Type make install to copy both the binaries smachine & scompiler in /usr/local/bin/

  - Type make uninstall to remove the binaries smachine & scompiler in /usr/local/bin/

- Usage

	- to compile with scompiler: 

            scompiler [-o outputfile] sourcefile
    
    default outputfile is "s.out"

	- to run a compiled file with smachine: 

            smachine inputfile
