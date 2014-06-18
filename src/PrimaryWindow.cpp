// PrimaryWindow.cpp
// Project: Allegro Game Template v9.0 (Allegro Starter Kit)
// Author: Richard Marks <ccpsceo@gmail.com>
/*
  ****************************************************************************
  * Copyright (c) 2010, Richard Marks, CCPS Solutions,                       *
  * Undefined Aeon Software.                                                 *
  *                                                                          *
  * Permission is hereby granted, free of charge, to any person obtaining a  *
  * copy of this software and associated documentation files (the            *
  * "Software"), to deal in the Software without restriction, including      *
  * without limitation the rights to use, copy, modify, merge, publish,      *
  * distribute, distribute with modifications, sub-license, and/or sell      *
  * copies of the Software, and to permit persons to whom the Software is    *
  * furnished to do so, subject to the following conditions:                 *
  *                                                                          *
  * The above copyright notice and this permission notice shall be included  *
  * in all copies or substantial portions of the Software.                   *
  *                                                                          *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
  * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  *
  * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
  * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
  * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
  * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
  *                                                                          *
  * Except as contained in this notice, the name(s) of the above copyright   *
  * holders shall not be used in advertising or otherwise to promote the     *
  * sale, use or other dealings in this Software without prior written       *
  * authorization.                                                           *
  ****************************************************************************
*/

// include the complementing header
#include "PrimaryWindow.h"
#include <cmath>
#include <cstdio>

PrimaryWindow::PrimaryWindow()
{
}

PrimaryWindow::~PrimaryWindow()
{
}

bool PrimaryWindow::UpdateFrame()
{
	// update stuff here
	this->UpdateBall();
	this->UpdateBallShot();
	this->UpdatePlayer();
	this->UpdatePlayerShot();

	return ASKWindow::UpdateFrame();
}

void PrimaryWindow::RenderFrame()
{
	clear_bitmap(_backBuffer);

	// draw here
	this->RenderBall();
	this->RenderBallShot();
	this->RenderPlayer();
	this->RenderPlayerShot();
	this->RenderHeadsUpDisplay();


	ASKWindow::RenderFrame();
}

bool PrimaryWindow::LoadContent()
{
    if (!this->LoadContentForBall())
	{
		return false;
	}

	if (!this->LoadContentForBallShot())
	{
		return false;
	}

	if (!this->LoadContentForPlayer())
	{
		return false;
	}

	if (!this->LoadContentForPlayerShot())
	{
		return false;
	}

	return ASKWindow::LoadContent();
}

void PrimaryWindow::UnloadContent()
{
	// unload your content here

	ASKWindow::UnloadContent();
}

void PrimaryWindow::UpdateBall()
{
	// NEW - ball only updates when its alive

	// if the ball is not alive
	if (!ballAlive_)
	{
		// increment the respawn counter
		ballRespawnTimeCounter_++;

		// if the respawn counter has reached the respawn delay
		if (ballRespawnTimeCounter_ == ballRespawnTimeDelay_)
		{
			// reset the respawn counter
			ballRespawnTimeCounter_ = 0;

			// respawn the ball:
			// * choose a random trajectory
			ballDX_ = (float)4+(rand()%8);
			ballDY_ = (float)4+(rand()%8);

			// * choose a new random position in the top half of the screen
			ballX_ = (float)(ballRadius_ + rand() % (SCREEN_W - ballRadius_));
			ballY_ = (float)(ballRadius_ + rand() % ((SCREEN_H / 2) - ballRadius_));

			// * set the ball to be alive
			ballAlive_ = true;
		}

		// exit the function
		return;
	}

	ballX_ += ballDX_;
	ballY_ += ballDY_;

	if (ballX_ < ballRadius_ || ballX_ > (SCREEN_W - ballRadius_))
	{
		ballDX_ = ballDX_ * -1.0f;
		ballX_ += ballDX_;
	}

	if (ballY_ < ballRadius_ || ballY_ > ((SCREEN_H - 18) - ballRadius_))
	{
		ballDY_ = ballDY_ * -1.0f;
		ballY_ += ballDY_;
	}

    // is the player even alive?
    if (!playerAlive_)
    {
        // exit the function
        return;
    }

	// can fire only if the ball shot is dead
    if (ballShotAlive_)
    {
        // exit the function
        return;
    }

    // randomly attack the player
    if ((rand() % 8) != 0)
    {
        // exit the function
        return;
    }

    // if the ball is not traveling downwards,
    if (ballDY_ <= 0.0f)
    {
        // exit the function
        return;
    }

    // if the ball has passed the Y centerline
    if (ballY_ > SCREEN_H / 2)
    {
        // exit the function
        return;
    }

    // calculate the trajectory to the player
    float dx = playerX_ - ballX_;
    float dy = playerY_ - ballY_;

    float length = sqrt((dx * dx) + (dy * dy));

    dx /= length;
    dy /= length;

    ballShotDX_ = dx * ballShotSpeed_;
    ballShotDY_ = dy * ballShotSpeed_;

    ballShotAlive_ = true;
}

void PrimaryWindow::RenderBall()
{
    if (!ballAlive_)
    {
        return;
    }

	circlefill(_backBuffer, (int)ballX_, (int)ballY_, ballRadius_, ballColor_);
}

void PrimaryWindow::UpdatePlayer()
{
    // if the player is dead
    if (!playerAlive_)
    {
        // if the player has no lives remaining
        if (!playerLives_)
        {
            // end the program for now.
            // We will add game over screens and menus in later tutorials.
            this->Close();

            // exit the function
            return;
        }

        // increment the respawn counter
        playerRespawnTimeCounter_++;

        // if the respawn counter has reached the respawn delay
        if (playerRespawnTimeCounter_ == playerRespawnTimeDelay_)
        {
            // reset the respawn counter
            playerRespawnTimeCounter_ = 0;

            // respawn the player:
            playerX_ = SCREEN_W / 2 - playerWidth_ / 2;
            playerY_ = (SCREEN_H - 20) - playerHeight_;

            // set the player to be alive
            playerAlive_ = true;
        }

        // exit the function
        return;
    }

	// if the left arrow key is down
	if (key[KEY_LEFT])
	{
		// move left
		playerX_ -= playerSpeed_;
	}
	// if the right arrow key is down
	else if (key[KEY_RIGHT])
	{
		// move right
		playerX_ += playerSpeed_;
	}

	// NEW
    // the player is limited to firing a single shot at a time.
    // if the space bar is pressed and the shot is currently dead
    if (key[KEY_SPACE] && (!playerShotAlive_))
    {
        // fire a shot by making it alive
        playerShotAlive_ = true;
    }
    //

	// ensure that the player hasn't gone off screen
	int xMax = SCREEN_W - playerWidth_;
	if (playerX_ < 0)
	{
		playerX_ = 0;
	}
	else if (playerX_ > xMax)
	{
		playerX_ = xMax;
	}
}

void PrimaryWindow::RenderPlayer()
{
	// we draw the player as a filled triangle

	/*
		The player is defined by a position and a size:
		x,y,w,h

		The triangle's 3 points are

		x,y
		o----2----o
		|   / \   |
		|  /   \  |
		| /     \ |
		|/       \|
		1---------3x+w,y+h

		So, point 1 is at x, y+h
		point 2 is at x+(w/2),y and
		point 3 is at x+w,y+h
	*/

	// we calculate the bottom once and save the value
	// because it will be used twice. a tiny optimization
	// that reduces the number of calculations needed.
	int playerBottom = playerY_ + playerHeight_;

	int vx[] = { playerX_, playerX_ + (playerWidth_ / 2), playerX_ + playerWidth_ };
    int vy[] = { playerBottom, playerY_, playerBottom };

    if (!playerAlive_)
    {
        // wireframe display flashes
        if ((playerRespawnTimeCounter_ % 2) == 0)
        {
            line(_backBuffer, vx[0], vy[0], vx[1], vy[1], playerColor_);
            line(_backBuffer, vx[1], vy[1], vx[2], vy[2], playerColor_);
            line(_backBuffer, vx[2], vy[2], vx[0], vy[0], playerColor_);
        }

        // exit the function
        return;
    }

    // we draw the player as a filled triangle
    triangle(_backBuffer, vx[0], vy[0], vx[1], vy[1], vx[2], vy[2], playerColor_);
}

void PrimaryWindow::UpdatePlayerShot()
{
	// if the shot is not alive, then we move it to match
	// the player's X center position
	if (!playerShotAlive_)
	{
		playerShotX_ = playerX_ + (playerWidth_ / 2);
	}
	else
	// if the shot is alive, we move it along the Y axis
	{
		playerShotY_ -= playerShotDY_;

		// if the shot moves off the top of the screen
		if (playerShotY_ <  -playerShotRadius_)
		{
			// kill the shot
			playerShotAlive_ = false;

			// reset its position to the top of the player
			playerShotX_ = playerX_ + (playerWidth_ / 2);
			playerShotY_ = playerY_ - playerShotRadius_;

			// exit the function because we don't test for
			// collisions if its dead.
			return;
		}

		// if the shot collides with the ball:
		/*
			To test the collision of the ball and the shot,
			a simple rectangle / rectangle intersection is
			tested. If there is an intersection, there is a
			collision.

			I chose to use rectangle / rectangle intersection
			instead of circle/circle intersection because
			it is much faster, and the perfect accuracy is not
			needed for a simple demonstration.

			This is a collision:

			      E        F
			      o--------o
			A     |  B     |
			o-----|--o     |
			|     |  |     |
			|     o--|-----o
			|     G  |     H
			o--------o
			C        D

			Shot Object Rectangle:

			A: sx, sy
			B: sx + (sr * 2), sy
			C: sx, sy + (sr * 2)
			D: sx + (sr * 2), sy + (sr * 2)

			Ball Object Rectangle:

			E: bx, by
			F: bx + (br * 2), by
			G: bx, by + (br * 2)
			H: bx + (br * 2), by + (br * 2)
		*/

		// calculate the diameter of the ball and shot
		// just once since we use this dimension multiple times
		int ballDiameter = ballRadius_ * 2;
		int shotDiameter = playerShotRadius_ * 2;

		// use integers for testing to be simpler and faster
		// the x/y coordinates for these objects are the centers.
		// we need to calculate the upper-left corned of the bounding
		// rectangle, so we just subtract the radius from the center.
		int sx = (int)playerShotX_ - playerShotRadius_;
		int sy = (int)playerShotY_ - playerShotRadius_;
		int bx = (int)ballX_ - ballRadius_;
		int by = (int)ballY_ - ballRadius_;

		// if there is a collision of the shot and the ball
		if (
			sx + shotDiameter > bx &&
			sx < bx + ballDiameter &&
			sy + shotDiameter > by &&
			sy < by + ballDiameter
			)
		{
			// kill the shot
			playerShotAlive_ = false;

			// reset its position to the top of the player
			playerShotX_ = playerX_ + (playerWidth_ / 2);
			playerShotY_ = playerY_ - playerShotRadius_;

			// kill the ball
			ballAlive_ = false;

			// increase the player's score
            playerScore_ += 25;
		}
	}
}

void PrimaryWindow::RenderPlayerShot()
{
	// exit the function if the shot is dead
	if (!playerShotAlive_)
	{
		return;
	}

	// draw the shot
	circlefill(_backBuffer,
		(int)playerShotX_, (int)playerShotY_,
		playerShotRadius_, playerShotColor_);
}

bool PrimaryWindow::LoadContentForBall()
{
    // load your content here
	// a blue ball
    ballColor_ = makecol(0, 0, 255);

    // the ball has a radius of 8 pixels
    ballRadius_ = 8;

    // the ball starts in the center of the screen
    ballX_ = SCREEN_W / 2;
    ballY_ = SCREEN_H / 2;

    // the ball starts on a random trajectory
    ballDX_ = (float)4+(rand()%8);
    ballDY_ = (float)4+(rand()%8);

    // the ball starts off alive by default
    ballAlive_ = true;

    // a dead ball will respawn in roughly 1 second (30 FPS)
    ballRespawnTimeDelay_ = 30;
    ballRespawnTimeCounter_ = 0;

	return true;
}

bool PrimaryWindow::LoadContentForPlayer()
{
    // the player is 32 pixels wide by 64 pixels tall
    playerWidth_ = 32;
    playerHeight_ = 64;

    // the player is at the bottom center of the screen
    playerX_ = SCREEN_W / 2 - playerWidth_ / 2;
    playerY_ = (SCREEN_H - 20) - playerHeight_;

    // the player is green
    playerColor_ = makecol(0, 255, 0);

    // the player moves at 1/3 of its width
    playerSpeed_ = playerWidth_ / 3;

    // the player starts the game in a dead state
    // this gives the player a few seconds before the action begins.
    playerAlive_ = false;

    // the player will start with 5 lives
    startingPlayerLives_ = 5;

    // set the player with the right number of lives
    playerLives_ = startingPlayerLives_;

    // the player will take roughly 3 seconds to re-spawn
    playerRespawnTimeDelay_ = 90;
    playerRespawnTimeCounter_ = 0;

    // player starts with a score of zero
    playerScore_ = 0;

	return true;
}

bool PrimaryWindow::LoadContentForPlayerShot()
{
    // the shot is dead by default
    playerShotAlive_ = false;

    // the player's shots have a 4 pixel radius
    playerShotRadius_ = 4;

    // the player fires cyan shots
    playerShotColor_ = makecol(0, 255, 255);

    // position the shot at the top-center of the player's ship
    playerShotX_ = playerX_ + (playerWidth_ / 2);

    // reset its Y position to the top of the player
    playerShotY_ = (float)(playerY_ - playerShotRadius_);

    // shot velocity is set once
    // all shots are fired at 20 pixels per frame
    playerShotDY_ = 20.0f;

	return true;
}

void PrimaryWindow::UpdateBallShot()
{
    // if the shot is not alive, move it's position to the ball
    if (!ballShotAlive_)
    {
        ballShotX_ = ballX_;
        ballShotY_ = ballY_;
        return;
    }

    // toggle the display
    ballShotShow_ = !ballShotShow_;

    #if 0
	// 10% chance to track player for lock-on missile behavior
	if (rand() % 100 < 10)
    {
        // calculate the trajectory to the player
        float dx = playerX_ - ballShotX_;
        float dy = playerY_ - ballShotY_;

        float length = sqrt((dx * dx) + (dy * dy));

        dx /= length;
        dy /= length;

        ballShotDX_ = dx * ballShotSpeed_;
        ballShotDY_ = dy * ballShotSpeed_;
    }
    #endif

    // apply velocity
    ballShotX_ += ballShotDX_;
    ballShotY_ += ballShotDY_;

    // if the shot goes off screen
    if (ballShotX_ < 0 || ballShotY_ > SCREEN_H || ballShotX_ > SCREEN_W)
    {
        // kill the shot
        ballShotAlive_ = false;

        // reset the position to the ball
        ballShotX_ = ballX_;
        ballShotY_ = ballY_;

        // exit the function
        return;
    }

    // handle collision against the player
    /*
        We are going to use a point/triangle intersection test this time.

        Warning: The following code is convoluted and difficult to understand.
        I didn't write the algorithm. I found it somewhere a long time ago.
        I do not remember the source, so I credit it as (C) Original Author.

        Just use it, and know that if the inside boolean is true after the code
        executes, that there has been a collision.

        The input variables are: x,y, vx1, vy1, vx2, vy2, vx3, vy3

        x - the x coordinate of the point to test
        y - the y coordinate of the point to test
        vxN - the x coordinate for the vertice N of the triangle
        vyN - the y coordinate for the vertice N of the triangle
    */

    // the player's triangle
    int playerBottom = playerY_ + playerHeight_;
    float vx1 = (float)playerX_;
    float vy1 = (float)playerBottom;
    float vx2 = (float)(playerX_ + (playerWidth_ / 2));
    float vy2 = (float)playerY_;
    float vx3 = (float)(playerX_ + playerWidth_);
    float vy3 = vy1;

    // the point is the shot's center (its position)
    float& x = ballShotX_;
    float& y = ballShotY_;

    // test if the point is within the triangle
    bool inside = false;
    if((((vy1<=y)&&(y<vy3))||((vy3<=y)&&(y<vy1)))&&(x<(vx3-vx1)*(y-vy1)/(vy3-vy1)+vx1)){inside=!inside;}
    if((((vy2<=y)&&(y<vy1))||((vy1<=y)&&(y<vy2)))&&(x<(vx1-vx2)*(y-vy2)/(vy1-vy2)+vx2)){inside=!inside;}
    if((((vy3<=y)&&(y<vy2))||((vy2<=y)&&(y<vy3)))&&(x<(vx2-vx3)*(y-vy3)/(vy2-vy3)+vx3)){inside=!inside;}
    if (inside)
    {
        // kill the shot
        ballShotAlive_ = false;

        // reset the position to the ball
        ballShotX_ = ballX_;
        ballShotY_ = ballY_;

        // kill the player! Oh Noooo
        playerAlive_ = false;

        // decrease the number of lives the player has
        playerLives_--;
    }
}

void PrimaryWindow::RenderBallShot()
{
    // exit the function if the shot is dead
    if (!ballShotAlive_)
    {
        return;
    }

    // draw the shot
    if (!ballShotShow_)
    {
        circlefill(_backBuffer,
        (int)ballShotX_, (int)ballShotY_,
        ballShotRadius_, ballShotColor_);
    }
    else
    {
        circlefill(_backBuffer,
        (int)ballShotX_, (int)ballShotY_,
        ballShotRadius2_, ballShotColor_);
    }
}

bool PrimaryWindow::LoadContentForBallShot()
{
    // the shot is dead by default
    ballShotAlive_ = false;

    // the ball's shots will alternate between 2 sizes
    ballShotRadius_ = 4;
    ballShotRadius2_ = 5;

    // the ball fires a red to yellow shot
    ballShotColor_ = makecol(255, 0, 0);
    ballShotColor2_ = makecol(255, 255, 0);

    // position the shot at the center of the ball
    ballShotX_ = ballX_;
    ballShotY_ = ballY_;

    // ball shots move at 15 pixels per frame
    ballShotSpeed_ = 15.0f;

    // the shot has 2 displays, and we use a
    // boolean to decide which display to render
    ballShotShow_ = true;

    // shots trajectory will be calculated when firing
    ballShotDX_ = 0.0f;
    ballShotDY_ = 0.0f;

    return true;
}

void PrimaryWindow::RenderHeadsUpDisplay()
{
    const int hudBackColor = makecol(32, 32, 32);
    rectfill(_backBuffer, 0, SCREEN_H - 18, SCREEN_W, SCREEN_H, hudBackColor);
    this->RenderPlayerLivesIndicator();
    this->RenderScoreIndicator();
}

void PrimaryWindow::RenderPlayerLivesIndicator()
{
    int xOffset = 8;
    int yOffset = SCREEN_H - 18;
    int iconWidth = 8;
    int iconWidthOver2 = iconWidth / 2;
    int iconHeight = 16;
    int iconSpacing = 4;
    int iconBottom = yOffset + iconHeight;

    for (int i = 0; i < playerLives_; i++)
    {
        int x = xOffset + ((i * iconWidth) + (i * iconSpacing));
        triangle(_backBuffer,
            x, iconBottom,
            x + iconWidthOver2, yOffset,
            x + iconWidth, iconBottom,
            playerColor_);
    }
}

void PrimaryWindow::RenderScoreIndicator()
{
    const int fontWidth = 8;
    const int fontHeight = 14;
    const int fontSpacing = 2;
    const int letterWidth = fontWidth + fontSpacing;
    const int numberOfDigitsToDisplay = 8;
    const int digitBufferSize = numberOfDigitsToDisplay + 1;
    const int indicatorWidth = letterWidth * numberOfDigitsToDisplay;
    const int xOffset = (SCREEN_W / 2) - (indicatorWidth / 2);
    const int yOffset = (SCREEN_H - 18) + (9 - (fontHeight / 2));
    const int scoreColor = makecol(255, 255, 255);

    char scoreDigitBuffer[digitBufferSize];
    snprintf(scoreDigitBuffer, digitBufferSize, "%08d", playerScore_);

    for (int i = 0, xPos = xOffset; i < numberOfDigitsToDisplay; i++)
    {
        int digit = (int)scoreDigitBuffer[i];
        this->RenderDigit(digit - 48, xPos, yOffset, fontWidth, fontHeight, scoreColor);
        xPos += letterWidth;
    }
}

void PrimaryWindow::RenderDigit(int digit, int x, int y, int width, int height, int color) const
{
    // A simple 7-segment digital digit rendering routine
    // (C) Copyright 2010, Richard Marks
    // Use in your own programs requires permission.
    const int x2 = x + width;
    const int y2 = y + (height / 2);
    const int y3 = y + height;
    const int vx[] = {x, x2, x2, x2,  x, x};
    const int vy[] = {y,  y, y2, y3, y3, y2};
    const int invalidDigit = 0xB;
    const unsigned char pattern[] =
    {
        0x7E, 0x30, 0x6D, 0x79, 0x33,
        0x5B, 0x5F, 0x70, 0x7F, 0x73,
        0x00
    };

    if (digit < 0 || digit > 9)
    {
        digit = invalidDigit;
    }
    if (pattern[digit] & 0x40) { line(_backBuffer, vx[0], vy[0], vx[1], vy[1], color); }
    if (pattern[digit] & 0x20) { line(_backBuffer, vx[1], vy[1], vx[2], vy[2], color); }
    if (pattern[digit] & 0x10) { line(_backBuffer, vx[2], vy[2], vx[3], vy[3], color); }
    if (pattern[digit] & 0x08) { line(_backBuffer, vx[3], vy[3], vx[4], vy[4], color); }
    if (pattern[digit] & 0x04) { line(_backBuffer, vx[4], vy[4], vx[5], vy[5], color); }
    if (pattern[digit] & 0x02) { line(_backBuffer, vx[5], vy[5], vx[0], vy[0], color); }
    if (pattern[digit] & 0x01) { line(_backBuffer, vx[5], vy[5], vx[2], vy[2], color); }
}
