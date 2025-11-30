# Lexical-Analyzer
A Lexical Analyzer (also called lexer or scanner) is a program that reads a sequence of characters from the source code and breaks it into tokens. Tokens are the small parts of code like keywords, variable names, numbers, operators, or symbols.


# Requirements:

| Parameter        | Details                                |
| ---------------- | -------------------------------------- |
| Language         | Advanced C                             |
| Compiler         | GCC (GNU Compiler Collection)          |
| Operating System | Linux / Windows                        |
| Libraries Used   | `<stdio.h>`, `<string.h>`, `<ctype.h>` |



# Why This Project
This project is a C program that simulates a basic lexical analyzer.

It breaks source code into tokens like keywords, identifiers, operators, and constants.

Helps in understanding how compilers work.

Useful for code analysis, IDE syntax highlighting, and text processing.

Improves programming skills and prepares for advanced topics in compiler design.



# What We Learned from This Project:

How to break down complex problems (like source code) into smaller, manageable parts (tokens).

How to process text or data sequentially, which is useful in parsing, compilers, and scripting.

How to handle errors and exceptions systematically.

How to organize data using tables or arrays (like symbol tables).

How to think like a compiler, understanding how programming languages are interpreted.

# Where This Knowledge Is Helpful:

In compiler or interpreter development, understanding the first step of code processing.

In software tools like IDEs, code linters, and static analyzers.

In data processing tasks, where input must be analyzed and categorized.

In any programming project that requires reading, analyzing, and validating structured input.


<img width="718" height="497" alt="image" src="https://github.com/user-attachments/assets/9a86a7e1-99b2-4c6e-ad82-ec26ddcef33f" />


The Lexical Analyzer reads the source code and converts characters into tokens like keywords, identifiers, and operators.

The Parser takes these tokens and checks if the code follows correct syntax.

They work together: the Lexer provides tokens, and the Parser uses them to understand the program structure.

# Usage
# 1) How to compile
    
    gcc file_names.c
    ex: in my project
  # gcc token.c keywords.c main.c
# 2) How to run
   ./a.out input_filename
   ->atleast two arguments and must it is .c file
   # ./a.out input.c



