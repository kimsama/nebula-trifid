//------------------------------------------------------------------------------
//  shaperendererbase.cc
//  (C) 2007 Radon Labs GmbH
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaperendererbase.h"
#include "threading/threadid.h"

namespace Base
{
__ImplementClass(Base::ShapeRendererBase, 'SRBS', Core::RefCounted);
__ImplementSingleton(Base::ShapeRendererBase);

using namespace Math;
using namespace Util;
using namespace Threading;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::ShapeRendererBase() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::~ShapeRendererBase()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->shapes[RenderShape::AlwaysOnTop].IsEmpty());
	n_assert(this->shapes[RenderShape::CheckDepth].IsEmpty());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Close()
{
    n_assert(this->isOpen);
    this->isOpen = false;
	this->shapes[RenderShape::AlwaysOnTop].Clear();
    this->shapes[RenderShape::CheckDepth].Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DeleteShapesByThreadId(Threading::ThreadId threadId)
{
    n_assert(this->IsOpen());
	IndexT t;	
	for(t = 0; t<CoreGraphics::RenderShape::NumDepthFlags; t++)
	{
		IndexT i;
		for (i = this->shapes[t].Size() - 1; i != InvalidIndex; i--)
		{
			ThreadId shapeThreadId = this->shapes[t][i].GetThreadId();
			n_assert(shapeThreadId != InvalidThreadId);
			if (shapeThreadId == threadId)
			{
				this->shapes[t].EraseIndex(i);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::AddShape(const RenderShape& shape)
{
    n_assert(this->IsOpen());
	this->shapes[shape.GetDepthFlag()].Append(shape);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::AddShapes(const Array<RenderShape>& shapeArray)
{
    n_assert(this->IsOpen());
	for (int i = 0; i<shapeArray.Size();i++)
	{
		this->shapes[shapeArray[i].GetDepthFlag()].Append(shapeArray[i]);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DrawShapes()
{
    // override in subclass!
    n_error("ShapeRendererBase::DrawShapes() called!");
}

//------------------------------------------------------------------------------
/**
*/
void 
ShapeRendererBase::AddWireFrameBox(const Math::bbox& boundingBox, const Math::float4& color, Threading::ThreadId threadId)
{
    // render lines around bbox
    const Math::point& center = boundingBox.center();
    const Math::vector& extends = boundingBox.extents();    
    const Math::vector corners[] = {  vector(1,1,1),
        vector(1,1,-1),
        vector(1,1,-1),
        vector(-1,1,-1),
        vector(-1,1,-1),
        vector(-1,1,1),
        vector(-1,1,1),
        vector(1,1,1),

        vector(1,-1,1),
        vector(1,-1,-1),
        vector(1,-1,-1),
        vector(-1,-1,-1),
        vector(-1,-1,-1),
        vector(-1,-1,1),
        vector(-1,-1,1),
        vector(1,-1,1),

        vector(1,1,1),
        vector(1,-1,1),
        vector(1,1,-1),
        vector(1,-1,-1),
        vector(-1,1,-1),
        vector(-1,-1,-1),
        vector(-1,1,1),
        vector(-1,-1,1)};

    Util::Array<point> lineList;
    IndexT i;
    for (i = 0; i < 24; ++i)
    {
        lineList.Append(center + float4::multiply(extends, corners[i]));    	
    }       
    RenderShape shape;
    shape.SetupPrimitives(threadId,
        matrix44::identity(),
        PrimitiveTopology::LineList,
        lineList.Size()/ 2,
        &(lineList.Front()),
        4,
        color,
		CoreGraphics::RenderShape::CheckDepth);
    this->AddShape(shape);    
}


} // namespace Base
