# Project structure
- Core
- Tests
- Main

## Core
The core library is generally contains the implementation of a files' comparison algorithm based on [Myers's algorith](http://www.xmailserver.org/diff2.pdf). Also there is a [_cleaup_](https://neil.fraser.name/writing/diff/) functionality for improving output quality.
The implementation was mainly inspired by [Neil Fraser](https://github.com/NeilFraser).

## Tests
A hand-made unit testing just using overloaded assetions. Different cases have been tested, more short and simple to be sure that the code works fine without "basic" exceptions. Enourmously huge texts haven't been tested.

## Main
Console-based program for outputting a differences between two inputs.