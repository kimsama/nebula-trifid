//------------------------------------------------------------------------------
//  reflectionprojector.fx
//  (C) 2014 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/util.fxh"
#include "lib/techniques.fxh"
#include "lib/shared.fxh"

sampler2D NormalMap;
sampler2D SpecularMap;
sampler2D AlbedoMap;
sampler2D DepthMap;

vec4 BBoxMin;
vec4 BBoxMax;
vec4 BBoxCenter;
float FalloffDistance = 0.2f;
float FalloffPower = 16.0f;
int NumEnvMips = 9;
samplerCube EnvironmentMap;
samplerCube IrradianceMap;

samplerstate GeometrySampler
{
	Samplers = { NormalMap, DepthMap, SpecularMap, AlbedoMap };
	//Filter = Point;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
	Filter = MinMagMipLinear;
};

samplerstate EnvironmentSampler
{
	Samplers = { EnvironmentMap, IrradianceMap };
	Filter = MinMagMipLinear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

state ProjectorState
{
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	//SrcBlendAlpha[0] = One;
	//DstBlendAlpha[0] = One;
	//SeparateBlend = true;
	CullMode = Front;
	DepthEnabled = true;
	DepthFunc = Greater;
	//DepthClamp = false;
	DepthWrite = false;
};

prototype float CalculateDistanceField(vec3 point);
subroutine (CalculateDistanceField) float Box(
	in vec3 point)
{
	vec3 d = abs(point) - vec3(FalloffDistance);
	return min(max(d.x, max(d.y, d.z)), 0.0f) + length(max(d, 0.0f));
	//return length(max(abs(point) - vec3(FalloffDistance), vec3(0)));	
}

subroutine (CalculateDistanceField) float Sphere(
	in vec3 point)
{
	// use radius.x to get the blending factor
	return length(point) - FalloffDistance;
}

CalculateDistanceField calcDistanceField;

prototype vec3 CalculateCubemapCorrection(vec3 worldSpacePos, vec3 reflectVec);
subroutine (CalculateCubemapCorrection) vec3 ParallaxCorrect(
	in vec3 worldSpacePos, in vec3 reflectVec)
{
	vec3 intersectMin = (BBoxMin.xyz - worldSpacePos) / reflectVec;
	vec3 intersectMax = (BBoxMax.xyz - worldSpacePos) / reflectVec;
	vec3 largestRay = max(intersectMin, intersectMax);
	float distToIntersection = min(min(largestRay.x, largestRay.y), largestRay.z);
	vec3 intersectPos = worldSpacePos + reflectVec * distToIntersection;
	return intersectPos - BBoxCenter.xyz;		
}

subroutine (CalculateCubemapCorrection) vec3 NoCorrection(
	in vec3 worldSpacePos, in vec3 reflectVec)
{
	return reflectVec;
}

CalculateCubemapCorrection calcCubemapCorrection;
//------------------------------------------------------------------------------
/**
*/
shader
void
vsMain(in vec3 position,
	in vec3 normal,
	in vec2 uv,
	out vec3 ViewSpacePosition,
	out vec3 WorldViewVec,
	out vec2 UV) 
{
	vec4 modelSpace = Model * vec4(position, 1);
	ViewSpacePosition = (View * modelSpace).xyz;
    gl_Position = ViewProjection * modelSpace;
    UV = uv;
	WorldViewVec = modelSpace.xyz - EyePos.xyz;
}

//------------------------------------------------------------------------------
/**
	Calculate reflection projection using a box, basically the same as circle except we are using a signed distance function for boxes instead of a simple radius
*/
[earlydepth]
shader
void
psMain(in vec3 viewSpacePosition,	
	in vec3 worldViewVec,
	in vec2 uv,
	[color0] out vec4 Color)
{	
	vec2 pixelSize = GetPixelSize(DepthMap);
	vec2 screenUV = psComputeScreenCoord(gl_FragCoord.xy, pixelSize.xy);
	float depth = textureLod(DepthMap, screenUV, 0).r;
	
	// get view space position
	vec3 viewVec = normalize(viewSpacePosition);
	vec3 surfacePos = viewVec * depth;
	vec4 worldPosition = InvView * vec4(surfacePos, 1);
	vec4 localPos = InvModel * worldPosition;	
	
	vec3 dist = vec3(0.5f) - abs(localPos.xyz);
	if (all(greaterThan(dist, vec3(0))))
	{
		// sample normal and specular
		vec3 viewSpaceNormal = UnpackViewSpaceNormal(textureLod(NormalMap, screenUV, 0));
		vec4 spec = textureLod(SpecularMap, screenUV, 0);
		vec4 oneMinusSpec = 1 - spec;
		vec3 albedo = textureLod(AlbedoMap, screenUV, 0).rgb * oneMinusSpec.rgb;
	
		// calculate reflection
		//vec3 worldViewVec = ;
		//vec3 viewVecWorld = vec4(worldPosition.xyz - EyePos.xyz, 0).xyz;
		vec3 viewVecWorld = worldViewVec;
		vec3 worldNormal = (InvView * vec4(viewSpaceNormal, 0)).xyz;
		vec3 reflectVec = reflect(viewVecWorld, worldNormal);
		reflectVec = calcCubemapCorrection(worldPosition.xyz, reflectVec);
		// perform local parallax correction
		/*
		vec3 intersectMin = (BBoxMin.xyz - worldPosition.xyz) / reflectVec;
		vec3 intersectMax = (BBoxMax.xyz - worldPosition.xyz) / reflectVec;
		vec3 largestRay = max(intersectMin, intersectMax);
		float distToIntersection = min(min(largestRay.x, largestRay.y), largestRay.z);
		vec3 intersectPos = worldPosition.xyz + reflectVec * distToIntersection;
		reflectVec = intersectPos - BBoxCenter.xyz;
		*/
		
		// calculate unsigned distance field, then power by like a million to get a smooth transition
		float d = calcDistanceField(localPos.xyz);		
		float distanceFalloff = pow(saturate(1 - d), FalloffPower);
		
		// calculate reflection and irradiance
		float x =  dot(viewSpaceNormal, -viewVec);
		vec3 rim = FresnelSchlickGloss(spec.rgb, x, spec.a);
		vec3 refl = textureLod(EnvironmentMap, reflectVec, oneMinusSpec.a * NumEnvMips).rgb * rim;
		vec3 irr = textureLod(IrradianceMap, worldNormal.xyz, 0).rgb;
		Color = vec4(irr * albedo + refl, distanceFalloff);
	}
	else
	{
		discard;
	}
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(BoxProjector, "Alt0", vsMain(), psMain(calcDistanceField = Box, calcCubemapCorrection = NoCorrection), ProjectorState);
SimpleTechnique(SphereProjector, "Alt1", vsMain(), psMain(calcDistanceField = Sphere, calcCubemapCorrection = NoCorrection), ProjectorState);
SimpleTechnique(CorrectedBoxProjector, "Alt0|Alt2", vsMain(), psMain(calcDistanceField = Box, calcCubemapCorrection = ParallaxCorrect), ProjectorState);
SimpleTechnique(CorrectedSphereProjector, "Alt1|Alt2", vsMain(), psMain(calcDistanceField = Sphere, calcCubemapCorrection = ParallaxCorrect), ProjectorState);
