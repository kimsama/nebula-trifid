#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::BillboardNode
    
     representation of a billboard shape. 
	Is basically a shape node but without a mesh load from file.
    
    (C) 2013-2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "models/nodes/statenode.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/vertexlayout.h"
#include "coregraphics/shaderinstance.h"
namespace Billboards
{
class BillboardNode : public Models::StateNode
{
	__DeclareClass(BillboardNode);
public:
	/// constructor
	BillboardNode();
	/// destructor
	virtual ~BillboardNode();

	/// create a model node instance
	virtual Ptr<Models::ModelNodeInstance> CreateNodeInstance() const;

}; 

} // namespace Models
//------------------------------------------------------------------------------