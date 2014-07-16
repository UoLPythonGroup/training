Z = [[0,0,0,0,0,0],
     [0,0,0,1,0,0],
     [0,1,0,1,0,0],
     [0,0,1,1,0,0],
     [0,0,0,0,0,0],
     [0,0,0,0,0,0]]

def compute_neighbours(Z):
    rows,cols = len(Z), len(Z[0])
    N  = [[0,]*(cols)  for i in range(rows)]
    for x in range(1,cols-1):
        for y in range(1,rows-1):
            N[y][x] = Z[y-1][x-1]+Z[y][x-1]+Z[y+1][x-1] \
                    + Z[y-1][x]            +Z[y+1][x]   \
                    + Z[y-1][x+1]+Z[y][x+1]+Z[y+1][x+1]
    return N

def show(Z):
    for l in Z[1:-1]: print l[1:-1]
    print

def iterate(Z):
    rows,cols = len(Z), len(Z[0])
    N = compute_neighbours(Z)
    for x in range(1,cols-1):
        for y in range(1,rows-1):
            if Z[y][x] == 1 and (N[y][x] < 2 or N[y][x] > 3):
                Z[y][x] = 0
            elif Z[y][x] == 0 and N[y][x] == 3:
                Z[y][x] = 1
    return Z

show(Z)
for i in range(4):
    iterate(Z)
show(Z)
