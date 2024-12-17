# Branch-Predictor-Sim

This project measures the effectiveness of several branch direction predictors on a number of traces of conditional branch instructions.

The predictors include always taken, always not taken, bimodal with 1-bit counters, bimodal with 2-bit counters, bimodal with 3-bit counters, gshare, and tournament predictors.

Each trace contains many branch instructions, and each line in the trace contains the following information for each branch: the program counter (expressed as a word address), and the actual outcome of the branch.

The command line should follow the format ./predictors input_trace.txt output.txt

The output file has the following format:

#,@; <- always taken

#,@; <- always not taken

#,@; #,@; #,@; #,@; #,@; #,@; #,@; <- bimodal 1-bit counter for table sizes 4, 8, 32, 64, 256, 1024, and 4096 respectively, and non-taken for initial state of counters

#,@; #,@; #,@; #,@; #,@; #,@; #,@; <- bimodal 2-bit counter for table sizes 4, 8, 32, 64, 256, 1024, and 4096 respectively, and strongly non-taken for initial state of counters

#,@; #,@; #,@; #,@; #,@; #,@; #,@; <- bimodal 3-bit counter for table sizes 4, 8, 32, 64, 256, 1024, and 4096 respectively, and most strongly taken for initial state of counters

#,@; #,@; #,@; #,@; #,@; #,@; #,@; #,@; #,@; #,@; #,@; gshare with table size of 4096, with varied history bit length from 2 to 12 in 1-bit increments and strongly non-taken for initial state of counters

#,@; <- tournament predictor between gshare and 2-bit bimodal predictor with 12 bits global history

Where each

\# corresponds to the number of correct predictions made by each of the predictors

@ corresponds to the number of branches.
