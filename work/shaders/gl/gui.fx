//------------------------------------------------------------------------------
//  gui.fx
//
//	Basic GUI shader for use with LibRocket
//
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/techniques.fxh"

/// Declaring used textures
sampler2D Texture;

samplerstate TextureSampler
{
	Samplers = { Texture };
};

mat4 Model;

state DefaultGUIState
{
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	CullMode = Back;
	DepthEnabled = false;
};

state ScissorGUIState
{
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	CullMode = Back;
	ScissorEnabled = true;
	DepthEnabled = false;
};

//------------------------------------------------------------------------------
/**
*/
shader
void
vsMain(in vec3 position,
	in vec4 color,
	in vec2 uv,	
	out vec2 UV,
	out vec4 Color) 
{
	gl_Position = Model * vec4(position, 1);
	Color = color;
	UV = uv;
}

//------------------------------------------------------------------------------
/**
*/
shader
void
psMain(
	in vec2 UV,
	in vec4 Color,
	[color0] out vec4 FinalColor) 
{
	vec4 texColor = texture(Texture, UV);
	FinalColor = texColor * Color;
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(Default, "Static", vsMain(), psMain(), DefaultGUIState);
SimpleTechnique(Scissor, "Static|Alt0", vsMain(), psMain(), ScissorGUIState);