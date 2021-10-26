# Template for module repositories

Module repository must follow some basic guidelines so that it can be safely integrated with the rest of the tinyverse:

- all source code must be in a folder with the same name as the repository itself
- all source code must be in a namespace of the same name as the repository itself

## Declaring modules

A repository may provide implementations for at most one module per category, i.e. frontend, optimizer, backend and target. These must be declared in special files at the root directory of the module with filenames in the format `REPONAME_frontend`, `REPONAME_optimizer`, etc. 


