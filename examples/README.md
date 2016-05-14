The examples folder now contains a C example that iterates through a given configuration file, converts each entry to an appropriate type, and prints it out.

To use, compile and then run with a filename:

    $ make
    $ ./ucl_c <filename>

For example, to run the file on a sample test in the respository:

    $ ./ucl_c ../tests/basic/2.res

Note that the printing example does not print nested arrays. It would be trivial to do, but since nested arrays are just values without a key, adding the logic is more trouble than it's worth for a simple example. However, the example does contain a template to recursively process objects and arrays.

In other words, an array is:

sample = [1, 2, 3]

Where the key is "sample" and the value is the array. But if we nest an array:

sample = [1, [2, 3, 4], 5]

Then the array [2, 3, 4] does not have a key.
