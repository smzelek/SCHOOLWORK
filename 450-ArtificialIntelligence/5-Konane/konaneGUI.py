from konane import *
from tkinter import *
from threading import Timer, Lock

#Steve Zelek
#updated 4/15/17 12:57 pm

class KonaneGUI(Konane):
    def __init__(self, size, p1, p2):
        Konane.__init__(self, size)
        self.root = Tk()
        self.p1 = p1
        self.p2 = p2
        self.playSpeed = 1.0
        self.playTimer = None

        matchTitle = Label(self.root, text=p1.name + " (B) vs. " + p2.name + " (W)")
        matchTitle.config(font=("Helvetica", 14))
        matchTitle.pack()
        self.matchTitle = matchTitle

        boardBackground = Frame(self.root, bg="#d1974b", highlightbackground="#281403", highlightthickness=10)
        canvas = Canvas(boardBackground, bg='white', width=400, height=400, relief=RIDGE)
        canvas.pack(padx=10, pady=10)
        boardBackground.pack()
        self.canvas = canvas

        moveLabel = Label(self.root, text="")
        moveLabel.config(font=("Helvetica", 12))
        moveLabel.pack()
        self.moveLabel = moveLabel

        buttonTray = Frame(self.root)

        b = Button(buttonTray,text="Play", command=self.startGamePlayer)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=0)
        self.playBtn = b

        b = Button(buttonTray,text="Pause", command=self.pauseGame)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=1)
        self.pauseBtn = b

        b = Button(buttonTray,text="Step", command=self.stepGame)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=2)
        self.stepBtn = b

        b = Button(buttonTray,text="Skip to End", command=self.skipGame)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=3)
        self.skipBtn = b

        buttonTray.pack()

        buttonTray = Frame(self.root)

        b = Button(buttonTray,text="Swap Players", command=self.swapPlayers)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=5)
        self.swapBtn = b

        b = Button(buttonTray,text="Reset", command=self.resetGame)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=4)
        self.resetBtn = b

        b = Button(buttonTray, text="Set Speed", command=self.setPlaySpeed)
        b.config(font=("Helvetica", 10))
        b.grid(row=0, column=6)
        self.speedBtn = b

        buttonTray.pack()

        self.drawBoard()
        self.resetGame()

        self.root.title("Konane - Hawaiian Checkers")
        self.root.mainloop()

    def setPlaySpeed(self):
        self.pauseGame()
        msgBox = MessageBox("New Play Speed:")
        result = msgBox.getText()
        if (result == ""):
            return

        self.playSpeed = float(result)
        if (self.playSpeed <= 0):
            raise ValueError("Play speed must a number greater than 0.")

    def setGameOver(self, state):
        if (state):
            self.playBtn.config(state=DISABLED)
            self.pauseBtn.config(state=DISABLED)
            self.stepBtn.config(state=DISABLED)
            self.skipBtn.config(state=DISABLED)
        else:
            self.playBtn.config(state=NORMAL)
            self.pauseBtn.config(state=NORMAL)
            self.stepBtn.config(state=NORMAL)
            self.skipBtn.config(state=NORMAL)

        self.gameOver = state

    def startGamePlayer(self):
        self.gamePaused = False
        self.playBtn.config(state=DISABLED)
        self.playGame()

    def playGame(self):
        if (self.gameOver or self.gamePaused):
            return

        self.lock.acquire()
        self.playNextMove(True)
        self.playTimer = Timer(self.playSpeed, self.playGame)
        self.playTimer.daemon = True
        self.playTimer.start()
        self.lock.release()

    def pauseGame(self):
        self.gamePaused = True
        self.playBtn.config(state=NORMAL)
        if (self.playTimer is not None):
            self.playTimer.cancel()

    def stepGame(self):
        if (self.gameOver):
            return

        self.pauseGame()
        self.lock.acquire()
        self.playNextMove(True)
        self.lock.release()

    def skipGame(self):
        if (self.gameOver):
            return

        self.pauseGame()

        self.lock.acquire()
        while (not self.gameOver):
            self.playNextMove(False)
        self.lock.release()

        self.canvas.delete("move")
        self.drawPieces()

    def showWinner(self):
        self.nextPlayersTurn()
        string = self.currentPlayer.name + " (" + self.currentPlayer.side + ") wins!"
        self.moveLabel.config(text=string)

    def resetGame(self):
        self.pauseGame()
        self.reset()
        self.p1.initialize('B')
        self.p2.initialize('W')
        self.matchTitle.config(text=self.p1.name + " (B) vs. " + self.p2.name + " (W)")
        self.moveLabel.config(text="")
        self.lock = Lock()
        self.setGameOver(False)
        self.gamePaused = False
        self.currentPlayer = self.p1
        self.canvas.delete("move")

        self.drawPieces()

    def swapPlayers(self):
        self.pauseGame()
        self.p1, self.p2 = self.p2, self.p1
        self.resetGame()

    def playNextMove(self, show):
        if (self.gameOver):
            return
        move = self.currentPlayer.getMove(self.board)
        if move == []:
            self.setGameOver(True)
            self.showWinner()
            return
        try:
            self.makeMove(self.currentPlayer.side, move)
        except KonaneError:
            print ("ERROR: invalid move by", self.currentPlayer.name)
            self.setGameOver(True)
            self.showWinner()
            return
        if show:
            self.drawPieces()
            self.explainMove(move)
            self.drawMove(move)

        self.nextPlayersTurn()

    def explainMove(self, move):
        string = self.currentPlayer.name + " moved from " + \
                    "(" + str(move[1]) + "," + str(move[0]) + ") to " + \
                    "(" + str(move[3]) + "," + str(move[2]) + ")."
        self.moveLabel.config(text=string)

    def nextPlayersTurn(self):
        if (self.currentPlayer == self.p1):
            self.currentPlayer = self.p2
        else:
            self.currentPlayer = self.p1

    def drawBoard(self):
        self.canvas.config(bg="#d1974b", highlightthickness=0)

        tileSize = int(self.canvas["width"])/self.size
        tileSize = 50
        pieceSize = tileSize*.7
        for r in range(self.size):
            for c in range(self.size):
                coords = (r*tileSize+(tileSize-pieceSize)/2, \
                c*tileSize+(tileSize-pieceSize)/2, \
                (r+1)*tileSize-(tileSize-pieceSize)/2, \
                (c+1)*tileSize-(tileSize-pieceSize)/2)
                self.canvas.create_oval(coords, width=2, fill="#604520", outline="#604520")

    def drawPieces(self):
        self.canvas.delete("pieces")
        tileSize = int(self.canvas["width"])/self.size
        pieceSize = tileSize*.45
        for r in range(self.size):
            for c in range(self.size):
                coords = (r*tileSize+(tileSize-pieceSize)/2, \
                c*tileSize+(tileSize-pieceSize)/2, \
                (r+1)*tileSize-(tileSize-pieceSize)/2, \
                (c+1)*tileSize-(tileSize-pieceSize)/2)

                if (self.contains(self.board, r, c, "B")):
                    self.canvas.create_oval(coords, width=2, fill="black", outline="#494949", tags="pieces")
                elif (self.contains(self.board, r, c, "W")):
                    self.canvas.create_oval(coords, width=2, fill="white", outline="gray", tags="pieces")

    def drawMove(self, move):
        self.canvas.delete("move")
        tileSize = int(self.canvas["width"])/self.size
        startx = move[0]*tileSize+tileSize/2
        starty = move[1]*tileSize+tileSize/2
        endx = move[2]*tileSize+tileSize/2
        endy = move[3]*tileSize+tileSize/2
        self.canvas.create_line(startx, starty, endx, endy, fill="red", width=2, arrow=LAST, arrowshape=(15,18,10), tags="move")
        self.canvas.create_oval(startx-5,starty-5, startx+5, starty+5, fill="red", width=0, tags="move")

class MessageBox(object):
    def __init__(self,requestMessage):
        self.root = Tk()
        self.string = ""

        k = Label(self.root, text=requestMessage)
        k.pack(side=LEFT)
        e = Entry(self.root, text="")
        e.pack(side=LEFT)

        self.e = e

        b = Button(self.root, text="Confirm")
        b.bind("<Button-1>", self.finished)
        self.root.bind("<Return>", self.finished)
        b.pack(side=RIGHT)

        self.root.mainloop()

    def finished(self, event):
        self.string = self.e.get()
        self.root.quit()
        self.root.destroy()

    def getText(self):
        return self.string
