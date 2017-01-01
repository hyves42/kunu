

Initialization macros
=====================

C99 initializers are widely used to initialize statically allocated structures. Many macros are used to hide most of the initialization details away 
* Macro must named KN_something_INIT()
* If macro takes a *self* parameter, i.e. a reference to the structure being initialized, this parameter is first
* If macro takes a user_data parameter, this parameter is last.