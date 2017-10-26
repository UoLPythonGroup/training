# Code style

[Wikipedia](https://en.wikipedia.org/wiki/Programming_style) gives the classical definition of code style, which focuses on the:

- Layout
- Indentation
- Use of white space

Python forces some of this upon you (which is a good thing, IMHO).  I want to broaden the discussion to make "code style" more about generally "how you write code".


# Headline message

- [Why Coding Style Matters](https://www.smashingmagazine.com/2012/10/why-coding-style-matters/) is an excellent short article.

> “Programs are meant to be read by humans and only incidentally for computers to execute.”
> 
> — H. Abelson and G. Sussman (in “Structure and Interpretation of Computer Programs”)

> “Code is read much more often than it is written, so plan accordingly.
>
> Design for readability.
>
> Even if you don't intend anybody else to read your code, there's still a very good chance that somebody will have to stare at your code and figure out what it does: That person is probably going to be you, twelve months from now.”
> 
> — [Raymond Chen](https://blogs.msdn.microsoft.com/oldnewthing/20070406-00/?p=27343)


# First example

This is an example I made up:

        import random

        # Number of trials
        n = 10000

        m = 0
        while n > 0:
            x = random.random()
            y = random.random()
            m += (x*x + y*y <= 1)
            n -= 1

        print("Pi is about", m * 2500)

Think about how you might improve this.
- What are some really obvious problems?
- How can you make it easier for another human to understand?

---

If you're following along at home, try to answer the above questions.  Let me stress that there is no right answer (other than "My boss told me so") here.  What I think is important is that you think about the issues.

---

My effort is at [example1/good.py](example1/good.py).  Some of the things I thought about:

- Don't hard-code numbers!  If you change `10000` you also need to change `2500`.
- Factor the code better-- make a function.
- _Name variables with meaning_.
- What are Python specific [patterns](https://en.wikipedia.org/wiki/Software_design_pattern) or [idioms](https://en.wikipedia.org/wiki/Programming_idiom)?  By writing code in a _common style_ you make it easier for other people to understand.
- Avoid "tricks" unless absolutely necessary.  The following are equivalent:

        m += (x*x+y*y<=1)

        if x*x + y*y <= 1:
            m = m + 1

- I think the second form is far easier to read.

An unfortunate fact of life is that _production_ code sometimes ends up balancing form and function.  Look at some of the Python library code-- it's not that elegant!  If I was programming such a numerical algorithm for the day job, I would use `numpy` and the end result would be something like [numpy_code.py](example1/numpy_code.py).  Here I have written some documentation, and then used a bag of `numpy` tricks to perform the same calculation.

# Express meaning _in code_

Let's think about the python `datetime` module.  What do you think the following imaginary code does:

        import time
        import my_module

        time.sleep(3)

        my_module.start_process("Process name", 1243)

Some questions:

- What does `time.sleep(3)` do?
- It sleeps (pauses execution) for 3 time units.
- Is that 3 seconds?  3 minutes?  3 milli-seconds?

If I told you that `start_process` has signature

        def start_process(process_name, abort_timeout)

you might guess that `abort_timeout` is some time span.  Is it `1243` seconds?  Minutes?  Hours?

Now consider:

    import datetime
    import my_better_module

    pause_time = datetime.timedelta(seconds=3)
    my_better_module.sleep_for(pause_time)

    my_better_module.start_process(process_name = "name", abort_timeout = datetime.timedelta(minutes=1243))

This is completely clear from reading.  Here I have used two ideas:

- A timespan, for a computer, is just a number.
- But really a timespan is a _number_ and a _unit_.
- By encoding this idea in code (techically, by using the class `timedelta` instead of just a number) you can signal your intent in the computer language itself.
- I also made use of Python's ability to have _named parameters_.  I don't have to remember the order of the arguments to a function, but can instead put their names in.  Again, this improves readability.
- (It also makes functions with many, many parameters easier to work with.  In e.g. Java you'd often end up using a Builder pattern, or the pattern of having properties and getters and setters, instead.)



# Put outside the code as little as possible

I am not a big fan of comments.  I've met professional programmers who hate comments, and are distrustful of documentation.

- The key idea above is to _write in the programming language_.  Try to embed as much meaning as possible in the language.

### Some anti-patterns

Don't write comments which are obvious

    # Run the optimisation
    optimise(data, 5)

Don't write comments which don't actually have content

    # Performs the optimisation step 
    def optimise(data, iterations):

Try to avoid the pattern of writing comments to "keep track of things" or because functions are getting long and complication.

- Keep functions really short.  Your editor should display the whole of a function on one screen.
- I've heard said: functions should be at most 5 lines long.
- Split functions up into sub-functions.
- Name the sub-functions with meaningful names so you can read the code almost like an English sentence.



### Why?

- Code is the only thing, ultimately, which matters.
- It will get updated by you, or by your colleagues.
- Comments often aren't updated.  There is nothing worse than a comment which flatly contradicts the code, because you don't know what to trust.
- The same goes for documentation, to an extent.
- If you are writing code for others to use (which could well be yourself, next week) then you don't want people to have to read your code.  Comments can't be understood by tools.
- But my IDE can show me the definition of a function or class.  And it can show documentation.
- If the programming language is a language, then comments are like a meta-language.
If you need to add a comment to clarify something, then it might be better to re-express the concept in the language itself.


## Some examples

I've found it really good practise (but hard, and often humbling) to read other's code.
Look at Python code on GitHub.

- [Requests](https://github.com/requests) is a great library for web scraping.
   - [cookies.py](https://github.com/requests/requests/blob/master/requests/cookies.py) is code to read and write "cookies" (small bits of state, which are stored by your web browser, and used by web sites to tell who you are, if you are logged in, etc.)  
   - Relatively few comments, and almost all document "business rules".  I.e. concepts
   which are extremely hard to describe in code.
   - But I would still say there are silly comments:

            def multiple_domains(self):
                """Returns True if there are multiple domains in the jar.
                Returns False otherwise.
                :rtype: bool
                """
                domains = []
                for cookie in iter(self):
                    if cookie.domain is not None and cookie.domain in domains:
                        return True
                    domains.append(cookie.domain)
                return False # there is only one domain in jar

    - The comment says _exactly_ what the doc-string does, so it adds nothing.
    - It does not, for example, clarify some unusual bit of coding which might be
    hard to follow.
    - (And if it did, arguably you should work hard to make the code more standard, and
    so less in need of a comment.  Win win.)

- The [Python](https://github.com/python/cpython) language itself is on GitHub.
    - [datetime.py](https://github.com/python/cpython/blob/3.6/Lib/datetime.py) is the code for the `datetime` library discussed above.
    - Some parts have lots of comments.
    - And some parts almost none.
    - I wonder if this is because of multiple authors?  (Remember, I'm describing my coding style here.  You can disagree.  Just disagree thoughtfully.)
    - Some of the comments are essentially careful descriptions of algorithms (e.g.
    how timezones work).  This seems reasonable to me.


# Further reading

- [Style Guide](http://docs.python-guide.org/en/latest/writing/style/) from the excellent [The Hitchhiker’s Guide to Python](http://docs.python-guide.org/en/latest/).
- I've found reading "cookbook" style books to be really good for seeing how other people structure code.
- For example, [Python cookbook / edited by Alex Martelli, Anna Martelli Ravenscroft, and David Ascher](http://lib.leeds.ac.uk/record=b2510239~S5)  [Amazon](https://www.amazon.co.uk/Python-Cookbook-David-Beazley/dp/1449340377)
- For Java, [Effective Java](http://lib.leeds.ac.uk/record=b3591469~S5) is great, [Amazon](https://www.amazon.co.uk/Effective-Java-Joshua-Bloch/dp/0134685997)
- [Fluent Python](https://www.amazon.co.uk/Fluent-Python-Luciano-Ramalho/dp/1491946008)  is a great "second book" about Python, and spends a lot of time talking about Python idioms, and how to make best use of the language features.

# Finally... Practice

Coding is a practical skill, and nothing beats just doing it.  [10000 hours](https://www.amazon.co.uk/Outliers-Story-Success-Malcolm-Gladwell/dp/0141036257)  Which is of course nonsense.  But there seems to be something in _deliberate_ practice: doing something, and then thinking about how you might do it better.
