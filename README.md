# kaprekar
A C program that identifies and counts repeating numbers based on their digits. Users can specify the number of digits (between 4 to 9), and results are saved in an appropriate output file (like output4.txt).

# Number Repeater Detection Algorithm

This C program detects and counts numbers that reach a repeating cycle based on a specific algorithm. The algorithm works by:

1. Taking a number as input and checking its validity (allowing up to two repeated digits).
2. It rearranges the digits of the number in ascending and descending order.
3. The difference between the largest and smallest rearrangement is calculated.
4. This process is repeated until a repeating number is detected.

## Usage

- The constant `DIGITS` can be set to any value between **4** and **9**. 
- The program will generate an output file named `output{DIGITS}.txt`, where `{DIGITS}` corresponds to the value you set. For example, if `DIGITS` is set to `6`, the output file will be `output6.txt`.

Make sure to compile the program with a C compiler before running it. The results will be saved in the specified output file.
-----------------------------------------------------

<img width="645" alt="kaprekar-example" src="https://github.com/user-attachments/assets/c1986f7e-d651-4ca5-ac7d-1af52e518ee0">
</br>
<a href="https://behzadfarhadi.ir/2024/11/04/kaprekar-2/">Read more about Kaprekar Number, The Playful Mystery of Numbers in Mathematics</a>
