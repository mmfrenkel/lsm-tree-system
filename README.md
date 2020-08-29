# Mini LSM Tree Database System

This C project represents a simplified version of a log structed merge (LSM) tree database system. 

This system has two components: 
(1) An in-memory component represented by the `memtable.c` library. To keep things simple for this low-volume system, the `memtable` is implemented as a binary search tree. A two-three tree implementation is also provided in this repositority, though not currently supported. 
(2) A file system component, made up of several log files called `segments` which contain key, value pairs sorted by key. The contents of the `memtable` are periodically flushed to disk and once written, these `segments` are immutable, or read-only.

## Functionality 

To accept database input, this project currently only supports user interaction from the command line. Users may elect to add, delete, search or print values into/from the database, until they elect to quit the program.

Here is how the functionality is implemented behind the scenes: 

* `Insert`: All new records are inserted into the `memtable` component first. If the insertion results in the `memtable` exceeding a certain size, the `memtable`'s contents are added to a new file on disk called a segment. Because the key, value pairs are written to the segment via an in-order traversal, the keys in the file are sorted.

* `Search`: When searching for the value for a specific key, the system first searches through the `memtable` for that key. If that key isn't found in the `memtable`, the system will search the `segments` in reverse chronological order. This ensures that the system will return the most recent value for a given key, if it exists in the system already.

* `Delete`: If a record is selected for deletion, the program will first search for the key in the `memtable`. If found, it will mark the record with a `tombstone`, a unique value that the system recogizes as a deleted key. This is important, because the `segment` files are read-only; this means that even if a key was deleted from the `memtable` a previous entry for that key could persist in an older `segment`. Hence, the tombstone is important for alerting the system that the key should be removed from the system during the compaction step (see below). 

* `Print`: Currently, only in-order printing of the `memtable` is supported. 

* `Compaction`: As mentioned above, when a `memtable` exceeds a certain size, it is sent to disk as a `segment` file. Without further intervention, this has the potential to result in many segment files. The consequence is that search/read functionality becomes extremely slow, as all `segments` would have to be opened and searched in order to conclude that a key doesn't exist in the system. Here we address this concern by periodically "merging" multiple `segments`together using an algorithm analogous to merge-sort. In this process, duplicated key entries and deleted records are removed, with the effect of keeping the number of `segments` low. In this system, once two segment files are successfully merged, the old files are safely deleted.

## Use

This project uses a Makefile to facilitate compiling and running this program. Run the database system by issuing the following command, which will clean-up any existing files, compile the code, build the executable, and launch the program:

```
$ make all
```
## Future Development

This database system was built as an exercise of understanding how LSM Trees work. Currently, this database is a single-threaded system, where the compaction step occurs synchronously with user input. This does not degrade performance of this demo project, as the volume of writes and reads is low (a single user). However, future iterations will explore multithreading in order to allow compaction to run as a background process.


 