#!/usr/bin/python
import pygame
import random

pygame.init()

# List of possible asteroid images..
l_roids = [ "la1.png" , "la2.png" , "la3.png" , "la4.png",
            "la5.png" , "la6.png" , "la7.png" , "la8.png" ]

s_roids = [ "sa1.png" , "sa2.png" , "sa3.png" ,
            "sa4.png" , "sa5.png" , "sa6.png" ]

class asteroid():
    """The asteroid.. main protagonist in the game!"""
    def __init__(self, lg , pos ):
        if( lg == True ):
            self.img = pygame.image.load("./imgs/" + random.sample(l_roids,1)[0]).convert()
        else:
            self.img = pygame.image.load("./imgs/" + random.sample(s_roids,1)[0]).convert()
        self.large = lg
        self.rect = self.img.get_rect()
        self.iw = self.rect.right
        self.ih = self.rect.bottom
        self.rect.inflate_ip( -30 , -30 )
        self.CIMG = self.img
        self.posx = pos[0]
        self.posy = pos[1]
        self.rect.move_ip( self.posx , self.posy )
        # Try to make the roids go towards the center
        self.vel = [random.randrange(1,5,1),random.randrange(1,5,1)]
        if( self.posx > 800 ):
            self.vel[0] *= -1
        if( self.posy > 800 ): 
            self.vel[1] *= -1

        self.ang = random.randrange(1,4,1)
        self.rot = 0
        self.alive = True
        self.inBounds = True

    def drawPos(self):
        draw_pos = self.img.get_rect().move(
            self.posx - self.iw / 2,
            self.posy - self.ih / 2)
        return draw_pos    

    def update(self):
        # Update position of the roid
        self.posx += self.vel[0]
        self.posy += self.vel[1]
        self.rect.center = self.drawPos().center
        
        # Check to see if the asteroid is still in bounds..
        if( self.posx > 910 or self.posx < -110 ):
            self.inBounds = False
        if( self.posy > 910 or self.posy < -110 ):
            self.inBounds = False
        
        # Rotate the asteroids - WIP
        #self.rot += self.ang
        #self.rect.x = self.posx
        #self.rect.y = self.posy
        #orig_rect = self.img.get_rect()
        #self.img = pygame.transform.rotate( self.CIMG , self.rot )
        #rot_rect = orig_rect.copy()
        #self.posx += orig_rect.centerx - rot_rect.width/2
        #self.posy += orig_rect.centery - rot_rect.height/2
    
    def getPos(self):
        return (self.posx , self.posy)

    def setPos(pos):
        self.posx = pos[0]
        self.posy = pos[1]
