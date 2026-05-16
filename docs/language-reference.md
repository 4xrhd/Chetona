# Chetona Language Reference

## Syntax Overview

Chetona uses Bengali keywords while maintaining C++ syntax structure. The compiler translates Chetona source code to standard C++.

## Program Structure

### Main Function

Every Chetona program must have exactly one `aromvho()` function:

```chetona
aromvho() {
    // Your code here
    shomapti();
}
```

### Program Exit

Use `shomapti()` to exit the program:

```chetona
shomapti();  // equivalent to return 0;
```

## Variables

### Declaration

```chetona
dhoro x = 10;
dhoro name = "Alice";
```

### Assignment

```chetona
x = 20;
name = "Bob";
```

## Conditionals

### If Statement

```chetona
shorto (condition) {
    // code
}
```

### If-Else

```chetona
shorto (condition) {
    // code
} onno_thay {
    // else code
}
```

### If-Else If-Else

```chetona
shorto (condition1) {
    // code
} natuva (condition2) {
    // else if code
} onno_thay {
    // else code
}
```

## Loops

### While Loop

```chetona
ghurnon (condition) {
    // code
}
```

### For Loop

```chetona
jotokhon (dhoro i = 0; i < 10; i = i + 1) {
    // code
}
```

## Functions

### Arithmetic

```chetona
shonkolon(a, b)  // a + b
biyojon(a, b)    // a - b
gunon(a, b)      // a * b
vabhon(a, b)     // a / b
mod(a, b)        // a % b
```

### Comparison

```chetona
shomota(a, b)  // a == b
boro(a, b)     // a > b
choto(a, b)    // a < b
```

### I/O

```chetona
prodorshon(value)        // Print
grohon_shobdo()          // Read string
grohon_shonkhya()        // Read number
```

## Complete Example

```chetona
aromvho() {
    prodorshon("Enter a number:");
    dhoro num = grohon_shonkhya();
    
    shorto (num > 0) {
        prodorshon("Positive");
    } natuva (num < 0) {
        prodorshon("Negative");
    } onno_thay {
        prodorshon("Zero");
    }
    
    shomapti();
}
```
