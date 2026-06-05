# Chi-square simulation

C++ project for simulation and chi-square test for the hypergeometric distribution.

The solution contains two projects:

- `CHI2_Program` — MFC application with dialogs and graphical output.
- `CHI2_Console` — console demonstration of the mathematical classes.

## Features

- Hypergeometric distribution with cached theoretical probabilities.
- Two simulation methods:
  - Bernoulli method;
  - Inverse method.
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

Both projects use the same physical files for the mathematical classes, stored in CHI2_Program folder.

## MFC application usage

The MFC application has several menu sections:

- `Histogram` — opens a dialog for setting H0, H1, simulation method and sample size. Shortcut: `Alt + H`.
- `P-values` — opens a dialog for setting H0, H1, simulation method, sample size and number of p-values. Shortcut: `Alt + P`.
- `Power by n` — opens a dialog for setting parameters and building power depending on sample size.
- `Power by alpha` — opens a dialog for setting parameters and building power depending on alpha.

The `Update` command regenerates data with the current parameters and redraws the current view. It is available from the menu and from the square toolbar button. Shortcut: `Alt + U`.
