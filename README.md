# Project structure
- Core
- Tests
- Main

## Core
The core library is generally contains the implementation of a files' comparison algorithm based on [Myers's algorith](http://www.xmailserver.org/diff2.pdf). Also there is a [_cleaup_](https://neil.fraser.name/writing/diff/) functionality for improving output quality.
The implementation was mainly inspired by [Neil Fraser](https://github.com/NeilFraser).

## Tests
A hand-made unit testing just using overloaded assetions. Different cases have been tested, mostly short and simple to be sure that the code works fine without "basic" exceptions. Enourmously huge texts haven't been tested.

## Main
Console-based program for outputting a differences between two inputs.

# Building
For each piece of the structure there is a _Makefile_. The root makefile describes the rules for building each *part* seprately:
- _build-static-lib_: builds the *statically linking* output file. The output *libmyersdiff.a* file will be created in a _lib_ folder of the project's root directory.
- _run-tests_: based on a *libmyersdiff.a* file builds the executable for running all existing unit tests. By running this command without existing static library the output will throw an exception.

Also there is _all_ rule that combines creation of a static library, building tests and run it.