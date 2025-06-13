# Progres

[![CI build](https://github.com/tmatos/progres/actions/workflows/build.yml/badge.svg)](https://github.com/tmatos/progres/actions/workflows/build.yml)
[![CI tests](https://github.com/tmatos/progres/actions/workflows/tests.yml/badge.svg)](https://github.com/tmatos/progres/actions/workflows/tests.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/8b26336f5528494cafa64eaa7ac673d6)](https://app.codacy.com/gh/tmatos/progres/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![codecov](https://codecov.io/gh/tmatos/progres/graph/badge.svg?token=XCUNGD1HQD)](https://codecov.io/gh/tmatos/progres)

Event-driven simulator for a subset of the Verilog language.

Currently, provides simulation of combinational circuits written
in structural Verilog.


## 1. Usage

In the command line interface:

```
progres [-s] source_file.v [inputs.in] [outputs.out]
```

Arguments inside square brackets are optional.

The above command analyzes the provided Verilog source code file.
When the first syntactical error is found, the program will be interrupted and
a message will be displayed, indicating the line and column of the occurrence,
with a possible description of the problem.

Example output with an error:

`5:13 error: Expected symbol was not found...`

This message above tells us that on line 5 and column 13 of the provided 
`.v` file, an expected symbol was not found. The rest of the text may contain 
something more specific to each possible existent error.

If the source file is correct and a valid input file has been provided, 
the program will attempt to simulate the circuit with this input and 
generate the corresponding output.

If the simulation is successful, an output file will be created as specified 
in the program arguments or, if these were omitted, a file will be created 
with the same name as the input file and with the `.out` extension appended.

`-s` Option omits all of the command line textual output messages.


## 2. Input and output files

In order to do simulations, the `.in` input file should be in a very specific 
format, defined below, and there must be an exact correspondence between the 
input ports of the top Verilog module and the signals inside this `.in` file.

The `.in` input files and the `.out` output files are all plain text files,
which follows an extremely simple format:

* Single line comments are valid, anything following a `//` will be ignored.

* Each signal begins with an identifier that names it. Next, between curly 
brackets, there will be a sequence of "pulses". Each of them having one of 
the four possible values: `0`, `1`, `z` or `x`, along with their specific 
duration times. Each "pulse" is separated from the other by a comma.

Example:

``` 
// Input file with two signals

first
{
 X(2), 0(5), 1(3)
}

second { 0(2),1(2),x(3),0(3) }

// End of input file
```

In this file, we have the following signal representations:

```
                  ▁▁▁
    first: ╳╳▁▁▁▁▁▏  
time unit: 0123456789
             ▁▁      
   second: ▁▁▏ ╳╳╳▁▁▁
time unit: 0123456789
```


## 3. Building and installing

The source code is written primarily in C and should compile under any 
build environment supporting the C99 standard.
A makefile is provided, so simply use:

```
make
```

To generate the executable, which can be copied to any desired location.


## 4. Contacts

* Email: tiago AT tmatos DOT net
* Site: https://tmatos.net
* Repo: https://github.com/tmatos/progres


(C) 2014, 2025 Tiago Matos
