#pragma once

#define GLIWDLL_API __declspec(dllexport)

#include <iostream>
#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace gliw
{
	namespace detail
	{
		GLIWDLL_API int init(int width, int height);
		GLIWDLL_API void deInit(void);
		GLIWDLL_API int update(void);
		GLIWDLL_API void swapBuffers(void);
		GLIWDLL_API int getWidth(void);
		GLIWDLL_API int getHeight(void);
	};

	static int init(int width, int height)
	{
		int tmp = detail::init(width, height);
		glewInit();
		return tmp;
	}

	static int update(void)
	{
		return detail::update();
	}

	static void swapBuffers(void)
	{
		detail::swapBuffers();
	}

	static int getWidth(void)
	{
		return detail::getWidth();
	}

	static int getHeight(void)
	{
		return detail::getHeight();
	}
};