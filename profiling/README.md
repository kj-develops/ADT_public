# Profiling results

Cacm is a relativly small dataset in english.

Wiki is files from the Norwegian wikipedia from November 2006. The profiling is done with 2.000 and 20.000 files from this wiki.

The files in this folder contains the results of the gprof profiling of the following scenarios:

CACM dataset:

cacm_index_only : The program is run without making any search.
cacm_one_term: The program is run with the search query "index".
cacm_complex_query: The program is run with the search query "(index AND page) ANDNOT (right OR evolution)".

2.000 Wiki files:

wiki_2000_index_only : The program is run without making any search.
wiki_2000_one_term: The program is run with the search query "fotball".
wiki_2000_complex_query: The program is run with the search query “(fotball ANDNOT menn) AND (fotball ANDNOT herrer) AND 12 AND juli NOT Hamkam ANDNOT Alfheim stadion".

20.000 Wiki files:

wiki_20000_index_only : The program is run without making any search.
wiki_20000_one_term: The program is run with the search query "fotball".
wiki_20000_complex_query: The program is run with the search query “(fotball ANDNOT menn) AND (fotball ANDNOT herrer) AND 12 AND juli NOT Hamkam ANDNOT Alfheim stadion".