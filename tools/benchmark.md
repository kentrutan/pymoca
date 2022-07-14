
# Run 1 of benchmark: Flatten All MSL 4 Examples
```
rutanwk@macbookpro ~ % cd modelica/pymoca/tools
rutanwk@macbookpro tools % conda activate pymoca-antlr49
(pymoca-antlr49) rutanwk@macbookpro tools % python benchmark.py
[...]
py_elapsed:  369.803 seconds
cpp_elapsed: 39.373 seconds
Speedup: 9.39
(pymoca-antlr49) rutanwk@macbookpro tools % python --version
Python 3.9.12
(pymoca-antlr49) rutanwk@macbookpro tools %
```

# Run 2
```
py_elapsed:  361.976 seconds
cpp_elapsed: 38.580 seconds
Speedup: 9.38
(pymoca-antlr49) rutanwk@macbookpro tools %
```

# Run 1 of benchmark: Parse all MSL4 files
```
py_elapsed:  315.527 seconds
cpp_elapsed: 70.181 seconds
Speedup: 4.50
```
Python 3.9 memory was 14.3 GB at end of benchmark. Need to do several runs of compiler on one model to see if memory use is getting in the way due to parsing whole MSL.

# Run 2
```
```

# Run 1 of benchmark: Flatten OpAmp, 10 times
```
py_elapsed:  4.034 seconds
cpp_elapsed: 0.822 seconds
Speedup: 4.91
```

# Run 2
```
py_elapsed:  2.898 seconds
cpp_elapsed: 0.670 seconds
Speedup: 4.33
```

# Run 3
```
py_elapsed:  2.483 seconds
cpp_elapsed: 0.641 seconds
Speedup: 3.87
```

# Run 4
```
py_elapsed:  2.588 seconds
cpp_elapsed: 0.640 seconds
Speedup: 4.04
```

# Run 1 of benchmark: Parse pymoca models, 10 times
```
py_elapsed:  0.960 seconds
cpp_elapsed: 0.288 seconds
Speedup: 3.33
```

# Run 2
```
py_elapsed:  1.033 seconds
cpp_elapsed: 0.290 seconds
Speedup: 3.56
```

# Run 3
```
py_elapsed:  1.100 seconds
cpp_elapsed: 0.308 seconds
Speedup: 3.57
```

