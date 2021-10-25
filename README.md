# TinyVerse

This is the main repository for tinyverse projects as well as a template for project- and course- specific repositories. 


## Organization

This repository implements the repl and compiler functionality that is shared by all languages and targets. The languages and targets then provide the language- and target- specific functionality, such as frontends, optimizers, backends, etc. The whole system is designed so that its pieces can be swapped at compile time to suit different languages, targets, optimizers, etc. 

