#!/usr/bin/python

import pygame, math
pygame.init()

class player():

    def __init__(self):
        self.img = pygame.image.load("./imgs/ship.png")
        self.CIMG = self.img
        self.posx = 400
        self.posy = 400
        self.alive = True
        self.rot = 0
        self.acel = [0,0]
        self.vel = [0,0]
        self.rect = self.img.get_rect()
        self.rect.inflate_ip( -20 , -20 )
        self.rect.x = self.posx
        self.rect.y = self.posy
        self.boost = 0
        
        self.iw = self.CIMG.get_rect().right
        self.ih = self.CIMG.get_rect().bottom
        

    def getPos(self):
        return (self.posx,self.posy)

    def update(self):

        if( self.boost <  0 ): self.boost += .35
        if( self.vel[0] > 5 ): self.vel[0] = 5
        if( self.vel[1] > 5 ): self.vel[1] = 5
        if( self.vel[0] < -5 ): self.vel[0] = -5
        if( self.vel[1] < -5 ): self.vel[1] = -5    
       
        ang = self.rot * math.pi / 180
        self.posx += self.vel[0] 
        self.posy += self.vel[1] 
        self.posx += self.boost * math.sin(ang)
        self.posy += self.boost * math.cos(ang)
        
        #self.rect.x = self.posx
        #self.rect.y = self.posy
        self.rect.center = self.drawPos().center

        if( self.posx >= 820 ): self.posx = 5
        if( self.posx <= -20 ): self.posx = 795
        if( self.posy >= 820 ): self.posy = 5
        if( self.posy <= -20 ): self.posy = 795

    def rotate( self, ang ):
        self.rot += ang
        self.img = pygame.transform.rotate( self.CIMG , self.rot )
        #self.draw_pos = self.img.get_rect().move(
        #    self.posx - self.iw / 2,
        #    self.posy - self.ih / 2)
        #oldcenter = self.rect.center      
        #self.img = pygame.transform.rotate( self.CIMG , self.rot )
        #self.rect = self.img.get_rect()
        #self.rect.center = oldcenter

    def drawPos(self):
        self.draw_pos = self.img.get_rect().move(
            self.posx - 50,
            self.posy - 50)
        return self.draw_pos

    def move(self , speed):
        ang = self.rot * math.pi / 180
        self.vel[0] += speed * math.sin(ang)
        self.vel[1] += speed * math.cos(ang)
