Data Structures and Algorithms | 2020
Văn Chí Nam - Bùi Huy Thông

# CHALLENGE 1: ARITHMETIC EXPRESSION CALCULATION
La Ngọc Hồng Phúc  
Hồ Lâm Bảo Khuyên  
Nguyễn Tất Trường  
Trần Quốc Tuấn  
## 1. Preliminaries

### 1.1 Research

While we use infix expressions in our daily lives, computers have trouble understanding this format because they need to keep in mind rules of operator precedence and also brackets. Prefix and Postfix expressions are easier for a computer to understand and evaluate. Below are some examples of Infix, Prefix, and Postfix expressions.

|**Infix Expression**|**Prefix Expression**|**Postfix Expression**|
| :- | :- | :- |
|A + B|+ A B|A B +|
|A + B \* C|+ A \* B C|A B C \* +|
|(A + B) \* C|\* + A B C|A B + C \*|
|A + B \* C + D|+ + A \* B C D|A B C \* + D +|
|(A + B) \* (C + D)|\* + A B + C D|A B + C D + \*|

_Table 1: Examples of Infix, Prefix, and Postfix expressions_

Explain the algorithms, step-by-step example, for the following algorithms:
- Converting an infix expression to a prefix expression.
- Converting an infix expression to a postfix expression.
- Converting a prefix expression to a postfix expression.

### 1.2 Programming

- You are requested to build a program for validating, converting, and calculating the arithmetic infix expressions. Please choose one of the following options for further requirements.
    - **Integer option**: Input expressions contain only integers. *(8 points max)*
    - **Floating-point option**: Input expressions contain both integers and floating- point numbers. *(10 points max)*

- Input: the file *"input.txt"* consists of infix expressions, each located on a single line. Example:	
    - (1 + 1) ˆ 3
    - 0. 3 \* 2 + 1.5
    - 0. 4 4 3 + - *(error expression)*
    - 0. (3.72 + 5.16) / 2.22

    There might be some error expressions in the input file. However, the expressions must follow the below format:
    - Operands and operators from these expressions must be separated by one single space " ".
    - Legal operators: + (addition) , − (subtraction) , ∗ (multiplication), */* (di- vision) ,ˆ(power).
    - Legal brackets: ( ), [ ], { }
    - For the **Floating-point option**, real numbers have at most 2 decimal digits.
- Your program must be built into an execution *".exe"* file that runs within the following command line arguments: `A.exe InputPath N Action OutputPath`, in which:
    - A.exe: Your execution *".exe"* file.
    - InputPath: Path to the *"input.txt"* file.
    - N: an integer, represent the number of expressions from the *"input.txt"* file.
    - Action:	
        - `-c`: calculate the identified the expressions.
        - `-t`: convert the identified the expressions into postfix expressions.
    - OutputPath: Path to the *"output.txt"* file, which will be mentioned below.

- Output: the file *"output.txt"*. Content of this file depends on the action chosen from the command line argument.
    1. `-c`: 
        - Calculated value of the expressions from input file. Real numbers result in 2 decimal digits. Error expressions result in **E**.
        - Each value locates on a single line, corresponding to its expression line from the input file.  
     2. `-t`:
        - Converted postfix expressions of the expressions from input file. Error expressions result in **E**.
        - Converted postfix expressions format must follow the input expression format above.
        - Each result postfix expression locates on one single line, corresponding to its infix expression line from the input file.

Examples 1:

|**Input**|**Output**|
| :-: | :-: |
|(1 + 1) ˆ 3|8|
|3 \* 2 + 1.5|7.5|
|4 4 3 + -|E|
|(3.72 + 5.16) / 2.22|4|

Examples 2:

|**Input**|**Output**|
| :-: | :-: |
|(1 + 1) ˆ 3|1 1 + 3 ˆ|
|3 \* 2 + 1.5|3 2 \* 1.5 +|
|4 4 3 + -|E|
|(3.72 + 5.16) / 2.22|3.72 1.56 + 2.22 /|
