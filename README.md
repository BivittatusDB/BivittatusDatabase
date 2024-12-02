# BIVITTATUS DATABASE
Soon, [BivittatusDB](https://github.com/HarbingerOfFire/bivittatusDB) will be moving from a HDF5 file system to this custom system. Right now we are in the beginning stages of this change, and are working on making the readable files. 

## USAGE
This code is available in order to keep our DBMS as open source, and allow others to contibute. To make your edits, all you have to do is clone the repository. I have provided a "test.c" file to make sure the system will be compatable after your edits.

After making your edits, please compile the code with the commands below:
```bash
make && make clean
```
Note, windows machines may encounter some issue compiling due to the use bash commands in the renaming and cleaning segements. To fix this run the command below:
```bash
make win && make winclean
```

## PYTHON IMPLEMENTATION
This code, when compiled as shown above, is meant to be a C-library for Python. The actual implementation will be kept in a class within the main repo as `BDB_io.py`. 

The main reason for this edit is to minimize the requirements to only the bare encryption algorithms, which will be implemented using proper python libraries. 
