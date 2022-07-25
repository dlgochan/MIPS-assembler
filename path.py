import sys

stack = []
path = []
min = sys.maxsize
def printBoard(board):
    for row in board:
        print(row)
def nextStep(x, y, board, price, dir):
    global stack, path, min;
    
    if x >= len(board) or y >= len(board) or x < 0 or y < 0:
        return
    if board[x][y] == 1:
        # print(f"{x},{y} is wall")
        return
    if price >= min:
        return
    # printBoard(board)
    # print(price)
    stack.append((x,y))
    # print(stack)
    board[x][y] = 1

    if x == len(board)-1 and y == len(board)-1:
        min = price
        # print("min = ", min)
        # print(stack)
        stack.pop()
        board[x][y]=0
        return
    if dir == 0:
            nextStep(x, y+1, board, price+100, -1)
            nextStep(x, y-1, board, price+100, -1)
            nextStep(x+1, y, board, price+100, 1)
            nextStep(x-1, y, board, price+100, 1)
    elif dir == 1:
            nextStep(x, y+1, board, price+600, -1)
            nextStep(x, y-1, board, price+600, -1)
            nextStep(x+1, y, board, price+100, 1)
            nextStep(x-1, y, board, price+100, 1)
    elif dir == -1:
            nextStep(x, y+1, board, price+100, -1)
            nextStep(x, y-1, board, price+100, -1)
            nextStep(x+1, y, board, price+600, 1)
            nextStep(x-1, y, board, price+600, 1)
    stack.pop()
    board[x][y]=0
    return



def solution(board):
    global stack, path, min;
    answer = nextStep(x=0, y=0, board=board, price=0, dir=0)


# board = [[0,0,0],[0,0,0],[0,0,0]]	
board = [[0,0,0,0,0,0,0,1],[0,0,0,0,0,0,0,0],[0,0,0,0,0,1,0,0],[0,0,0,0,1,0,0,0],[0,0,0,1,0,0,0,1],[0,0,1,0,0,0,1,0],[0,1,0,0,0,1,0,0],[1,0,0,0,0,0,0,0]]
# board = [[0,0,1,0],[0,0,0,0],[0,1,0,1],[1,0,0,0]]
# board = [[0,0,0,0,0,0],[0,1,1,1,1,0],[0,0,1,0,0,0],[1,0,0,1,0,1],[0,1,0,0,0,1],[0,0,0,0,0,0]]
solution(board)
# print(min)