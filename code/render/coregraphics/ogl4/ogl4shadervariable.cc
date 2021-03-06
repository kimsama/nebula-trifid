//------------------------------------------------------------------------------
//  OGL4ShaderVariable.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/ogl4/ogl4shadervariable.h"
#include "coregraphics/shadervariableinstance.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/ogl4/ogl4renderdevice.h"
#include "math/vector.h"


namespace OpenGL4
{
__ImplementClass(OpenGL4::OGL4ShaderVariable, 'D1VR', Base::ShaderVariableBase);

using namespace CoreGraphics;
using namespace Math;
using namespace Util;
//------------------------------------------------------------------------------
/**
*/
OGL4ShaderVariable::OGL4ShaderVariable() :	
    effectVar(0),
	texture(0)
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
OGL4ShaderVariable::~OGL4ShaderVariable()
{
	this->texture = 0;
	this->effectVar = 0;
}

//------------------------------------------------------------------------------
/**
*/
void OGL4ShaderVariable::Setup(AnyFX::EffectVariable* var)
{
	n_assert(0 == effectVar);
	
	String name = var->GetName().c_str();
	this->SetName(name);
	this->SetSemantic(name);
	switch (var->GetType())
	{
	case AnyFX::Double:
	case AnyFX::Float:
		this->SetType(FloatType);
		break;
	case AnyFX::Short:
	case AnyFX::Integer:
	case AnyFX::UInteger:
		this->SetType(IntType);
		break;
	case AnyFX::Bool:
		this->SetType(BoolType);
		break;
	case AnyFX::Float2:
	case AnyFX::Float3:
	case AnyFX::Float4:	
	case AnyFX::Double2:
	case AnyFX::Double3:
	case AnyFX::Double4:
	case AnyFX::Integer2:
	case AnyFX::Integer3:
	case AnyFX::Integer4:
	case AnyFX::UInteger2:
	case AnyFX::UInteger3:
	case AnyFX::UInteger4:
	case AnyFX::Short2:
	case AnyFX::Short3:
	case AnyFX::Short4:
	case AnyFX::Bool2:
	case AnyFX::Bool3:
	case AnyFX::Bool4:
		this->SetType(VectorType);
		break;
	case AnyFX::Matrix2x2:
	case AnyFX::Matrix2x3:
	case AnyFX::Matrix2x4:
	case AnyFX::Matrix3x2:
	case AnyFX::Matrix3x3:
	case AnyFX::Matrix3x4:
	case AnyFX::Matrix4x2:
	case AnyFX::Matrix4x3:
	case AnyFX::Matrix4x4:
		this->SetType(MatrixType);
		break;
	case AnyFX::Sampler1D:
	case AnyFX::Sampler1DArray:
	case AnyFX::Sampler2D:
	case AnyFX::Sampler2DArray:
	case AnyFX::Sampler2DMS:
	case AnyFX::Sampler2DMSArray:
	case AnyFX::Sampler3D:
	case AnyFX::SamplerCube:
	case AnyFX::SamplerCubeArray:
	case AnyFX::Image1D:
	case AnyFX::Image1DArray:
	case AnyFX::Image2D:
	case AnyFX::Image2DArray:
	case AnyFX::Image2DMS:
	case AnyFX::Image2DMSArray:
	case AnyFX::Image3D:
	case AnyFX::ImageCube:
	case AnyFX::ImageCubeArray:
		this->SetType(TextureType);
		break;
	}
	this->effectVar = var;
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::Setup(AnyFX::EffectVarbuffer* var)
{
	String name = var->GetName().c_str();
	this->SetName(name);
	this->SetSemantic(name);
	this->effectBuffer = var;
	this->SetType(BufferType);
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4ShaderVariable::Cleanup()
{
	n_assert(0 != this->effectVar || 0 != this->effectBuffer);

	this->effectBuffer = 0;
	this->effectVar = 0;
	this->texture = 0;
	ShaderVariableBase::Cleanup();
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetInt(int value)
{
	n_assert(0 != this->effectVar);
	
	this->effectVar->SetInt(value);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetIntArray(const int* values, SizeT count)
{
	n_assert(0 != this->effectVar);
	n_assert(0 != values);
	
	this->effectVar->SetIntArray(values, count);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetFloat(float value)
{
	n_assert(0 != this->effectVar);
	
	this->effectVar->SetFloat(value);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetFloatArray(const float* values, SizeT count)
{
	n_assert(0 != this->effectVar);
	n_assert(0 != values);
	
	this->effectVar->SetFloatArray(values, count);
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4ShaderVariable::SetFloat2( const Math::float2& value )
{
	n_assert(0 != this->effectVar);
	
	this->effectVar->SetFloat2((float*)&value);
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4ShaderVariable::SetFloat2Array( const Math::float2* values, SizeT count )
{
	n_assert(0 != this->effectVar);

	this->effectVar->SetFloat2Array((float*)&values, count);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetFloat4(const float4& value)
{
	n_assert(0 != this->effectVar);
	
	this->effectVar->SetFloat4((float*)&value);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetFloat4Array(const float4* values, SizeT count)
{
	n_assert(0 != this->effectVar);
	n_assert(0 != values);

	this->effectVar->SetFloat4Array((float*)values, count);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetMatrix(const matrix44& value)
{
	n_assert(0 != this->effectVar);
	this->effectVar->SetMatrix((float*)&value);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetMatrixArray(const matrix44* values, SizeT count)
{
	n_assert(0 != this->effectVar);
	n_assert(0 != values);

	this->effectVar->SetMatrixArray((float*)values, count);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetBool(bool value)
{
	n_assert(0 != this->effectVar);
	this->effectVar->SetBool(value);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetBoolArray(const bool* values, SizeT count)
{
	n_assert(0 != this->effectVar);

    // hmm... Win32's BOOL is actually an int
    const int MaxNumBools = 128;
    n_assert(count < MaxNumBools);
    bool tmp[MaxNumBools];
    IndexT i;
    for (i = 0; i < count; i++)
    {
		tmp[i] = values[i];
    }
	
	this->effectVar->SetBoolArray((const bool*)tmp, count);
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetTexture(const Ptr<CoreGraphics::Texture>& value)
{
	n_assert(0 != this->effectVar);

	this->texture = value;
    if (value.isvalid())
    {
	    this->effectVar->SetTexture((void*)value->GetOGL4Variable());
    }
    else
    {
        this->effectVar->SetTexture(NULL);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
OGL4ShaderVariable::SetBufferHandle(void* handle)
{
	n_assert(0 != this->effectBuffer);
	this->effectBuffer->SetBuffer(handle);
}

} // namespace OpenGL4
