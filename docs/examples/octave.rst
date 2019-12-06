Octave
######

For example, when using Octave + VUnit + GtkWave, data is allocated in Octave, and VUnit/C receives a struct with the pointer and some fields describing the size (be it a vector, a matrix, a cube). Assuming that the data is saved by Octave as 32bit IEEE integers, C and/or VHDL can use the pointer/data without needing to explicitly allocate another buffer. This is opposite to current VUnit's built-in and internal integer vector types, where allocation is hidden. Should we reproduce the API in C, it would not be possible to implement the Octave example I just explained.