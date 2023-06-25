#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <errno.h>

/*
    Thread safe atoi implementation.
*/
char* remove_until_delim(char* str, char delim);
/*
    Removes the first n characters of the string.
*/
void remove_first_n_chars(char* str,int n);
/*
    If the given command line arguments are less than the expected amount,
    prints an error message and exits.
*/
void arg_check(int necessary_argc,int argc);
/*
    Prints the message and corresponding errno message of the given errnum and exits.
*/
void exit_on_error(int errnum,char* msg);
/*
If a function returns a negative value, calls exit_on_error with the given message.
*/
void fail_check(int return_val,char* msg);
/*
    If a function returns a non-zero value, calls exit_on_error with the given message.
*/
void fail_check_pthread(int return_val, char* msg);
/*
    Sets the signal handler for the given signal to the given function.
*/
void set_sig_handler(void func ( int));
/*
    Returns a timestamp in the format of YYYY-MM-DD HH:MM:SS.
*/
void get_timestamp( char *buffer);
/*
    Returns the PID of the current process. (uses proc/self)
*/
pid_t get_pid();
#endif //UTILS_H