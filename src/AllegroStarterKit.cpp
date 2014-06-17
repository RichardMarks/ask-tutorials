// AllegroStarterKit.cpp
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

#include "AllegroStarterKit.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

static volatile int timerTickCount = 0;
static volatile bool mainThreadIsRunning = true;

static void OnTimerTickEvent()
{
	timerTickCount++;
}
END_OF_FUNCTION(OnTimerTickEvent)

static void OnCloseButtonClickEvent()
{
	mainThreadIsRunning = false;
}
END_OF_FUNCTION(OnCloseButtonClickEvent)


struct StartupConfiguration
{
	int windowWidth;
	int windowHeight;
	int windowBpp;
	bool useFullScreen;
	std::string windowCaption;

	enum RunState { Load, Save };

	StartupConfiguration(){this->UseDefaults();}
	StartupConfiguration(const char* fileName, RunState state)
	{
		this->UseDefaults();

		if (state == StartupConfiguration::Load)
		{
			this->LoadFromFile(fileName);
		}
		else if (state == StartupConfiguration::Save)
		{
			this->SaveToFile(fileName);
		}
	}

	void Set(int width, int height, int bpp, bool fullScreen, std::string caption)
	{
		windowWidth = width;
		windowHeight = height;
		windowBpp = bpp;
		useFullScreen = fullScreen;
		windowCaption = caption;
	}

	void LoadFromFile(const char* fileName)
	{
		std::ifstream stream;
		stream.open(fileName);
		if (stream.is_open())
		{
			while(!stream.eof())
			{
				std::string line;
				std::getline(stream, line);

				if (this->StringStartsWith(line, "Geometry"))
				{
					this->ParseWindowGeometry(line);
				}
				else if (this->StringStartsWith(line, "FullScreen"))
				{
					this->ParseFullScreen(line);
				}
				else if (this->StringStartsWith(line, "Caption"))
				{
					this->ParseCaption(line);
				}
				else
				{
					if (line.size())
					{
						//std::cout << "Unknown CFG Parameter: " << line << "\n";
					}
				}
			}
			stream.close();
		}
	}

	bool StringStartsWith(std::string& str, std::string pattern)
	{
		if (str.size() < pattern.size())
		{
			return false;
		}
		int length = (int)pattern.size();
		for (int i = 0; i < length; i++)
		{
			if (str[i] != pattern[i])
			{
				return false;
			}
		}
		return true;
	}

	void SaveToFile(const char* fileName)
	{
		std::ofstream stream;
		stream.open(fileName);
		if (stream.is_open())
		{
			stream << "Geometry=" << windowWidth << "x" << windowHeight << "@" << windowBpp << "\n";
			stream << "FullScreen=" << ((useFullScreen) ?"true":"false") << "\n";
			stream << "Caption=" << windowCaption << "\n";
			stream.close();
		}
	}

	void Display()
	{
		std::cout <<
			"Window Geometry = " << windowWidth << "x" << windowHeight << "@" << windowBpp << "\n" <<
			"Startup FullScreen ? " << ((useFullScreen) ?"true":"false") << "\n" <<
			"Window Caption = \"" << windowCaption << "\"\n";
	}

	void UseDefaults()
	{
		windowWidth = 800;
		windowHeight = 600;
		windowBpp = 24;
		useFullScreen = false;
		windowCaption = "This is an untitled project";
	}

private:


	void ParseWindowGeometry(std::string& geometry)
	{
		std::vector<std::string> tokens = TokenizeString(geometry, "=x@ \t");
		windowWidth = atoi(tokens.at(1).c_str());
		windowHeight = atoi(tokens.at(2).c_str());
		windowBpp = atoi(tokens.at(3).c_str());
	}

	void ParseFullScreen(std::string& fs)
	{
		std::vector<std::string> tokens = TokenizeString(fs, "= \t");
		useFullScreen = (tokens.at(1) == "true") ? true : false;
	}

	void ParseCaption(std::string& cap)
	{
		std::vector<std::string> tokens = TokenizeString(cap, "=");
		windowCaption = tokens.at(1);
	}

	std::vector<std::string> TokenizeString(const std::string& source, const std::string& delimiters)
	{
		std::vector<std::string> tokens;
		size_t p0 = 0;
		size_t p1 = std::string::npos;
		while (p0 != std::string::npos)
		{
			p1 = source.find_first_of(delimiters, p0);
			if (p1 != p0)
			{
				std::string token = source.substr(p0, p1 - p0);
				tokens.push_back(token);
			}
			p0 = source.find_first_not_of(delimiters, p1);
		}
		return tokens;
	}
};


ASKWindow::ASKWindow()
{
	StartupConfiguration cfg("startup.cfg", StartupConfiguration::Load);

	allegro_init();
	install_timer();
	install_keyboard();
	install_mouse();
	set_color_depth(cfg.windowBpp);
	set_gfx_mode(
		(cfg.useFullScreen) ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
		cfg.windowWidth, cfg.windowHeight, 0, 0);
	set_window_title(cfg.windowCaption.c_str());
	_backBuffer = create_bitmap(SCREEN_W, SCREEN_H);
	LOCK_FUNCTION(OnCloseButtonClickEvent);
	LOCK_FUNCTION(OnTimerTickEvent);
	LOCK_VARIABLE(timerTickCount);
	set_close_button_callback(OnCloseButtonClickEvent);
	install_int_ex(OnTimerTickEvent, BPS_TO_TIMER(30));
	srand(time(0));
}

ASKWindow::~ASKWindow()
{
	this->UnloadContent();
}

bool ASKWindow::UpdateFrame()
{
	return true;
}

void ASKWindow::RenderFrame()
{
	const int WHITE = makecol(255, 255, 255);
	textprintf_ex(_backBuffer, font, 8, 8, WHITE, -1, "Allegro Starter Kit - Press F9 to Close");
	blit(_backBuffer, screen, 0, 0, 0, 0, _backBuffer->w, _backBuffer->h);
}

bool ASKWindow::LoadContent()
{
	return true;
}

void ASKWindow::UnloadContent()
{
}

void ASKWindow::Close()
{
	mainThreadIsRunning = false;
}

int ASKWindow::Start()
{
	if (!this->LoadContent())
	{
		return 1;
	}
	while(mainThreadIsRunning)
	{
		if (timerTickCount > 0)
		{
			while (timerTickCount > 0)
			{
				if (keyboard_needs_poll())
				{
					poll_keyboard();
				}
				if (key[KEY_F9]) // panic key
				{
					mainThreadIsRunning = false;
				}
				if (mouse_needs_poll())
				{
					poll_mouse();
				}
				if (!this->UpdateFrame())
				{
					mainThreadIsRunning = false;
				}
				timerTickCount--;
			}
			this->RenderFrame();
		}
		else
		{
			rest(1);
		}
	}
	return 0;
}

namespace ASK
{
	int Run(ASKWindow* window)
	{
		if (!window)
		{
			return 1;
		}

		return window->Start();
	}
}
