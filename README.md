# GridPACK

GridPACK is a software framework consisting of a set of modules
designed to simplify the development of programs that model the power
grid and run on parallel, high performance computing platforms. The
modules are available as a library and consist of components for
setting up and distributing power grid networks, support for modeling
the behavior of individual buses and branches in the network,
converting the network models to the corresponding algebraic
equations, and parallel routines for manipulating and solving large
algebraic systems. Additional modules support input and output as well
as basic profiling and error management.  

See the [GridPACK home page](https://www.gridpack.org) for more information.

## Submodule(s) ##

The GridPACK code use some third-party code that is included as a
submodule.  After cloning this repository, do the following in the
top-most directory of your clone to get the submodule code:

        git submodule update --init


## Building Global Arrays ##

[Global Arrays](https://hpc.pnl.gov//globalarrays/) is one of the
[libraries required](https://www.gridpack.org/wiki/index.php/How_to_Build_GridPACK#Prerequisite_Software)
to build and use GridPACK.  On some platforms this is available as a
system package, on others it is not.
[Global Arrays](https://hpc.pnl.gov//globalarrays/) can be built with
GridPACK if needed by adding 

        -D BUILD_GA:BOOL=ON

to the GridPACK [CMake](https://cmake.org/) configuration (more
details on GridPACK build is
[here](https://www.gridpack.org/wiki/index.php/How_to_Build_GridPACK)).
This will cause the configuration to build a GridPACK-specific
[Global Arrays](https://hpc.pnl.gov//globalarrays/) library along with
GridPACK.  

# GridPACK HADREC Application Python Wrapper

## Requirements

  * GridPACK >= 3.4
  * Python >= 2.7
  * pybind11 >= 2.4
  * Python `setuptools` package
  * Python `nose` package
  * CMake
  * C++ Compiler used to build GridPACK
  * MPI installation used to build GridPACK

### Pybind11

If pybind11 is not installed, or installed without CMake support, the
build will fail.  In that event, place the pybind11 code in the top
directory: 
```
git clone -b v2.4 https://github.com/pybind/pybind11.git
```
It is not needed after the module is built.
  
### GridPACK

GridPACK must be built and *installed* as *shared* libraries. This
requires that any GridPACK dependencies (e.g. PETSc, Global Arrays,
Boost) also be built as shared libraries. 

Set the `GRIDPACK_DIR` environment variable to indicate where GridPACK
was installed. 

## Build and Test

Build the GridPACK python wrapper modules with
```
GRIDPACK_DIR=/usr/local/gridpack
export GRIDPACK_DIR
python setup.py build
```
