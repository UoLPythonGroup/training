# -*- coding: utf-8 -*-
"""
Implement function for generating permutations.

Discussed in meeting of 19 February 2016.
"""

def permutations_internal(partial_list, 
                          remaining_numbers):
    """Returns a list of permutations starting from 
    partial_list with all zeros filled in from remaining_numbers"""
    if len(remaining_numbers) == 1:
        partial_list = partial_list[:]
        index = partial_list.index(0)
        partial_list[index] = remaining_numbers[0]
        return [partial_list]
        
    result = []
    first_number = remaining_numbers[0]
    for index, number in enumerate(partial_list):
        if number == 0:
            new_partial_list = partial_list[:]
            new_partial_list[index] = first_number
            new_remaining_numbers = remaining_numbers[1:]
            tmp = permutations_internal(new_partial_list,
                                        new_remaining_numbers)
            result.extend(tmp)
    return result

def permutations(size):
    """Returns a list of permutations of {1, ..., size}"""
    return permutations_internal(size * [0], 
                                 range(1, size+1))
    
if __name__ == '__main__':    
    print(len(permutations(6)))
