// utilities.ixx
module;

#include <stdio.h>

// 1. Declare the module name
export module utilities;

// 2. Wrap the function declaration/definition within the module scope, using
// 'export' to make it public
export void doWork() { printf("You gay \n"); }