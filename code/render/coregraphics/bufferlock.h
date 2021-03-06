#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::BufferLock
    
	Defines different buffer locks for different platforms.
	
	(C) 2012-2015 Individual contributors, see AUTHORS file
*/
#if __OGL4__
#include "coregraphics/ogl4/ogl4bufferlock.h"
namespace CoreGraphics
{
class BufferLock : public OpenGL4::OGL4BufferLock
{
	__DeclareClass(BufferLock);
};
}
#else
#error "Bufferlock class not implemented on this platform!"
#endif