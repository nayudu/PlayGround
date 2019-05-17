#include "mainwindow.h"
#include <QtWidgets>
#include <QTimeLine>
#include <QDebug>

#include <map>
#include <utility>

enum class Anchor
{
	kTopLeft,
	kTop,
	kTopRight,
	kLeft,
	kCenter,
	kRight,
	kBottomLeft,
	kBottom,
	kBottomRight
};

std::map< Anchor, std::pair< double, double > > kAnchorToFactorMap =
{
	{ Anchor::kTopLeft, {0, 0} },		{ Anchor::kTop, {-0.5, 0} },		{ Anchor::kTopRight, {-1, 0} },
	{ Anchor::kLeft, {0, -0.5} },		{ Anchor::kCenter, {-0.5, -0.5} },	{ Anchor::kRight, {-1, -0.5} },
	{ Anchor::kBottomLeft, {0, -1 } },	{ Anchor::kBottom, {-0.5, -1} },	{ Anchor::kBottomRight, {-1, -1} }
};

class AnchorObject : public QObject
{
public:
	AnchorObject( QWidget * inWidget, QPoint const & inRefPoint, Anchor inAnchor )
	:
		QObject( inWidget ),
		mAnchoredWidget( inWidget ),
		mRefPoint( inRefPoint ),
		mAnchor( inAnchor ),
		mOffset{0, 0}
	{
		inWidget->installEventFilter( this );
	}

	void SetAnchor( Anchor inAnchor )
	{
		mAnchor = inAnchor;
	}

	void SetReferencePoint( QPoint const & inRefPoint )
	{
		mRefPoint = inRefPoint;
	}

	void SetOffset( QPoint const & inOffset )
	{
		mOffset = inOffset;
	}

	virtual bool eventFilter( QObject *obj, QEvent *event ) override
	{
		if( event->type() == QEvent::Resize )
			UpdateWidgetPosition();

		return QObject::eventFilter(obj, event);
	}

private:
	void UpdateWidgetPosition()
	{
		auto const width = mAnchoredWidget->width();
		auto const height = mAnchoredWidget->height();
		auto const factorPair = kAnchorToFactorMap[mAnchor];
		auto const posX = static_cast< int >( mRefPoint.x() + factorPair.first * width + mOffset.x() );
		auto const posY = static_cast< int >( mRefPoint.y() + factorPair.second * height + mOffset.y() );

		mAnchoredWidget->move( posX, posY );
	}

private:
	QWidget * mAnchoredWidget;
	QPoint mRefPoint;
	Anchor mAnchor;
	QPoint mOffset;
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setFixedSize( 1000, 1000 );

	QPushButton *button1 = new QPushButton("Prasad", this);
	button1->setFixedWidth(10);

	QFrame *vframe = new QFrame( this );
	vframe->setFixedSize( 2,1000 );
	vframe->move( 499, 0 );
	vframe->setStyleSheet( "background-color: yellow");

	QFrame *hframe = new QFrame( this );
	hframe->setFixedSize( 1000, 2 );
	hframe->move( 0, 499 );
	hframe->setStyleSheet( "background-color: yellow");

	AnchorObject * anchorer = new AnchorObject( button1, QPoint( 500,500 ), Anchor::kRight );
	anchorer->SetOffset( QPoint( 0, 0 ) );

	auto step = 1000/25;

	QObject::connect
	(
		button1, &QPushButton::clicked,
		[button1, this, step](bool)
		{
			width += step;
			qDebug()<<width;
			button1->setFixedWidth( width );
		}
	);
}

MainWindow::~MainWindow()
{

}
