# Testing

## Why test

- [Statins alert over IT glitch in heart risk tool](http://www.bbc.co.uk/news/health-36274791)  You probably don't want your GP using software with a bug in it.

Closer to academic home:

- [The Reinhart-Rogoff error – or how not to Excel at economics ](https://theconversation.com/the-reinhart-rogoff-error-or-how-not-to-excel-at-economics-13646)
  - A mistake in an excel spreadsheet lead to wrong conclusions in a highly cited ("impactful") work of economics
  - The [academic paper](http://www.peri.umass.edu/fileadmin/pdf/working_papers/working_papers_301-350/WP322.pdf) which rebutted the work makes excellent reading (and, under my interpretation, the Excel error is perhaps the least of their sins)
  - The [Conversation Article](https://theconversation.com/the-reinhart-rogoff-error-or-how-not-to-excel-at-economics-13646) makes an excellent plea for properly [Reproducible Research](http://reproducibleresearch.net/)
  - But that's a rant for another day.

I also want to make that case that testing will:

- Lead you to write better code
- Make it easier to change and update code in the future
- Make it less stressful to develop code
- Ultimately make you more productive, though it may not seem so at the start.


# We all test

No doubt, when you were learning to code, you played a lot with the [REPL](https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop).
This is interactive coding.  Typing and seeing what happens.

When you moved on to writing small programs, you probably still checked things as you went along.  Added `print` statements, and thought about whether the result was correct.  Used a debugger to trace a program through.  Ran the code, and checked the output.

[Software testing](https://en.wikipedia.org/wiki/Software_testing) is a vast field, employing many, many people.  I want to concentrate upon:

- [Unit testing](https://en.wikipedia.org/wiki/Unit_testing), which is writing _automatic_ tests of small _units_ of code.
- [Test driven development](https://en.wikipedia.org/wiki/Test-driven_development) which is, loosely, a way of using unit tests to help you design software.


# Why make tests _automatic_

For me, the key idea is to make things as _automatic_ as possible.  Get the computer to help you.  If you are manually checking things, then each time you change the software (to "correct" a bug, or add a "feature") you have to re-check everything that might have changed.  What if you accidently change something which you didn't mean to?

If you have a load of _automatic_ tests, you can just re-run them (or even get the computer to automatically run them every time you do something -- in a nut-shell, [Continuous integration](https://en.wikipedia.org/wiki/Continuous_integration)).

As you get used to writing tests, you can start to plan software using tests.  Write the test _first_.  This allows you to think about what you want your software to do.  What inputs and output should my function have?  How should it deal with invalid inputs?  You code all this as a test, and then write the function.  In this way, the tests almost become a form of documentation, describing _in code_ how the software is meant to run.  It's documentation which cannot become out of date, because if you change how the software functions, but don't update the tests, then the tests will start to fail.


# Python testing

Python comes with a useful test framework, the `unittest` module.  But it's based on the `xUnit` model (`JUnit` in Java, for example) which is not terribly _Pythonic_.  I've found using [`pytest`](https://docs.pytest.org/en/latest/) to be easier.

You have to understand a little about how [Python modules](https://docs.python.org/3/tutorial/modules.html) work.  Let's create a module called `spam` (see the folder [testing_example](testing_example/)):

- A Python package is just a directory, here named `spam`...
- Which contains a "magic file", called `__init__.py`.  It can be empty.
- Let's also create a file `math.py` in `spam` with the contents:

      def sqrt(n):
        """Compute the +ve square root
        
        :param n: Non-negative number.

        :return: The square root.
        """
        pass

- Now create a directory `tests` in the same directory which contains `spam`
- Also create an empty `__init__.py` is `tests`
- Navigate to the main directory, and type `pytest`
- Something should run, and report that `no tests ran`.

So, let's create a test.  Under `tests` create the file `math_test.py` with contents:

      import spam.math

      def test_sqrt():
        assert spam.math.sqrt(4) == 2

The first line imports the file `math.py` from the `spam` package.  The function tests that our square-root function computes the square-root of 4 as 2.  Now run `pytest`.  You should see a test failure (as we haven't written our square-root function yet...)

A `pytest` test file is just a normal Python module, but any function with a name starting as `test` will be run as a test.  Simple as that!  (And very Pythonic-- it just works).  More advanced things to look into are "fixtures" and "mocking" (the `unittest.mock` package is great, I think).

---

Anyway, as an *exercise*, implement your favourite way to find the square-root.

---

If you're doing it by hand, your function may return `2.000000001` as the square-root.  We should be happy with this, as we made no promises about returning an _integer_ if the input was a perfect square.  `pytest` can help us.  Replace `math_test.py` with:

      import pytest
      import spam.math

      def test_sqrt():
        assert spam.math.sqrt(4) == pytest.approx(2)

This is, roughly, equivalent to

        assert abs(  spam.math.sqrt(4) - 2   ) < something_small

Notice, however, that you can just read and understand the first version.
This style of using (hidden away) clever Object Oriented Ideas to create code you
can read as a sentence is called a [Fluent Interface](https://en.wikipedia.org/wiki/Fluent_interface).

---

Some further exercises:

- Write some more tests.  Check the square-roots of some more numbers.
- Decide what should happen if you pass a _negative_ number to the square-root function.
- Write the test.  Check it fails.  Modify the function.  Check that the test now passes.


# Refactoring

A huge use of tests comes about when you need to modify code.  Suppose you leave your square-root function alone for a few months, and then need to modify it somehow.  Maybe so it can optionally take a list of numbers, and return the square-root of each one.
You do this, but you want to make sure you haven't broken the old behaviour.  With the tests in place, you literally do nothing-- just rerun `pytest` and check nothing is broken!


# Less stress

I hate the feeling of writing code, then running it, and it... not working.  As projects become more complicated, as you work with other people, or external libraries, it becomes harder and harder to keep track of everything, to think about what can go wrong.

I've found that writing tests (I rarely, if I'm honest, go full TDD) forces a certain discipline on me.

- One of the goals of more formal software design is to produce [De-coupled code](https://en.wikipedia.org/wiki/Coupling_(computer_programming)) -- "units" of code which do one thing, do it well, and aren't "coupled" to other bits of code.
- This means I can _use_ other modules, but I should use them in a carefully controlled way.  Different parts of the software should be able to change without everything needing to change.
- See the e.g. [SOLID](https://en.wikipedia.org/wiki/SOLID_(object-oriented_design)) principles.

Writing code which is _easy_ to test almost _forces_ you to design code in a decoupled way.

- Once you have the tests, you have a safety net.  You can fiddle with things, and have some certainty that you're not breaking things.

The first time I spent a month writing code, writing tests, checking that each "unit" of code worked, and then putting it all together into an application and it just... working, was a revelation.

I use tests, and writing _testable code_, as a debugging method.

- You split code into very small parts.
- Short functions or methods.
- Test each one.
- I get lots of things wrong.
- But because I have 10 lines of code where the error _must_ be, it doesn't feel stressful.
- It's more like problem solving, something to be dispatched in 30 seconds, test now passes, get on to the next task...
- Instead of the grinding horror of "why isn't this working..."

It has made me more productive.  It takes longer to get going, what with the hassle of writing tests, and formally writing packages etc.  But in the long run, I save so much time...
