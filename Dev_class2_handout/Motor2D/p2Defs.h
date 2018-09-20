#ifndef __P2DEFS_H__
#define __P2DEFS_H__

#include <stdio.h>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned char uchar;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// TODO 5: Improve config.xml to store all configuration variables that we have as macros
// Use a section with the name of each module (see Module::name)

#define FULLSCREEN App->node.child("j1Window").child("fullscreen").attribute("value").as_bool()//false
#define BORDERLESS App->node.child("j1Window").child("borderless").attribute("value").as_bool()//false
#define RESIZABLE App->node.child("j1Window").child("resizable").attribute("value").as_bool()//true
#define FULLSCREEN_WINDOW App->node.child("j1Window").child("fullscreen_window").attribute("value").as_bool()//false
#define TITLE App->node.child("j1Window").child_value("name") //"replaceme title"
#define WIDTH App->node.child("j1Window").child("width").attribute("value").as_int()//1024
#define HEIGHT App->node.child("j1Window").child("height").attribute("value").as_int()//768
#define SCALE App->doc.child("config").child("j1Window").child("scale").attribute("value").as_int() //1

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

#endif