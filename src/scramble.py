from random import randint

sides = ["F", "U", "R", "B", "D", "L"]
end = ["", "'", "2"]


second_last_move = "_"
last_move = "_"

scrambles = ""
for _ in range(1000):
    scramble_len = 0
    while scramble_len < 50:
        move = sides[randint(0, 5)] + end[randint(0, 2)]
        if move[0] != last_move[0] and {"F":"B", "U":"D", "R":"L", "B":"F", "D":"U", "L":"R"}[move[0]] != last_move[0]:
            scrambles += move + " "
            scramble_len += 1
    scrambles += "\n"

scrambles = scrambles[:-1] #Remove the last newline

with open("./test_cubes.txt", "w") as f:
    f.truncate(0)
    f.write(scrambles)
    f.close()