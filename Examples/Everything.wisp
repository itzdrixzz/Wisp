// ===== Wisp Feature Draft v0.0.1 =====
// This file demonstrates syntax ideas for Wisp.
// How syntax works and will be compiled and stuff.
// Not final grammar.

// OVERALL NOTES:
// semicolons are not optional must be used
// A semicolon ends a statement (newlines/whitespace are ignored by the parser).
// Private and Public will not be added. maybe in the future

//==============================
//src/main.wisp
//==============================

// If missing compiler assumes latest compatible version.
// @wisp may only  appear in the file containing app main()
// If it appears in any other file -> compile error.
// If it appears in multiple files -> compile error.
@wisp 0.0.1;

// allows you to import code and functions 
// looks for matching file in your source directory e.g: import math -> src/math.wisp
import math;

// custom types for functions and stuff (This is a example of a type and i not used in the code yet)
type Person {
    name: String;
    age: Number;
}

// function that takes the input of two numbers a & b and outputs a number
// functions must be outside the app main()
function add(a: Number, b: Number): Number {
    return a + b;
}

// Main Entry Point of all wisp files this is where the code starts running
// app main() is not changable as of 0.0.1 but will add support for diffrent subsystems later in developement
app main(){
    const name: String = "willow";
    print(name);

    const multiplyProduct = math.multiply(5,9);
    print(multiplyProduct);

    const divideProduct = math.divide(9,3);
    print(divideProduct);

    const minusProduct = math.minus(5,9);
    print(minusProduct);


    // calles the add function and store it into added. no type needed because it is defined from add function
    const added = add(6, 8);
    print(added);
}

//==============================
//src/math.wisp
//==============================

// No @wisp here (only allowed in the app main file)

function multiply(a: Number, b: Number): Number {
    return a * b;
}

function divide(a: Number, b: Number): Number {
    return a / b;
}

function minus(a: Number, b: Number): Number {
    return a - b;
}