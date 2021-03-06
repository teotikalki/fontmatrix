//
// C++ Interface: fmlayout
//
// Description:
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FMLAYOUT_H
#define FMLAYOUT_H


#include <QObject>
#include <QString>
#include <QRectF>
#include <QPointF>
#include <QMap>
#include <QThread>

#include "fmsharestruct.h"

class QGraphicsScene;
class FontItem;
class QGraphicsPixmapItem;
class QGraphicsPathItem;
class QGraphicsRectItem;
class QGraphicsProxyWidget;
class QProgressBar;
class QAction;
class QMenu;
class QMutex;
class FMLayOptWidget;
class QDialog;
class QGridLayout;

/**
 Finally, we want to layout text elsewhere than _in_ the font or _in_ the view.
 And it’s "peu ou prou" forced if we really want to experiment in Fontmatrix
 I hope it will not be too much more than just copy & paste from FontItem & MainViewWidget - pm
 It has been quite more! - pm
**/

#define INFINITE 99999999L

class FMLayout;


struct Node
{
	struct ListItem
	{
		ListItem();
		ListItem ( Node* N, double D );
		~ListItem() ;
		Node* n;
		double distance;
		
		
// 		private:
// 		ListItem (const ListItem& v);
	};

	Node (FMLayout * layoutEngine,  int i ) ;
	~Node();

	QList<ListItem*> nodes;
	FMLayout *lyt;
	int index;

	bool hasNode ( int idx ) ;
	void nodes_clear();
	void nodes_insert(ListItem * v);

	void sPath ( double dist, QList< int > curList, QList<int>& theList, double& theScore );
	int deepCount();

private:
	Node() {}
	
};


		
class FMLayout : public QObject
{
	Q_OBJECT

	public:
		explicit FMLayout ( QGraphicsScene* scene, FontItem* font = 0, QRectF rect = QRectF());
		~FMLayout();
		void doLayout(const QList<GlyphList>& spec , double fs, FontItem* font = 0 );
		
	private://methods
		/// Build a graph on node
		virtual void doGraph();
		/// Build the good list of lines
		virtual void doLines();
		
		void clearCaches();

		void run();

	public:// utils
		double distance ( int start, int end, const GlyphList& gl , bool strip = false );
		int sepCount( int start, int end, const GlyphList& gl);
		void resetScene();

		bool isLayoutFinished() {return layoutIsFinished;}

		QList<int> breakList;
		QList<int> hyphenList;
		GlyphList theString;
		double lineWidth ( int l );
//		QMutex *layoutMutex;
		bool stopIt;
		int drawnLines;

		void setContext(bool c);
		
	public slots:
		void stopLayout();		

	private:// data
		// Argued
//		static FMLayout *instance;
		bool contextIsMainThread;
		QGraphicsScene* theScene;
		FontItem*	theFont;
		QList<GlyphList> paragraphs;
		QList<GlyphList> lines;
		QRectF theRect;// Not really argued now, will come soon
		QGraphicsRectItem *rules;
		FMLayOptWidget *optionsWidget;
		QGridLayout *optionLayout;
		bool persistentScene;

		// built
		Node *node;
		QList<int> indices;
		QList<QGraphicsPixmapItem *> pixList;
		QList<QGraphicsPathItem*> glyphList;
		QMap<int, QMap<int, double > > distCache;
		QMap<int, QMap<int, double > > stripCache;
		QMap<int, QMap<int, int > > sepCache;
		bool justRedraw;
		bool optionHasChanged;
		QPointF lastOrigine;

		// accessed
		bool processFeatures;
		QString script;
		bool processScript;
		double fontSize;
		double adjustedSampleInter;
		int textProgressionBlock;
		int textProgressionLine;
		bool deviceIndy;
		QPointF origine;

		bool layoutIsFinished;

	public: //accessors
		QRectF getRect()const{return theRect;}
		void setRect(const QRectF& r){theRect = r;}
		void setProcessFeatures ( bool theValue ){processFeatures = theValue;}
		void setScript ( const QString& theValue ){script = theValue;}
		void setProcessScript ( bool theValue )	{processScript = theValue;}
		void setAdjustedSampleInter ( double theValue );
		void setTextProgressionBlock ( int theValue ){textProgressionBlock = theValue;}
		void setTextProgressionLine ( int theValue ){textProgressionLine = theValue;}
		void setOrigine ( const QPoint& theValue ){origine = theValue;}
		void setFontSize ( bool theValue ){fontSize = theValue;}
		void setDeviceIndy( bool theValue ){deviceIndy = theValue;}
//		void setTheScene ( QGraphicsScene* theValue , QRectF rect = QRectF());
//		void setTheFont ( FontItem* theValue );
		void setPersistentScene(bool p){persistentScene = p;}


	private slots:
		/// Put lines on stage
		void doDraw();
		void endOfRun();
		void endOfParagraph();
		
		void slotOption(int v);

	signals:
		// needed if layout is executed outside the main (GUI) thread
		// receiver is expected to know with what font and on which scene;
		void drawPixmapForMe(int index, double fontsize, double x, double y);
		void drawBaselineForMe(double y);
		void clearScene();
		void objectWanted(QObject*);
		void updateLayout();
		void layoutFinished();
		void paragraphFinished();
		void paintFinished();
	public:
		QWidget *optionDialog;
		
		double FM_LAYOUT_NODE_SOON_F;
		double FM_LAYOUT_NODE_FIT_F;
		double FM_LAYOUT_NODE_LATE_F;
		double FM_LAYOUT_NODE_END_F;
		double FM_LAYOUT_HYPHEN_PENALTY;
		double FM_LAYOUT_MAX_COMPRESSION;
		QMenu * secretMenu;


};


#endif

