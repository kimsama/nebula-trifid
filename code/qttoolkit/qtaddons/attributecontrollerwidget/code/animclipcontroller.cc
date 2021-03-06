//------------------------------------------------------------------------------
//  animclipcontroller.cc
//  (C) 2013-2014 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animclipcontroller.h"
#include "game/entity.h"
#include "db/valuetable.h"
#include "ui_stringlistcontroller.h"
#include "basegamefeature/basegameprotocol.h"
#include "toolkit/toolkitutil/idldocument/idlattribute.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "messaging/messagecallbackhandler.h"

using namespace Util;
using namespace Attr;

namespace QtAttributeControllerAddon
{

//------------------------------------------------------------------------------
/**
*/
AnimClipController::AnimClipController(QWidget* parent, const Ptr<Game::Entity>& entity, const Ptr<Tools::IDLAttribute>& idlattr) :BaseAttributeController(parent)
{
	this->entity = entity;	
	this->attributeId = AttrId(idlattr->GetName());
	this->type = Util::Variant::String;

	// setup ui
	this->ui = new Ui::StringListController();
	this->ui->setupUi(this);

	this->ui->comboBox->installEventFilter(this);


	Ptr<BaseGameFeature::GetAttribute> msg = BaseGameFeature::GetAttribute::Create();
	msg->SetAttributeId(this->attributeId);
	__SendSync(entity, msg);

	this->currentValue = msg->GetAttr().GetString();

	Ptr<GraphicsFeature::GetModelEntity> entityMsg = GraphicsFeature::GetModelEntity::Create();
	__SendSync(entity,entityMsg);

	Ptr<Graphics::FetchClips> clipsMsg = Graphics::FetchClips::Create();
	__Send(entityMsg->GetEntity(), clipsMsg);
	__SingleFireCallback(AnimClipController, OnFetchedClipList, this, clipsMsg.upcast<Messaging::Message>());
	
}

//------------------------------------------------------------------------------
/**
*/
AnimClipController::~AnimClipController()
{
	this->entity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
AnimClipController::currentIndexChanged(int)
{
	BaseAttributeController::OnValueChanged(); 
}
//------------------------------------------------------------------------------
/**
*/
Util::Variant 
AnimClipController::GetValue() const
{
	Util::String value = this->ui->comboBox->currentText().toUtf8().constData();
	return Util::Variant(value);
}

//------------------------------------------------------------------------------
/**
*/
void 
AnimClipController::Lock()
{
	this->ui->comboBox->setEnabled(false);	
}


//------------------------------------------------------------------------------
/**
*/
void
AnimClipController::OnFetchedClipList(const Ptr<Messaging::Message>& msg)
{
	const Ptr<Graphics::FetchClips>& clipMsg = msg.downcast<Graphics::FetchClips>();
	const Util::Array<Util::StringAtom>& clips(clipMsg->GetClips());
	
	if (clips.Size() == 0)
	{
		this->ui->comboBox->addItem(this->currentValue.AsCharPtr());
		this->ui->comboBox->setCurrentIndex(0);
		this->ui->comboBox->setEnabled(false);
		return;
	}

	bool connected = false;
	connected = connect(this->ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
	n_assert(connected);

	this->ui->comboBox->setEnabled(true);	
	this->SetClips(clips, this->currentValue);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimClipController::SetClips(const Util::Array<Util::StringAtom> & clips, const Util::String & current)
{
	int selection = 0;
	
	this->ui->comboBox->clear();
	for (IndexT i = 0; i < clips.Size(); i++)
	{
		this->ui->comboBox->addItem(clips[i].AsString().AsCharPtr());
		if (clips[i] == current)
		{
			selection = i;
		}
	}
	this->ui->comboBox->setCurrentIndex(selection);
}

}