# dflow
Straightforward data-flow analysis in C++.

## Building the Source
```
./autogen && ./configure && make
```

## Example Usage and Output
```
$ ./src/dflow ./tests/if-reach-00
> generating AST from ./tests/if-reach-00 ... done
> generating source from AST ...
> -- setting nesting depths ... done
> -- labeling ... done
> here is the annotated source:
  [a = 1] -- 1
  [b = 2] -- 5
  if [a > b] -- 10 then
      [b = a] -- 14
  else
      [skip] -- 18
  fi
> drawing ASTs ...
> -- writing ./tests/if-reach-00-ast.eps ... done
> -- writing ./tests/if-reach-00-dast.eps ... done
> drawing control flow graph ...
> -- writing ./tests/if-reach-00-cfg.eps ... done
> calculating reaching definitions data-flow ...
> -- gathering variables ...
> -- 2 found ...
> -- { a b }
> -- starting fixed point iteration ...
> -- done ...
> -- here are the rd sets ...
{ (a, ?) (b, ?) }
[a = 1] -- 1
{ (a, 1) (b, ?) }
{ (a, 1) (b, ?) }
[b = 2] -- 5
{ (a, 1) (b, 5) }
{ (a, 1) (b, 5) }
[a > b] -- 10
{ (a, 1) (b, 5) }
{ (a, 1) (b, 5) }
[b = a] -- 14
{ (a, 1) (b, 14) }
{ (a, 1) (b, 5) }
skip
{ (a, 1) (b, 5) }

```

## Helpful Resources
* [Data-flow analysis: reaching definitions](http://www.csd.uwo.ca/~moreno/CS447/Lectures/CodeOptimization.html/node7.html)
* [Control Flow Graphs](http://www.cs.utexas.edu/~pingali/CS380C/2013/lectures/CFG.pdf)
* [Basic block](http://en.wikipedia.org/wiki/Basic_block)
* [ANSI C Yacc grammar](http://www.lysator.liu.se/c/ANSI-C-grammar-y.html#expression)
* [Semantic Analysis Phases of Compilation](http://www.cs.sunysb.edu/~cse304/Fall08/Lectures/ast-handout.pdf)
* [Classical Dataflow Optimizations](http://www.cs.cmu.edu/afs/cs/academic/class/15745-s06/web/handouts/04.pdf)
