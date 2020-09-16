# WebXO GTest Suite

This folder contains all of the tests for WebXO's Dynamic Library.

**DO NOT** run the makefile in this folder ``<WebXO>/Tests/Makefile``, instead run the ``Makefile`` in the root of the project as the following

```sh
# Make sure to be in the root of the project
cd /.../WebXO/

# This will build the tests
make build_tests

# This will run the tests
make run_tests
```