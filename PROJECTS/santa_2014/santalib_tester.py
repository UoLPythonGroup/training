import ctypes

compute_task = ctypes.cdll.LoadLibrary('./libsanta.so').compute_task
compute_task.retval = None  # function returns void

duration = 2000
starttime = 1000
old_productivity = ctypes.c_double(1.5)
actual_duration = ctypes.c_int()
elf_available = ctypes.c_int()
new_productivity = ctypes.c_double()

compute_task(duration, starttime, old_productivity, ctypes.byref(actual_duration), 
             ctypes.byref(elf_available), ctypes.byref(new_productivity))

print('actual_duration  = {:d}'.format(actual_duration.value))
print('elf_available    = {:d}'.format(elf_available.value))
print('new_productivity = {:.16f}'.format(new_productivity.value))

# print('Comparing with reference implementation ...')
# import sys
# sys.path.append('./HelpingSantasHelpers')
# 
# from hours import Hours
# from toy import Toy
# from elf import Elf
# import math
# 
# myelf = Elf(1)
# myelf.rating = old_productivity.value
# mytoy = Toy(1, '2015 01 01 02 05', duration)
# ref_duration = int(math.ceil(mytoy.duration / myelf.rating))
# hrs = Hours()
# myelf.update_elf(hrs, mytoy, starttime, ref_duration)
# 
# print('actual_duration  = {:d}'.format(ref_duration))
# print('elf_available    = {:d}'.format(myelf.next_available_time))
# print('new_productivity = {:f}'.format(myelf.rating))
