#ifndef TILESTAMPEDITORFORM_H
#define TILESTAMPEDITORFORM_H

#include <QToolButton>

#include "colorpushbutton.h"
#include "cdesignereditorbase.h"
#include "cdesignercommon.h"
#include "ctilestamprenderer.h"
#include "cchrromitemlistdisplaymodel.h"

typedef enum
{
   Overlay_HollowBox,
   Overlay_SolidBox,
   Overlay_Line,
   Overlay_Erase,
   Overlay_PasteClipboard,
   Overlay_FloodFill
} OverlayType;

namespace Ui {
   class TileStampEditorForm;
}

class CTileStamp;

class TileStampEditorForm : public CDesignerEditorBase
{
   Q_OBJECT

public:
   explicit TileStampEditorForm(QByteArray data,QByteArray attr,QString attrTblUUID,int xSize,int ySize,bool grid,IProjectTreeViewItem* link,QWidget *parent = 0);
   virtual ~TileStampEditorForm();

   QByteArray tileData();
   QByteArray attributeData();
   void currentSize(int* xSize,int* ySize) { (*xSize) = m_xSize; (*ySize) = m_ySize; }
   QUuid currentAttributeTable() { return m_attrTblUUID; }
   bool isGridEnabled() { return m_gridEnabled; }

   void initializeTile(QByteArray tileData,QByteArray attrData);
   void paintNormal();
   void setCurrentSize(int xSize,int ySize) { m_xSize = xSize; m_ySize = ySize; }

protected:
   void changeEvent(QEvent *event);
   void contextMenuEvent(QContextMenuEvent *event);
   void resizeEvent(QResizeEvent *event);
   void keyPressEvent(QKeyEvent *event);
   void renderer_enterEvent(QEvent* event);
   void renderer_leaveEvent(QEvent* event);
   void renderer_mousePressEvent(QMouseEvent *event);
   void renderer_mouseMoveEvent(QMouseEvent *event);
   void renderer_mouseReleaseEvent(QMouseEvent *event);
   void renderer_mouseDoubleClickEvent(QMouseEvent *event);
   bool eventFilter(QObject *obj, QEvent *event);
   void updateScrollbars();
   void recolorTiles(int pixx,int pixy,int newColor);
   void recolorClipboard(int boxX1,int boxY1,int boxX2,int boxY2);
   void paintOverlay(QByteArray overlayData,QByteArray overlayAttr,int overlayXSize,int overlayYSize,int boxX1,int boxY1,int boxX2,int boxY2);
   void paintOverlay(OverlayType type,int selectedColor,int boxX1,int boxY1,int boxX2,int boxY2);
   void paintOverlay(int selectedColor,int pixx,int pixy);
   void copyOverlayToNormal();
   void copyNormalToClipboard(int boxX1,int boxY1,int boxX2,int boxY2);
   void copyClipboardToOverlay();
   void updateInfoText(int x=-1,int y=-1);
   void paintTool(QMouseEvent* event);
   void pencilTool(QMouseEvent* event);
   void textTool(QMouseEvent* event);
   void tileTool(QMouseEvent* event);
   void filledBoxTool(QMouseEvent* event);
   void hollowBoxTool(QMouseEvent* event);
   void boxTool(QMouseEvent* event,bool filled);
   void lineTool(QMouseEvent* event);
   void selectionTool(QMouseEvent* event);

private slots:
   void on_paintAttr_clicked();
   void on_selectionTool_clicked();
   void on_flipVertical_clicked();
   void on_flipHorizontal_clicked();
   void on_areaTool_clicked();
   void on_cwRotate_clicked();
   void on_ccwRotate_clicked();
   void on_boxTool_clicked();
   void on_textTool_clicked();
   void on_lineTool_clicked();
   void on_pencilTool_clicked();
   void on_paintTool_clicked();
   void on_tileList_clicked(QModelIndex index);
   void on_tileList_activated(QModelIndex index);
   void on_clear_clicked();
   void grid_toggled(bool checked);
   void attributeTable_currentIndexChanged(int index);
   void ySize_currentIndexChanged(int index);
   void xSize_currentIndexChanged(int index);
   void colorPicked(bool value);
   void on_verticalScrollBar_valueChanged(int value);
   void on_horizontalScrollBar_valueChanged(int value);
   void on_zoomSlider_valueChanged(int value);
   void applyChangesToTab(QString uuid);

private:
   Ui::TileStampEditorForm *ui;
   CTileStampRenderer* renderer;
   CTileStampRenderer* previewer;
   CChrRomItemListDisplayModel* tileListModel;
   QList<ColorPushButton*> m_colors;
   int m_xSize;
   int m_ySize;
   QString m_attrTblUUID;
   bool m_gridEnabled;
   char* imgData;
   char* colorData;
   char* colorDataOverlay;
   char* colorDataClipboard;
   QToolButton* m_activeTool;
   QPoint m_anchor;
   bool m_selection;
   QRect m_selectionRect;
   bool m_clipboard;

public slots:
   void renderData();
};

class TileStampPaintCommand : public QUndoCommand
{
public:
   TileStampPaintCommand(TileStampEditorForm* pEditor,
                         QByteArray oldTileData,
                         QByteArray oldAttributeData,
                         QUndoCommand* parent = 0);
   virtual int id() const { return TILE_STAMP_PAINT_COMMAND; }
   virtual bool mergeWith(const QUndoCommand* command);
   virtual void redo();
   virtual void undo();

private:
   TileStampEditorForm* m_pEditor;
   QByteArray m_oldTileData;
   QByteArray m_oldAttributeData;
   QByteArray m_newTileData;
   QByteArray m_newAttributeData;
};

class TileStampResizeCommand : public QUndoCommand
{
public:
   TileStampResizeCommand(TileStampEditorForm* pEditor,
                         int oldXSize,
                         int oldYSize,
                         QUndoCommand* parent = 0);
   virtual int id() const { return TILE_STAMP_RESIZE_COMMAND; }
   virtual bool mergeWith(const QUndoCommand* command);
   virtual void redo();
   virtual void undo();

private:
   TileStampEditorForm* m_pEditor;
   int m_oldXSize;
   int m_oldYSize;
   int m_newXSize;
   int m_newYSize;
};

#endif // TILESTAMPEDITORFORM_H