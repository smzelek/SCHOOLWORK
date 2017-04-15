from konane import *
from konaneGUI import *
# Carly Marshall & Steve Zelek

class MinimaxPlayer(Konane, Player):
    def __init__(self, size, depthLimit):
        Konane.__init__(self, size)
        self.limit = depthLimit

    def initialize(self, side):
        self.side = side
        self.name = "MinimaxPlayer" + str(self.limit)
        self.boardValues = [[1, 1, 1, 1, 1, 1, 1, 1],
                            [1, 2, 2, 2, 2, 2, 2, 1],
                            [1, 2, 3, 3, 3, 3, 2, 1],
                            [1, 2, 3, 4, 4, 3, 2, 1],
                            [1, 2, 4, 4, 4, 3, 2, 1],
                            [1, 2, 3, 3, 3, 3, 2, 1],
                            [1, 2, 2, 2, 2, 2, 2, 1],
                            [1, 1, 1, 1, 1, 1, 1, 1]]

    def getMove(self, board):
        return self.minimax(board, 0, -float("inf"), float("inf"))

    def minimax(self, board, depth, alpha, beta):
        if (depth == self.limit):
            return self.eval(board)
        if (depth % 2 == 0):
            return self.maxPlay(board, depth, alpha, beta)
        else:
            return self.minPlay(board, depth, alpha, beta)

    def maxPlay(self, board, depth, alpha, beta):
        curSide = (self.side)
        moves = self.generateMoves(board, curSide)

        highestScore = float("-inf")
        moveToMake = []

        for move in moves:
            result = self.minimax(self.nextBoard(board, curSide, move), depth+1, alpha, beta)

            if (result > highestScore):
                highestScore = result
                moveToMake = move
                if (result > alpha):
                    alpha = result
                    if (alpha >= beta):
                        break

        if (depth == 0):
            return moveToMake
        else:
            return highestScore

    def minPlay(self, board, depth, alpha, beta):
        curSide = self.opponent(self.side)
        moves = self.generateMoves(board, curSide)

        lowestScore = float("inf")

        for move in moves:
            result = self.minimax(self.nextBoard(board, curSide, move), depth+1, alpha, beta)

            if (result < lowestScore):
                lowestScore = result
                if (result < beta):
                    beta = result
                    if (alpha >= beta):
                        break

        return lowestScore

    def pieceHeuristic(self, board):
        pieceH = 0
        for r in range(0, 8):
            for c in range(0, 8):
                if (self.contains(board, r, c, self.side)):
                    pieceH += self.boardValues[r][c]

        return pieceH

    def eval(self, board):

        oppMoves = len(self.generateMoves(board, self.opponent(self.side)))

        if (oppMoves == 0):
            return float("inf")

        moves = len(self.generateMoves(board, self.side))
        numDots = self.countSymbol(board, ".")
        pieceH = self.pieceHeuristic(board)

        return (64 - numDots) * (pieceH / 64) + (numDots) * (moves - oppMoves) / 10


game = KonaneGUI(8, MinimaxPlayer(8, 2), RandomPlayer(8))
