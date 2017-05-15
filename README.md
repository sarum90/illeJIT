# illeJIT

A tiny JIT with very few capabilities. Made for expression evaluation, but only
capable of generating assembly code that loads values into registers and calls
existing functions.

It does not handle register allocation for you, so you have to be very
intentional when you generate code to ensure you are following Linux x86\_64
calling conventions.

Currently handles expressions with:

- Integral, undefined, and boolean values.
- literals and variables that can be modified after the expression is JIT'd
- integer multiplication, addition, and less than comparison
- boolean AND

Generates code that calls functions that operate on an expression stack that
lives in the heap.

Currently isn't very fast, when compared to lazy evaluation of the same
expression tree, the interpreted version appears to be faster, even for
relatively large expressions:

```
./test
JIT: 41.567427ns per run
EVAL: 41.358897ns per run
```

In theory as more expression types are added to expression engine, or after
optimization work on the JIT, I would expect the JIT to outperform the
interpreted version.
