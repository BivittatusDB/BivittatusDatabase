# BIVITTATUS DATABASE
Soon, [BivittatusDB](https://github.com/HarbingerOfFire/bivittatusDB) will be moving from a HDF5 file system to this custom system. Right now we are in the beginning stages of this change, and are working on making the readable files. 

## USAGE
This code is available in order to keep our DBMS as open source, and allow others to contibute. To make your edits, all you have to do is clone the repository. I have provided a "test.c" file to make sure the system works.

After making your edits, please compile the code with the commands below:
```bash
gcc -c -Wall -Werror -fpic database.c
gcc -shared -o lib_bdb.so add.o
```
Make sure you rename the `database.h` file in test to `database.c` before compiling. You can then open a pull request and if your edits are accepted, we will update the main repo accordingly.

## PYTHON IMPLEMENTATION
This code, when compiled as shown above, is meant to be a C-library for Python. The actuall implementation will be kept in a class within the main repo as `BDB_io.py`. 

The main reason for this edit is to minimize the requirements to only the bare encryption algorithms, which will be implemented using proper python libraries. 

## KNOWN ISSUES
Right now the `CheckDataSet` function in `base.h` is always returning `False` even when the dataset is present. Effectively, it is just there for show, but it will need to be edited to work in order to ensure proper error handling in the python implementation.
