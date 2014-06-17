// PrimaryWindow.h
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
#ifndef PRIMARYWINDOW_H
#define PRIMARYWINDOW_H

// include the starter kit header - because we inherit from ASKWindow,
// we must #include the header
#include "AllegroStarterKit.h"

// whenever you write a program using the starter kit, you must create
// a primary window (you could name the class anything, but it needs to inherit
// from the ASKWindow class to be used.
class PrimaryWindow : public ASKWindow
{
private:

	// your game variables here
	bool playerShotAlive_;
    float playerShotX_;
    float playerShotY_;
    float playerShotDY_;
    int playerShotRadius_;
    int playerShotColor_;

	int playerX_;
    int playerY_;
    int playerWidth_;
    int playerHeight_;
    int playerColor_;
    int playerSpeed_;

	float ballX_;
    float ballY_;
    float ballDX_;
    float ballDY_;
    int ballRadius_;
    int ballColor_;

    bool ballAlive_;
    int ballRespawnTimeCounter_;
    int ballRespawnTimeDelay_;

	// your game methods here
	void UpdatePlayerShot();
    void RenderPlayerShot();

	void UpdatePlayer();
    void RenderPlayer();

	void UpdateBall();
    void RenderBall();

    bool LoadContentForBall();
    bool LoadContentForPlayer();
    bool LoadContentForPlayerShot();


public:
	// the class constructor
	PrimaryWindow();

	// the class destructor - make sure you specify that it is virtual
	virtual ~PrimaryWindow();

protected:

	// overload member function that will be called every time the main loop updates
	virtual bool UpdateFrame();

	// overload member function that will be called every time the main loop renders
	virtual void RenderFrame();

	// overloaded member function that will be called when the program starts
	virtual bool LoadContent();

	// overloaded member function that will be called when the program exits
	virtual void UnloadContent();
};

#endif
