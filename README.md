# Search Engine Application

## Lessons learned

Programming language used: C

Knowledge of choices, design and implementation of algorithms and underlying datastructures. Basic knoledge of dynamic datastructures. Searching and sorting. Iteration and recursion.

A basic search engine that can find certain terms/words in a given set of documents and tell the user which documents meets their search requirements. The terms and the Boolean operators used to narrow the search are sent into the abstract data type (ADT), in this case an index that uses a hash map to store the results and connecting files containing the term searched for. The result is ordered to have the most relevant hits on top of the results list.


Parts of the code has been removed due to privacy reasons.

# Explanation of original task with prekode 
## Compiling
In the supplied `Makefile` there are two targets: `assert_index` and `indexer`.

The `Makefile` is designed to work with the current folder structure. 
Simply add any `.c` files you would like to include in the `src/` directory and add these to the `INDEXER_SRC` or `ASSERT_SRC` variables respectively.  
Add any `.h` files to the `ìnclude/` directory.

## assert_index
This is a test program to check that the index is implemented correctly.

## indexer
Can be tested with seperate data sets (Cacm and Wiki as explained in the assignment). It takes a single argument, which is a path to the files you want to index.

Once the indexer has indexed all files, it will start a web server that you can interact with in your browser at [127.0.0.1:8080](http://127.0.0.1:8080) or [localhost:8080](http://localhost:8080).
From here you can input your search queries.

## Remember to add data to be indexed!
To run the program add example data (cacm, wiki etc.) under the folder called 'data'.

## Changes and additions to precode:

Added:
  to src folder: 'index.c', 'parser.c'
  to include folder: 'parser.h'
  folder called 'profiling' containing all profiling info.

Changed:
'linkedlist.c' and 'list.h' to add function to return next element, without moving iterator.

Removed: 
Data sets under the folder 'data'. 

