# Chi-square simulation

C++ project for simulation and chi-square goodness-of-fit testing for the hypergeometric distribution.

The solution contains two projects:

- `CHI2_Program` — MFC application with dialogs and graphical output.
- `CHI2_Console` — console demonstration of the mathematical classes.

## Features

- Hypergeometric distribution with cached theoretical probabilities.
- Two simulation methods:
  - Bernoulli method;
  - inverse method.
- Chi-square test.
- Histogram of empirical and theoretical frequencies.
- Generation and visualization of p-values.
- Estimation of type I error and test power.
- Power depending on sample size.

## Documentation

Generated Doxygen HTML documentation is available here:

```text
docs/html/index.html
```

The documentation is generated for the console project and describes the mathematical classes used by both projects.

## Structure

```text
CHI2_Program/
    MFC application and shared mathematical classes

CHI2_Console/
    Console example with main function in "How to" style

docs/html/
    Generated Doxygen documentation
```

## Build

Open `CHI2_Program.slnx` in Visual Studio.

Build and run either:

- `CHI2_Program` for the graphical MFC application;
- `CHI2_Console` for the console demonstration.

Both projects use the same physical files for the mathematical classes.