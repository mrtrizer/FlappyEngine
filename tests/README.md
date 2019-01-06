# Unit tests
This folder contains a project that unites all tests of build-in modules.

Reasons:
- I can tweak configurations of tests in project generation phase. So I can make them generate coverage information.
- When you work in IDE, it's convinient to build all tests at once to see compilation errors
- Refactoring tools also becoming applicable to tests with using this uniting project
- When all executables are built, they can be run with a single script see `exec.sh`
