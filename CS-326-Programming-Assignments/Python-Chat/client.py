#!/usr/bin/python

import xmlrpclib
import sys
import curses
import curses.textpad
from time import localtime


port = 6789

server = xmlrpclib.Server("http://localhost:6789")


def getTime():
    t = localtime()
    timestamp = ""
    timestamp += str(t.tm_mon) + "." + str(t.tm_mday) + "." + str(t.tm_year) + "-"
    timestamp += str(t.tm_hour) + ":" + str(t.tm_min) + ":" + str(t.tm_sec) 
    return timestamp


def update():

    xy = screen.getmaxyx()

    # Print the menu to the screen
    screen.addstr(2,3,"Matt's Chat")
    screen.addstr(3,3,"Press m to type a message")
    screen.addstr(5,3,"Press any other key to refresh messages..")
    screen.addstr(4,3,"Press ESC to exit")
    screen.addstr(2 , 16 , getTime())
    # Print the latest messages
    msgLog = server.get()

    line = xy[0] - 4
    while((line > 8) and len(msgLog) > 0):
        message = msgLog.pop()
        screen.addstr(line,3,message[2] + ":: " + message[0] + ": " + message[1])
        line -= 1

    if( typing == True ):
        screen.addstr(5,3,"Message input in progress.. ESC to cancel, ENTER to send.")
        screen.addstr(xy[0] - 2 , 3 , msg )

def getUser():
    # Get the desired username
    username = ""
    while True:
        screen.clear()
        screen.addstr(2,3,"Enter your username..")
        screen.addstr(3,3,username)
        key = screen.getch()
        
        # Check to see if valid key..
        if( key >= 32 and key <= 126): username += chr(key)

        # ESC Key pressed
        if( key == 27 ): 
            running = False
            return ""
            

        # Backspace
        if( key == 127 ):
            # Make sure in bounds before deleting
            if( len(username) > 0 ):
                username = username[:-1]
 
        #Enter
        if( key == 10 ):
            return username

 

screen = curses.initscr()
typing = False
running = True
key = 0
global msg

username = getUser()


while( running == True ):

    screen.clear()
    screen.refresh()
    screen.border(0)
     
    update()
  
   
    key = screen.getch()
 
    # Handle typing.. 
    if(typing == True):
 
        # Check to see if valid key..
        if( key >= 32 and key <= 126): msg += chr(key)

        # ESC Key pressed
        if( key == 27 ): 
            typing = False
            msg = ""

        # Backspace
        if( key == 127 ):
            # Make sure in bounds before deleting
            if( len(msg) > 0 ):
                msg = msg[:-1]
 
        #Enter
        if( key == 10 ):
            typing = False
            #Add msg to the server queue
            server.send(username,msg,getTime())
            msg = ""

    if( key == ord('m') and typing == False ):
        typing = True
        msg = ""

    if( key == 27 and typing == False ):
        running = False

    
curses.endwin()




