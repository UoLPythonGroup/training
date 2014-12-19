# Python script to check our datetime calcultions

from datetime import datetime
from datetime import timedelta

ds = datetime(2014,1,1,0,0)

def tomin(dt):
	delta = dt - ds
	return int( delta.total_seconds()/60 )
	
print("'2014 1 2 3 4' ->", tomin(datetime(2014,1,2,3,4)))
print("'2014 11 12 13 14' ->", tomin(datetime(2014,11,12,13,14)))
print("'2016 5 6 10 53' ->", tomin(datetime(2016,5,6,10,53)))
print("'2018 8 9 10 11' ->", tomin(datetime(2018,8,9,10,11)))
print("'2020 2 15 10 0' ->", tomin(datetime(2020,2,15,10,0)))
print("'2020 3 15 10 0' ->", tomin(datetime(2020,3,15,10,0)))
print("'2200 9 10 23 55' ->", tomin(datetime(2200,9,10,23,55)))
print("'2450 8 7 6 5' ->", tomin(datetime(2450,8,7,6,5)))

print("16753 ->", ds + timedelta(minutes=16753))
print("1242123 ->", ds + timedelta(minutes=1242123))
print("5428452 ->", ds + timedelta(minutes=5428452))
print("14523 ->", ds + timedelta(minutes=14523))