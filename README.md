Introduction

BigAdd is a small programming language designed for adding and subtracting big integers programmatically. It is an interpreted language, and the BigAdd interpreter can be invoked from the command line using the command BA. The interpreter takes the script file's name with the extension .ba as the only argument. This readme file provides an overview of the BigAdd language, its features, syntax, and usage instructions.

Table of Contents

1. Language Overview
2. Lines of Code
3. Comments
4. Data Types
5. Variables
6. Statements
   - Assignment Statement
   - Addition Statement
   - Subtraction Statement
   - Output Statement
   - Loop Statement
   - Code Block
7. Examples

Language Overview

BigAdd is a simple language designed to handle large integers, which are represented as decimal numbers with up to 100 digits. It supports variable declarations, assignment statements, addition and subtraction operations, output statements, and loop constructs. Strings are treated as constants and used only in output statements.

Lines of Code

A line of code in BigAdd can contain keywords, brackets, variables, and constants. It can be divided across multiple text lines. The following are the different types of lines in BigAdd:
1. Variable declaration.
2. Assignment statement.
3. Addition statement.
4. Subtraction statement.
5. Output statement.
6. Loop statement.

Comments

Comments in BigAdd are written between curly braces {}. For example:

{ This is a comment }

Data Types

The only data type supported in BigAdd is the integer. Integers are signed whole numbers represented as decimal values. Valid integer representations include:
123113

Invalid integer representations:
- 3.14159 (real number)
- 3.0 (decimal point not allowed)
- 3. (decimal point not allowed)
- - 5 (no space between minus sign and first digit)
- --5 (only one minus sign allowed)
- +5 (plus sign not allowed)

Variables

All variables in BigAdd must be declared as integers using the following syntax:
int <variable_name>.

Variable names are case-sensitive and have a maximum length of 20 characters. They must start with an alphabetical character and can continue with alphanumeric characters or an underscore. Variables are global and static and are initialized to 0 at the time of creation.

Example:
int myVar.

Statements

1. Assignment Statement

The assignment statement is used to assign a value to a variable:
move <int_value> to <variable>.

Example:
move 25 to myVar. { assigns 25 to myVar }

2. Addition Statement

The addition statement increments the variable by the specified integer value:
add <int_value> to <variable>.

Example:
add 2 to sum. { The value of sum increases by 2 }

3. Subtraction Statement

The subtraction statement decrements the variable by the specified integer value:
sub <int_value> from <variable>.

Example:
sub t from total. { The value of total decreases by t }

4. Output Statement

The output statement is used to display strings and integer values on the screen:
out <out_list>.

The <out_list> is a list of strings and integer values separated by commas. The elements in the list can be variables, constants, or newline characters.

Example:
out "The result is:", sum.

5. Loop Statement

The loop statement is used for repetitive operations. It iterates a specified number of times and can contain a single line of code or a code block.
loop <int_value> times <line>

OR

loop <int_value> times <code_block>

Example:
loop 10 times out "*". { writes 10 stars on the screen }

Code Block

A code block is a list of code lines enclosed in square brackets []. It can be used interchangeably with a single line of code. Loops can be nested within each other using code blocks.

Example:
int size. int sum.

move 5 to size.

loop size times
[ 
    out size, newline.
    add size to sum.
]

out newline, "Sum:", sum.


