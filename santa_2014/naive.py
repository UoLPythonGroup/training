import numpy, heapq, datetime

# This is a script, so we'll tolerate some global variables, and "magic numbers"
dtstart = datetime.datetime(2014,1,1,0,0)
daystarts = 9*60
dayends = 19*60
minsday = 24*60
sanctionedminsday = dayends - daystarts
unsanctionedminsday = minsday - sanctionedminsday

def ssu_spliter(duration, place, start, end):
    """Input: duration, place, start, end where start--end represents
    an interval of time, place is the current place in the day, and
    duration is the total duration.  Intersects [place,
    place+duration] with the interval, and returns the 'delta' which
    is the maximum we can add to place to either get duration==0 or to
    reach the end of the interval; returns 0 if no intersection"""
    if start <= place and place < end:
        return min(duration, end - place)
    return 0

def split_sanctioned_unsanctioned(starttime, duration):
    """Split time period into sanctioned and unsanctioned minutes
    Input: starttime, integer, time since 2014 in minutes duration,
    integer, time in minutes 
    Returns: (sanctioned, unsanctioned)"""
    san = 0
    unsan = 0
    # In any 24 hour period, always the same number of sanctioned and
    # unsanctioned minutes so deal with hour days first, and then with
    # remainder
    wholedays = duration // minsday
    san += wholedays * sanctionedminsday
    unsan += wholedays * unsanctionedminsday
    duration -= wholedays * minsday
    # So now duration < A Day, but still might overlap everything Only
    # care about starttime relative to current day now
    day_starttime = starttime % minsday
    while duration > 0:
        # Before start of day
        delta = ssu_spliter(duration, day_starttime, 0, daystarts)
        unsan += delta
        day_starttime = (day_starttime + delta) % minsday
        duration -= delta
        # Working hours
        delta = ssu_spliter(duration, day_starttime, daystarts, dayends)
        san += delta
        day_starttime = (day_starttime + delta) % minsday
        duration -= delta
        # After working hours
        delta = ssu_spliter(duration, day_starttime, dayends, minsday)
        unsan += delta
        day_starttime = (day_starttime + delta) % minsday
        duration -= delta
    return (san,unsan)


class ElfExcept(Exception):
    pass

class Elf:
    def __str__(self):
        return "Elf id:{}, productivity:{}, next_free:{}".format(
            self.elfid, self.productivity, self.available_to_work)
    def __init__(self, elfid):
        self.productivity = 1.0
        self.elfid = elfid
        # Time the Elf is next able to work
        self.available_to_work = 60*9 # Work starts at 9am Jan 1st
        self.logging = False # Create a history
        if self.logging:
            self.history = []
    def __lt__(self, other):
        return self.elfid < other.elfid
    def do_task(self, starttime, duration):
        import math
        if starttime < self.available_to_work:
            raise ElfExcept("Elf {} not available to work at time {}.".format(self.elfid,starttime))
        # So now do all the calculation
        actual_duration = math.ceil(duration / self.productivity)
        san, unsan = split_sanctioned_unsanctioned(starttime, actual_duration)
        self.available_to_work = starttime + actual_duration
        if self.logging:
            log = "At time {} assigned task of length {} with productivity {}".format(starttime, duration, self.productivity)
            log += " which took me {}, sanctioned:{}, unsanctioned:{}".format(actual_duration, san, unsan)
        # Update productivity
        new_prod = (1.02 ** (san/60.0)) * (0.9 ** (unsan/60.0)) * self.productivity
        if new_prod < 0.25:
            new_prod = 0.25
        if new_prod > 4.0:
            new_prod = 4.0
        self.productivity = new_prod
        # Work out rest time needed
        # Copied this logic from the official Python code, because of
        # "bug"/"feature".
        if unsan > 0:
            num_days_since_jan1 = self.available_to_work // minsday
            rest_time = unsan
            rest_time_in_working_days = rest_time // sanctionedminsday
            rest_time_remaining_minutes = rest_time % sanctionedminsday
            local_start = self.available_to_work % minsday
            if local_start < daystarts:
                local_start = daystarts
            elif local_start > dayends:
                num_days_since_jan1 += 1
                local_start = daystarts
            if local_start + rest_time_remaining_minutes > dayends:
                rest_time_in_working_days += 1
                rest_time_remaining_minutes -= (dayends - local_start)
                local_start = daystarts
            total_days = num_days_since_jan1 + rest_time_in_working_days
            self.available_to_work = total_days * minsday + local_start + rest_time_remaining_minutes
        # if available_to_work is exactly 19:00, set it to 9:00 of next day
        if self.available_to_work % minsday == dayends:
            self.available_to_work += minsday - dayends + daystarts
        if self.logging:
            log += " will get off rest time at {}.".format(self.available_to_work)
            self.history.append( log )
        return actual_duration


def create_elves(num_elves):
    """ Elves are stored in a sorted list using heapq to maintain
    their order by next available time.  List elements are a tuple of
    (next_available_time, elf object)
    :return: list of elves
    """
    list_elves = []
    for i in range(1, num_elves+1):
        elf = Elf(i)
        heapq.heappush(list_elves, (elf.available_to_work, elf))
    return list_elves


def solution_firstAvailableElf(toys, num_elves):
    """ Creates a simple solution where the next available elf is
    assigned a toy. Elves do not start work outside of sanctioned
    hours.
    :param toys: array with toys
    :param num_elves: number of elves
    :return: solution
    """
    myelves = create_elves(num_elves)
    solution = numpy.empty((toys.shape[0], 4), dtype=numpy.int32)
    
    for toy_index, (toy_starttime, toy_duration) in enumerate(toys):
        # get next available elf
        elf_available_time, current_elf = heapq.heappop(myelves)

        work_starttime = max(elf_available_time, toy_starttime)
        actual_duration = current_elf.do_task(work_starttime, toy_duration)
        
        # put elf back in heap
        heapq.heappush(myelves, (current_elf.available_to_work, current_elf))

        solution[toy_index, 0] = toy_index
        solution[toy_index, 1] = current_elf.elfid
        solution[toy_index, 2] = work_starttime
        solution[toy_index, 3] = actual_duration
        
    return solution


if __name__ == '__main__':
    toys = numpy.load('toys_1000.npy')
    solution = solution_firstAvailableElf(toys, 10)
    for rowid in range(-20, 0):
        toyid, elfid, starttime, duration = solution[rowid, :]
        starttime_as_date = dtstart + datetime.timedelta(minutes = int(starttime))
        starttime_as_string = starttime_as_date.strftime("%Y %m %d %H %M")
        print('Toy {:4d}   Elf {:2d}   Starttime {:s}   Duration {:d}'
              .format(toyid, elfid, starttime_as_string, duration))
