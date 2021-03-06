//------------------------------------------------------------------------------
//  tileddirectoryitem.cc
//  (C) 2012-2014 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tileddirectoryitem.h"
#include <QIcon>
#include <QStyle>
#include <QApplication>
#include <QGraphicsSceneEvent>

namespace ResourceBrowser
{

//------------------------------------------------------------------------------
/**
*/
TiledDirectoryItem::TiledDirectoryItem()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
TiledDirectoryItem::~TiledDirectoryItem()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
TiledDirectoryItem::Setup()
{
	TiledGraphicsItem::Setup();

	QPixmap pix(":/icons/qtaddons/resourcebrowser/ui/folder_512.png");
	pix = pix.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	this->graphics->setPixmap(pix);
	QString format;
	format.sprintf("<p align=\"center\">%s</p>", this->dir.AsCharPtr());
	this->label->setTextWidth(this->background->boundingRect().width());
	this->label->setHtml(format);

	// set position of graphics
	this->graphics->setPos(
		this->background->boundingRect().width() / 2 - this->graphics->boundingRect().width() / 2,
		this->background->boundingRect().width() / 2 - this->graphics->boundingRect().height() / 2);	// using width twice is not a bug, we want the same margin!

	// move label to appear underneath graphics
	this->label->setPos(this->label->pos().x(), this->graphics->boundingRect().height() + 10);
}

//------------------------------------------------------------------------------
/**
*/
void
TiledDirectoryItem::Discard()
{
	TiledGraphicsItem::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
TiledDirectoryItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit this->OnSelected(QString(this->dir.AsCharPtr()));
	}
}

} // namespace ResourceBrowser