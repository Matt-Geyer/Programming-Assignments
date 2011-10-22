#!/usr/bin/python

import pygame, sys
import random

from pygame.locals import *
from asteroid import *
from player import *
from missile import *


pygame.init()
fpsClock = pygame.time.Clock()

windowSurfaceObj = pygame.display.set_mode((800,800))
pygame.display.set_caption("Asteroids")

white = pygame.Color(255,255,255)
red = pygame.Color(255,0,0)
blue = pygame.Color(0,0,255)
fontObj = pygame.font.Font("freesansbold.ttf", 20)


p1 = player()

projectiles = []
roids = []
prects = []

roid_spawn = range(-100, 0 , 1 ) + range( 800 , 900 , 1 )

level = 1
score = 0
lives = 10
roid_count = 4 * level
last_shot = 0
gameOver = False
shotClock = pygame.time.Clock()
shotClock.tick()

boostClock = pygame.time.Clock()
boostClock.tick()


# Intro Screen
intro = pygame.image.load("./imgs/intro.png")
windowSurfaceObj.blit(intro,(0,0))
pygame.display.update()

while( intro ):
    pygame.event.pump()
    key = pygame.key.get_pressed()
    if(key[K_SPACE]): intro = False

while( gameOver == False):
    windowSurfaceObj.fill(white)

    # Handle user input
    pygame.event.pump()
    key = pygame.key.get_pressed()
    if(key[K_w]):
        p1.move(-.4)
    if(key[K_a]):
        p1.rotate(5)
    if(key[K_d]):
        p1.rotate(-5)
    if(key[K_e]):
        boostClock.tick()
        if(boostClock.get_time() > 100 ):
            p1.boost = -10
    if(key[K_s]):
        # space brakes
        if(p1.vel[0] > 0 ): p1.vel[0] -= .3
        if(p1.vel[0] <= 0 ): p1.vel[0] += .3
        if(p1.vel[1] > 0 ): p1.vel[1] -= .3
        if(p1.vel[1] <= 0 ): p1.vel[1] += .3 
        if( 0 - abs(p1.vel[0]) > -.5 ): p1.vel[0] = 0
        if( 0 - abs(p1.vel[1]) > -.5 ): p1.vel[1] = 0
    if(key[K_SPACE]):
        #fire missiles!!
        shotClock.tick()
        if( shotClock.get_time() > 100 ): 
            projectiles.append( missile(p1.getPos(),p1.rot))
            #shotClock.tick()
            
    if(key[K_ESCAPE]):
        pygame.quit()
        exit(1)
 
    # Update the player
    p1.update() 

    # update projectiles
    prects = []
    for proj in projectiles:
        proj.update()
        prects.append(proj.rect)
        if( proj.alive != True ):
            projectiles.remove( proj )
        windowSurfaceObj.blit( proj.img , proj.getPos())

    # Take care of asteroid logic..
    while( len(roids) < roid_count ):
        roids.append( asteroid(True , (random.sample(roid_spawn,1)[0] , random.sample(roid_spawn , 1 )[0] ) ) )

    for roid in roids:
        roid.update()

        #pygame.draw.rect( windowSurfaceObj , red , roid.rect , 2 )
        # check collisions
        for proj in projectiles:
            if( proj.rect.colliderect( roid.rect ) ): 
                roid.alive = False
                #if( projectiles.count(proj) > 1 ): 
                projectiles.remove(proj)

                # if we destroy a large roid.. create two small roids
                if( roid.large ):
                    for x in range(0,random.randrange(3 ,5 , 1)):
                        roids.append( asteroid( False , (roid.posx + random.randrange(30 , 50 , 5), roid.posy + random.randrange( 30 , 50 , 5)) ) )
                        
                    score += 2
                else:
                    score += 1
        if( roid.rect.colliderect( p1.rect )):
            lives -= 1
            roids = []
            p1.posx = 400
            p1.posy = 400

        # Only display if alive and in bounds..
        if( roid.alive == True and roid.inBounds == True ): windowSurfaceObj.blit( roid.img , roid.drawPos())
        # Remove from the list if it's been destroyed..
        if( roid.alive == False or roid.inBounds == False ):
            if(roids.count(roid) >= 1 ) : roids.remove( roid )


    # Update the player
    p1.update() 

    n_level = int(score / 100)
    if( n_level > level ):
        level = n_level
        roid_count += 2

    if( lives  <= 0 ):
        gameOver = True

    msg = "Level: " + str(level) + " Score: " + str(score) + " Lives: " + str(lives)
    msgSurface = fontObj.render(msg,False,red) 
    msgRect = msgSurface.get_rect()
    msgRect.topleft = ( 50 , 50 )
    windowSurfaceObj.blit( msgSurface , msgRect )
    #pygame.draw.rect( windowSurfaceObj , red , p1.rect , 2 )
    #pygame.draw.rect( windowSurfaceObj , blue , p1.drawPos(), 2 )
    windowSurfaceObj.blit( p1.img , p1.drawPos())
    
    pygame.display.update()
    fpsClock.tick(30)



# GAME OVER
while True:
    pygame.event.pump() 
    key = pygame.key.get_pressed()
    if(key[K_ESCAPE]):
        pygame.quit()
        exit(1)

    windowSurfaceObj.fill(red)

    fontOver = pygame.font.Font("freesansbold.ttf", 40)
    msg = "GAME OVER"
    msgSurface = fontOver.render(msg, False, white)
    msgRect = msgSurface.get_rect()
    msgRect.topleft = (250,390)
    windowSurfaceObj.blit( msgSurface, msgRect)
    
    msg = "Score: " + str(score)
    msgSurface = fontOver.render(msg,False,white)
    msgRect = msgSurface.get_rect()
    msgRect.topleft = (250 , 500)
    windowSurfaceObj.blit(msgSurface,msgRect)
    
    msg = "Press ESC to exit.."
    msgSurface = fontOver.render(msg,False,white)
    msgRect = msgSurface.get_rect()
    msgRect.topleft = (250 , 600)
    windowSurfaceObj.blit(msgSurface,msgRect)
    
    pygame.display.update()
    fpsClock.tick(30)

