## Use

The examples folder contains two C examples. The first, ucl\_iterate.c, iterates through a given configuration file, converts each entry to an appropriate type, and prints it out. The second, ucl\_lookup.c, assumes a given set of keys, and looks up those keys in a configuration file.

To use, compile and then run with a filename:

    $ make
    $ ./ucl_lookup <filename>

For example, to run the file on a sample test:

    $ ./ucl_lookup sample_config.txt

## Limitations

Note that the iteration example does not print nested arrays. It would be trivial to do, but since nested arrays are just values without a key, adding the logic is more trouble than it's worth for a simple example. However, the example does contain a template to recursively process objects and arrays.

In other words, an array is:

```nginx
sample = [1, 2, 3]
```

Where the key is "sample" and the value is the array. But if we nest an array:

```nginx
sample = [1, [2, 3, 4], 5]
```

Then the array [2, 3, 4] does not have a key.
