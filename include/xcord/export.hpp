#pragma once

#ifdef WIN32
	#ifdef BUILDING_XCORD
		#define EXPORT __declspec(dllexport)
	#else
		#define EXPORT __declspec(dllimport)
	#endif
#else
	#define EXPORT
#endif
