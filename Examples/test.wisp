@wisp 0.0.1;

app main(){

    // Basic compare
    if (a == b) {
        print("a equals b");
    }

    // Not equal
    if (a != b) {
        print("a not equal b");
    } else {
        print("a equals b (else branch)");
    }

    // Less / Greater
    if (x < y) {
        print("x < y");
    }

    if (x > y) {
        print("x > y");
    }

    // LessEqual / GreaterEqual
    if (score <= 100) {
        print("score is <= 100");
    }

    if (temperature >= 32) {
        print("freezing or above");
    }

    // Comparisons with numbers and spacing variations
    if (count==0) {
        print("count is zero");
    }

    if (count !=0) {
        print("count is not zero");
    }

    if (  10   <   20  ) {
        print("weird spacing still works");
    }

    // Using identifiers and dot access inside a condition
    if (math.value >= limit) {
        print("math.value >= limit");
    }

    // Assignment inside blocks (tests '=' token)
    if (flag == 1) {
        result = 42;
        print("set result");
    } else {
        result = 0;
        print("reset result");
    }

    // Test comments next to operators
    if (a == b) { // comment after condition
        print("comment test");
    }

    // Test comment that contains operator-looking text
    // if (x >= y) { print("should not tokenize inside comments"); }

    if (x >= y) {
        print("real compare after fake comment");
    }
}
