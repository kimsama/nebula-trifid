//------------------------------------------------------------------------------
//  particlesystemmaterialnodeinstance.cc
//  (C) 2011-2013 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesystemnodeinstance.h"
#include "particles/particlesystemnode.h"
#include "coregraphics/transformdevice.h"
#include "particles/particlerenderer.h"
#include "coregraphics/shadersemantics.h"
#include "models/model.h"
#include "materials/materialinstance.h"
#include "models/modelnodeinstance.h"
#include "models/modelinstance.h"
#include "graphics/modelentity.h"
#include "coregraphics/shaderserver.h"
#include "frame/frameserver.h"
#include "particleserver.h"

namespace Particles
{
__ImplementClass(Particles::ParticleSystemNodeInstance, 'PSNI', Models::StateNodeInstance);

using namespace Models;
using namespace Util;
using namespace CoreGraphics;
using namespace Math;
using namespace Materials;
using namespace Graphics;
using namespace Frame;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::ParticleSystemNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::~ParticleSystemNodeInstance()
{
    n_assert(!this->particleSystemInstance.isvalid());
}    

//------------------------------------------------------------------------------
/**
*/
void 
ParticleSystemNodeInstance::OnVisibilityResolve(IndexT resolveIndex, float distanceToViewer)
{
    // check if node is inside lod distances or if no lod is used
    const Ptr<TransformNode>& transformNode = this->modelNode.downcast<TransformNode>();
    if (transformNode->CheckLodDistance(distanceToViewer))
    {
        this->modelNode->AddVisibleNodeInstance(resolveIndex, this);
        ModelNodeInstance::OnVisibilityResolve(resolveIndex, distanceToViewer);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Setup(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
{
    n_assert(!this->particleSystemInstance.isvalid());

    // up to parent class
    StateNodeInstance::Setup(inst, node, parentNodeInst);

    // setup a new particle system instance
    this->particleSystemInstance = ParticleSystemInstance::Create();
    const Ptr<ParticleSystemNode>& particleSystemNode = node.downcast<ParticleSystemNode>();

	// get mesh
	Ptr<Mesh> emitterMesh;
	if (particleSystemNode->GetEmitterMesh().isvalid())
	{
		emitterMesh = particleSystemNode->GetEmitterMesh()->GetMesh();
	}
	else
	{
		emitterMesh = ParticleServer::Instance()->GetDefaultEmitterMesh();
	}

	// setup and start particle system
    this->particleSystemInstance->Setup(emitterMesh, particleSystemNode->GetPrimitiveGroupIndex(), particleSystemNode->GetEmitterAttrs());
	this->particleSystemInstance->Start();

	const Ptr<MaterialInstance>& materialInstance = this->modelNode.downcast<StateNode>()->GetMaterialInstance();
	// get shader variables from shader

	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_EMITTERTRANSFORM))
	{
		this->emitterOrientation = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_EMITTERTRANSFORM);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_BILLBOARD))
	{
		this->billBoard = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_BILLBOARD);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_BBOXCENTER))
	{
		this->bboxCenter = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_BBOXCENTER);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_BBOXSIZE))
	{
		this->bboxSize = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_BBOXSIZE);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_TIME))
	{
		this->time = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_TIME);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_ANIMPHASES))
	{
		this->animPhases = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_ANIMPHASES);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_ANIMSPERSEC))
	{
		this->animsPerSec = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_ANIMSPERSEC);
	}
	if (materialInstance->HasVariableByName(NEBULA3_SEMANTIC_DEPTHBUFFER))
	{
		Ptr<FrameShader> defaultFrameShader = FrameServer::Instance()->LookupFrameShader(NEBULA3_DEFAULT_FRAMESHADER_NAME);
		Ptr<RenderTarget> depthTexture = defaultFrameShader->GetRenderTargetByName("DepthBuffer");
		this->depthBuffer = materialInstance->GetVariableByName(NEBULA3_SEMANTIC_DEPTHBUFFER);
		this->depthBuffer->SetTexture(depthTexture->GetResolveTexture());
	}


#if __PS3__
    const Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> > &shaderParams = particleSystemNode->GetShaderParameter();
    IndexT i;
    int numAnimPhases = -1;
    float animFramesPerSecond = -1.0f;
    for (i = 0; i < shaderParams.Size(); i++)
    {
        if(shaderParams[i].Key() == "ALPHAREF")
        {
            numAnimPhases = shaderParams[i].Value().GetInt();
        }
        else if(shaderParams[i].Key() == "INTENSITY1")
        {
            animFramesPerSecond = shaderParams[i].Value().GetFloat();
        }
    }
    n_assert(-1 != numAnimPhases);
    n_assert(-1.0f != animFramesPerSecond);
    this->particleSystemInstance->SetNumAnimPhases(numAnimPhases);
    this->particleSystemInstance->SetAnimFramesPerSecond(animFramesPerSecond);
#endif

}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::Discard()
{
    n_assert(this->particleSystemInstance.isvalid());

    // discard our particle system instance
    this->particleSystemInstance->Discard();
    this->particleSystemInstance = 0;

    // up to parent-class
    StateNodeInstance::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnRenderBefore(IndexT frameIndex, Timing::Time time)
{
    // call parent class
    StateNodeInstance::OnRenderBefore(frameIndex, time);    

    this->particleSystemInstance->OnRenderBefore();

    // update particle system with new model transform
    this->particleSystemInstance->SetTransform(this->modelTransform);

    // updating happens in 2 stages:
    // 1) within activity distance: particles are emitted and updated
    // 2) in view volume: particle system is added for rendering
    const point& eyePos = TransformDevice::Instance()->GetInvViewTransform().get_position();
    const point& myPos  = this->modelTransform.get_position();
    float dist = float4(myPos - eyePos).length();
    float activityDist = this->particleSystemInstance->GetParticleSystem()->GetEmitterAttrs().GetFloat(EmitterAttrs::ActivityDistance);
    if (dist <= activityDist && this->modelNode->GetResourceState() == Base::ResourceBase::Loaded)
    {
        // alright, we're within the activity distance, update the particle system
        this->particleSystemInstance->Update(time);

        // check if we're also in the view volume, and if yes, 
        // register the particle system for rendering
        // FIXME: use actual particle bounding box!!!
        const bbox& localBox = this->particleSystemInstance->GetBoundingBox();
		const Ptr<Graphics::ModelEntity>& modelEntity = this->modelInstance->GetModelEntity();
		modelEntity->ExtendLocalBoundingBox(localBox);
		
        /*
        // get model entity so that we can retrieve its local bounding box
        const Ptr<Graphics::ModelEntity>& modelEntity = this->modelInstance->GetModelEntity();

        // get transform from model
        // inverse transform
        // now apply to the global bounding box of the particles, the particles should now be in local space
        matrix44 trans = modelEntity->GetTransform();
        trans = matrix44::inverse(trans);
        bbox localBox = modelEntity->GetLocalBoundingBox();
        bbox localParticleBox = globalBox;
        localParticleBox.transform(trans);

        // extend with local particle box and update model entity local bounding box
        localBox.extend(localParticleBox);
        modelEntity->SetLocalBoundingBox(localBox);
        */
		
        const matrix44& viewProj = TransformDevice::Instance()->GetViewProjTransform();
		if (ClipStatus::Outside != localBox.clipstatus(viewProj))
        {
            // yes, we're visible
            ParticleRenderer::Instance()->AddVisibleParticleSystem(this->particleSystemInstance);
        }
        else
        {
            // FIXME DEBUG
            // n_printf("%f: Particle system invisible (clip) %s!\n", time, this->modelNode->GetName().Value());
        }
    }
    else
    {
        // FIXME DEBUG
        //n_printf("Particle system invisible (activity dist) %s!\n", this->modelNode->GetName().Value());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::ApplyState()
{
	// set variables
	if (this->GetParticleSystemInstance()->GetParticleSystem()->GetEmitterAttrs().GetBool(EmitterAttrs::Billboard))
	{
		// use inverse view matrix for orientation 
		this->emitterOrientation->SetMatrix(TransformDevice::Instance()->GetInvViewTransform());
	}
	else
	{
		// otherwise, use the matrix of the particle system
		this->emitterOrientation->SetMatrix(this->GetParticleSystemInstance()->GetTransform());
	}

    //billBoard->SetBool(this->GetParticleSystemInstance()->GetParticleSystem()->GetEmitterAttrs().GetBool(EmitterAttrs::Billboard));	
	//this->bboxCenter->SetFloat4(this->GetParticleSystemInstance()->GetBoundingBox().center());
	//this->bboxSize->SetFloat4(this->GetParticleSystemInstance()->GetBoundingBox().extents());
	this->animPhases->SetInt(this->GetParticleSystemInstance()->GetParticleSystem()->GetEmitterAttrs().GetInt(EmitterAttrs::AnimPhases));
	this->animsPerSec->SetFloat(this->GetParticleSystemInstance()->GetParticleSystem()->GetEmitterAttrs().GetFloat(EmitterAttrs::PhasesPerSecond));

	// call base class (applies time)
	StateNodeInstance::ApplyState();
}

//------------------------------------------------------------------------------
/**
*/
void 
ParticleSystemNodeInstance::Render()
{
    StateNodeInstance::Render();
    ParticleRenderer::Instance()->RenderParticleSystem(this->particleSystemInstance);
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::RenderDebug()
{
    StateNodeInstance::RenderDebug();
    this->particleSystemInstance->RenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnShow(Timing::Time time)
{
    this->particleSystemInstance->Start();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnHide(Timing::Time time)
{
    // FIXME: should stop immediately?
    if (this->particleSystemInstance->IsPlaying())
    {
        this->particleSystemInstance->Stop();
    }
}

} // namespace Particles
