# printf in C

## How to run the checker

```bash
make
```

## About

https://ocw.cs.pub.ro/courses/iocla/teme/tema-1

### Extern sources

- swap(), reverse(), itoa() from https://www.techiedelight.com/implement-itoa-function-in-c/

#### itoa() modifications to suit the project

- arguments taken by itoa are of long type
- special case for hexa base, also using tolower() after converting

## Implementation

I chose to alloc 3 dynamic arrays in order to manipulate the strings:
- aux: takes the returned string from itoa()
- string: takes the string from the va_list, for the '%s' case
- out: final string with all the modifications just to be printed

I also used a long variable called getArg in order to take the arguments
from the va_list, because some arguments were bigger than the maximum size
of unsigned int.

## Flow

For iterating through the string received as a a parameter in iocla_printf()
I have two while loops:
- first while loop stops when encountering the null terminator - '\0' so we
know that the string is over and there is nothing left to iterate
- the second while loop stops when encountering the '%' character. Here i 
check for the next characters and if it is also a '%' character we jump with
the goto instruction to the copy to final string - block of code. If it isn't
then we need to check all cases for specifiers - %d, %u, %x, %c, %s. For the
%u case i added a formula to convert a negative value:
UINT_MAX + value + 1 in order to have the unsigned behaviour.

After checking the specifiers cases we just copy the characters in the string
that will be printed in the final, and then return its length.

## Project structure

```bash
├── a.out
├── checker.py
├── input
│   ├── test0
│   ├── test1
│   ├── test10
│   ├── test11
│   ├── test12
│   ├── test13
│   ├── test14
│   ├── test15
│   ├── test2
│   ├── test3
│   ├── test4
│   ├── test5
│   ├── test6
│   ├── test7
│   ├── test8
│   └── test9
├── libprint.so
├── Makefile
├── output
│   ├── test0.out
│   ├── test0.ref
│   ├── test10.out
│   ├── test10.ref
│   ├── test11.out
│   ├── test11.ref
│   ├── test12.out
│   ├── test12.ref
│   ├── test13.out
│   ├── test13.ref
│   ├── test14.out
│   ├── test14.ref
│   ├── test15.out
│   ├── test15.ref
│   ├── test1.out
│   ├── test1.ref
│   ├── test2.out
│   ├── test2.ref
│   ├── test3.out
│   ├── test3.ref
│   ├── test4.out
│   ├── test4.ref
│   ├── test5.out
│   ├── test5.ref
│   ├── test6.out
│   ├── test6.ref
│   ├── test7.out
│   ├── test7.ref
│   ├── test8.out
│   ├── test8.ref
│   ├── test9.out
│   └── test9.ref
├── README.md
└── tema1.c
```