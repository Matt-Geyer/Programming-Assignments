#!/usr/bin/python

import pygame
import math
pygame.init()

class missile():
    def __init__(self,pos,angle):
        self.angle = angle * math.pi / 180 
        self.posx = pos[0]# + ( 1 * math.sin(self.angle))
        self.posy = pos[1]# + ( 1 * -math.cos(self.angle))
        self.speed = 15
        self.velx = self.speed * -math.sin(self.angle)
        self.vely = self.speed * -math.cos(self.angle)
        self.img = pygame.image.load("./imgs/missile.png").convert()
        self.img = pygame.transform.rotate(self.img, angle)
        self.alive = True
        self.rect = self.img.get_rect()
        self.rect.x = self.posx
        self.rect.y = self.posy

    def update(self):
        self.posx += self.velx
        self.posy += self.vely
        self.rect.x = self.posx
        self.rect.y = self.posy

        if(self.posx > 850 or self.posx < -20 ): self.alive = False
        if(self.posy > 850 or self.posx < -20 ): self.alive = False

    def getPos(self):
        return (self.posx, self.posy)
