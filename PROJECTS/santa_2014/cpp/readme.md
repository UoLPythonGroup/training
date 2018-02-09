Santa code in CPP
=================


Filelist:

   - elf.? : Simple elf class; same as in Python
   - calendar.? : I couldn't find a good date/time solution in the C++ standard library, so this is a minimal hand-rolled solution, enough to read and write the CSV files
   - calendar_demo.? : unit test for above, and Python comparison code
   - load_npy.? : A (slightly) generic solution to loading .npy files
   - load_toys.? : Uses the above, but also parses the CSV files (slightly slowly).
   - load\_toys\_test.? : unit test again Python generated npy
   - scorer.? : C++ version of the scorer; agrees with Python score!  And is fast.
   - makefile : build
