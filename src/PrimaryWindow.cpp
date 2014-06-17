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
	this->UpdatePlayer();
	this->UpdatePlayerShot();

	return ASKWindow::UpdateFrame();
}

void PrimaryWindow::RenderFrame()
{
	clear_bitmap(_backBuffer);

	// draw here
	this->RenderBall();
	this->RenderPlayer();
	this->RenderPlayerShot();

	ASKWindow::RenderFrame();
}

bool PrimaryWindow::LoadContent()
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

    // the player is 32 pixels wide by 64 pixels tall
    playerWidth_ = 32;
    playerHeight_ = 64;

    // the player is at the bottom center of the screen
    playerX_ = SCREEN_W / 2 - playerWidth_ / 2;
    playerY_ = SCREEN_H - playerHeight_;

    // the player is green
    playerColor_ = makecol(0, 255, 0);

    // the player moves at 1/3 of its width
    playerSpeed_ = playerWidth_ / 3;

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

	return ASKWindow::LoadContent();
}

void PrimaryWindow::UnloadContent()
{
	// unload your content here

	ASKWindow::UnloadContent();
}

void PrimaryWindow::UpdateBall()
{
	ballX_ += ballDX_;
	ballY_ += ballDY_;

	if (ballX_ < ballRadius_ || ballX_ > (SCREEN_W - ballRadius_))
	{
		ballDX_ = ballDX_ * -1.0f;
		ballX_ += ballDX_;
	}

	if (ballY_ < ballRadius_ || ballY_ > (SCREEN_H - ballRadius_))
	{
		ballDY_ = ballDY_ * -1.0f;
		ballY_ += ballDY_;
	}
}

void PrimaryWindow::RenderBall()
{
	circlefill(_backBuffer, (int)ballX_, (int)ballY_, ballRadius_, ballColor_);
}

void PrimaryWindow::UpdatePlayer()
{
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

	triangle(_backBuffer,
		playerX_, playerBottom,
		playerX_ + (playerWidth_ / 2), playerY_,
		playerX_ + playerWidth_, playerBottom, playerColor_);
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
