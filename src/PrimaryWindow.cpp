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

	return ASKWindow::UpdateFrame();
}

void PrimaryWindow::RenderFrame()
{
	clear_bitmap(_backBuffer);

	// draw here
	this->RenderBall();

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
