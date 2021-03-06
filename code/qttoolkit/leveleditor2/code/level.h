#pragma once
//------------------------------------------------------------------------------
/**
    @class LevelEditor2::Level
    
    Holds level specific data, loads and saves as well
    
    (C) 2013 Johannes Hirche, LTU Skelleftea
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/xmlwriter.h"
#include "io/xmlreader.h"
#include "math/bbox.h"
namespace PostEffect
{
class PostEffectEntity;
}

namespace LevelEditor2
{
class Level : public Core::RefCounted
{
	__DeclareClass(Level);
	__DeclareSingleton(Level);
public:
	/// constructor
	Level();
	/// destructor
	virtual ~Level();

	/// Loads a level from an xml file in work:levels. creates entities via CreateEntityByAttrs and CreateGameEntityByAttrs
	/// will clear level with RemoveAllEntities when file exists	
	bool LoadLevel(const Util::String& name);
	/// Save level with different name, name is only basename, without path or extension
	void SaveLevelAs(const Util::String& name);
	/// Save level
	void SaveLevel();

	/// get level name
	const Util::String& GetName() const;

	/// set Level is starting level
	void SetStartLevel (const bool& sLevel);
	/// get Level is starting level
	const bool& GetStartLevel() const;
	/// set Auto batch of level after save
	void SetAutoBatch (const bool& batch);
	/// get Auto batch of level after save
	const bool& GetAutoBatch() const;
	/// load default level configuration
	void LoadDefaults();
	/// get current bounding box
	Math::bbox GetBoundingBox();

protected:
	/// load a legacy level file
	bool LoadLegacyLevel(const Ptr<IO::XmlReader>& reader);
	/// load regular level file
	bool LoadVersionedLevel(const Ptr<IO::XmlReader>& reader);
	/// export level to database
	void ExportLevel(const Util::String&level);
	/// construct an _ID from type optional category and name
	Util::String AllocateID(const Util::String& objType, const Util::String&category, const Util::String &name);
	/// write a string node
	void WriteString(const Ptr<IO::XmlWriter> & xmlWriter, const Util::String & node, const Util::String& content );
    /// loads generic PE attribute by attribute name and string value
    void ReadPostEffectAttribute(const Util::String& attrName, const Util::String& content, const Ptr<PostEffect::PostEffectEntity>& postEffectEntity);

	Util::String name;
	bool startLevel;
	bool autoBatch;	
	Util::Dictionary<Util::String,int> objectCounters;
}; 

//------------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Level::GetName () const
{
	return this->name;
}
//------------------------------------------------------------------------------------
/**
*/
inline void 
Level::SetStartLevel (const bool& sLevel)
{
	this->startLevel = sLevel;
}

//------------------------------------------------------------------------------------
/**
*/
inline const bool&
Level::GetStartLevel () const
{
	return this->startLevel;
}

//------------------------------------------------------------------------------------
/**
*/
inline void 
Level::SetAutoBatch (const bool& batch)
{
	this->autoBatch = batch;
}

//------------------------------------------------------------------------------------
/**
*/
inline const bool&
Level::GetAutoBatch () const
{
	return this->autoBatch;
};
} // namespace LevelEditor2
//------------------------------------------------------------------------------