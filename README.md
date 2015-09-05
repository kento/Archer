<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#sec-1">1. License</a></li>
<li><a href="#sec-2">2. Introduction</a></li>
<li><a href="#sec-3">3. Prerequisites</a></li>
<li><a href="#sec-4">4. Installation</a>
<ul>
<li><a href="#sec-4-1">4.1. Building</a></li>
</ul>
</li>
<li><a href="#sec-5">5. Usage</a>
<ul>
<li><a href="#sec-5-1">5.1. How to compile</a>
<ul>
<li><a href="#sec-5-1-1">5.1.1. Single source</a></li>
<li><a href="#sec-5-1-2">5.1.2. Makefile</a></li>
<li><a href="#sec-5-1-3">5.1.3. Hybrid MPI-OpenMP programs</a></li>
</ul>
</li>
<li><a href="#sec-5-2">5.2. Options</a></li>
</ul>
</li>
<li><a href="#sec-6">6. Logos</a></li>
</ul>
</div>
</div>

pooo#+TITLE:     Archer

# License<a id="sec-1" name="sec-1"></a>

Please see LICENSE for usage terms.

# Introduction<a id="sec-2" name="sec-2"></a>

<img src="resources/images/archer_logo.png" hspace="5" vspace="5" height="45%" width="45%" alt="Archer Logo" title="Archer" align="right" />

**Archer** is a data race detector for OpenMP programs.


Archer combines static and dynamic techniques to identify data races
in large OpenMP applications, leading to low runtime and memory
overheads, while still offering high accuracy and precision. It builds
on open-source tools infrastructure such as LLVM and ThreadSanitizer
to provide portability.

# Prerequisites<a id="sec-3" name="sec-3"></a>

To compile Archer you need an host GCC version >= 4.7.

# Installation<a id="sec-4" name="sec-4"></a>

Archer has been developed under LLVM 3.5 with OpenMP support (for
more information go to <http://clang-omp.github.io>).

## Building<a id="sec-4-1" name="sec-4-1"></a>

First obtain Archer, e.g. from Github (<https://github.com/soarlab/Archer>):

    git clone git@github.com:soarlab/Archer.git Archer

Then, build Archer by running `install.sh`:

    ./install.sh --prefix=llvm_install_path [default: --prefix=/usr]

The installation script will create a folder called **LLVM** at the same
level of the Archer directory and install LLVM to *llvm_install_path*.

In case your GCC is not installed in a standard path you need to
specify the GCC toolchain path for LLVM/Clang using the flag
*&#x2013;gcc-toolchain-path*:

    ./install.sh --prefix=llvm_install_path --gcc-toolchain-path=gcc_toolchain_path

Once the installation completes, you need to setup your environement
to allow Archer to work correctly.

Please set the following path variables:

    export PATH=${LLVM_INSTALL}/bin:${LLVM_INSTALL}/local/archer/bin:\${PATH}"
    export LD_LIBRARY_PATH=${LLVM_INSTALL}/bin:${LLVM_INSTALL}/lib/intelomprt:${LLVM_INSTALL}/local/archer/lib:\${LD_LIBRARY_PATH}"

To make the environment permanent add the previous lines or
equivalents to your shell start-up script such as "~/.bashrc".

WARNING: 

During the installation process you may see an error like the
following:

    CMake Error: install(EXPORT "LLVMExports" ...) includes target "opt" which requires target "Polly" that is not in the export set.
    CMake Error: install(EXPORT "LLVMExports" ...) includes target "bugpoint" which requires target "Polly" that is not in the export set.

It is an error related to the CMake configuration process of Polly. It
does not affect your installation.  Future versions of Archer will
upgrade Polly to the latest version avoiding this problem.

# Usage<a id="sec-5" name="sec-5"></a>

## How to compile<a id="sec-5-1" name="sec-5-1"></a>

Archer provides a command to compile your programs with Clang/LLVM
OpenMP and hide all the mechanics necessary to detect data races
automatically in your OpenMP programs.

This Archer command is called *clang-archer*, and this can be used as
a drop-in replacement of your compiler command (e.g., clang, gcc,
etc.).

The following are some of the examples of how one can integrate
*clang-archer* into his/her build system.

### Single source<a id="sec-5-1-1" name="sec-5-1-1"></a>

    clang-archer example.c -L/path/to/openmp/runtime -lOMPRT -o example

### Makefile<a id="sec-5-1-2" name="sec-5-1-2"></a>

In your Makefile, set the following variables:

    CC = clang-archer
    
    LDFLAGS = -L/path/to/openmp/runtime -lOMPRT

### Hybrid MPI-OpenMP programs<a id="sec-5-1-3" name="sec-5-1-3"></a>

In your Makefile, set the following variables:

    CC = mpicc -cc=clang-archer
    
    ...
    
    LDFLAGS = -L/path/to/openmp/runtime -lOMPRT

## Options<a id="sec-5-2" name="sec-5-2"></a>

Running the following command:

    clang-archer --help

shows the options available with *clang-archer*.

    usage: clang-archer [-h] [-v] [-d] [--log] [-db] [-CC [CC]] [-USE_MPI]
                        [-MPICC [MPICC]] [-OPT [OPT]] [-LINK [LINK]] [-DIS [DIS]]
                        [-LIB [LIB]] [-PLUGIN_LIB [PLUGIN_LIB]]
                        [-OPENMP_INCLUDE [OPENMP_INCLUDE]] [-g]
                        [-O0 | -O1 | -O2 | -O3 | -Os | -Oz] [-fopenmp] [-liomp5]
                        [-c] [-o [O]]
    
    Compile your program with Archer support, a data race detector for OpenMP programs.
    
    optional arguments:
      -h, --help            show this help message and exit
      -v, --version         show program's version number and exit
      -d, --debug           Print the compiling commands
      --log                 Keep intermediate logs
      -db, --disable-blacklisting
                            Disable static analysis and apply ThreadSanitizer
                            instrumentation to the entire program
      -CC [CC]              Change the program used to compile and link the
                            programs
      -USE_MPI              Link against MPI libraries
      -MPICC [MPICC]        Change the program used to compile and link the MPI
                            programs
      -OPT [OPT]            Change the program used to optmize the programs
      -LINK [LINK]          Change the program used to link the byte code files
      -DIS [DIS]            Change the program used to disassemble the byte code
                            files
      -LIB [LIB]            Set the path where to find Archer libraries
      -PLUGIN_LIB [PLUGIN_LIB]
                            Set the path where to find Archer Plugin libraries
      -OPENMP_INCLUDE [OPENMP_INCLUDE]
                            Set the path where to find OpenMP headers
      -g                    If the debugging flag is not present in the
                            compilation command it will be added by default
      -O0                   The optimization flags will be forced to '-O0'
                            optimization level for analysis purposes
      -O1
      -O2
      -O3
      -Os
      -Oz
      -fopenmp              OpenMP flag
      -liomp5               OpenMP library
      -c                    Only run preprocess, compile, and assemble steps
      -o [O]                Output filename

# Logos<a id="sec-6" name="sec-6"></a>

<img src="resources/images/uofu_logo.png" hspace="5" vspace="5" height="35%" width="35%" alt="UofU Logo" title="University of Utah" align="left" />

<img src="resources/images/llnl_logo.png" hspace="5" vspace="5" height="50%" width="50%" alt="LLNL Logo" title="Lawrence Livermore National Laboratory" align="right" />