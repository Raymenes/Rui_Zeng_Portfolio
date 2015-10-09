#pragma once
#include <memory>
#include <vector>
#include <stack>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>
#include <wx/dcmemory.h>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);
    
    bool HasActiveCommand();
    
    void CreateCommand(CommandType type, const wxPoint &start);
    
    void UpdateCommand(const wxPoint& updatePoint);
    
    void FinalizeCommand();
    
    bool CanRedo();
    
    bool CanUndo();
    
    void Redo();
    
    void Undo();
    
    //load in an external img file
    //this will clear out all the current img board content
    void LoadFile(std::string filename, wxBitmapType type);
    
    //save the current img board content to the destination
    void SaveFile(std::string filename, wxSize bit_map_size, wxBitmapType type);
    
    const wxColour GetPenColor(){return mPen.GetColour();}
    
    int GetPenWidth(){return mPen.GetWidth();}
    
    const wxColour GetBrushColor(){return mBrush.GetColour();}
    
    wxPen GetPen(){return mPen;}
    
    wxBrush GetBrush(){return mBrush;}
    
    void SetPen(wxPen pen){mPen = pen;}
    
    void SetBrush(wxBrush brush){mBrush = brush;}
    
    void SetPenColor(const wxColour &col){mPen.SetColour(col);}
    
    void SetBrushColor(const wxColour &col){mBrush.SetColour(col);}
    
    void SetPenWidth(int width){mPen.SetWidth(width);}
    
    void InitPenBrush()
    {
        mPen = *wxBLACK_PEN;
        mBrush = *wxWHITE_BRUSH;
    }
    
    void SelectShape(const wxPoint &point)
    {
        //loop thru front to back to see if any shape can be selected
        for (int i = (int)mShapes.size()-1; i >= 0; i--) {
            if(mShapes.at(i)->Intersects(point))
            {
                mSelected = mShapes.at(i);
                return;
            }
        }
    }
    
    void ClearSelect(){ mSelected = nullptr; }
    
    std::shared_ptr<Shape> GetSelected(){return mSelected;}

private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
    
    std::shared_ptr<Shape> mSelected;
    
    //my current active command
    std::shared_ptr<Command> mCommand;
    
    std::stack< std::shared_ptr<Command> > mUndoStack;
    std::stack< std::shared_ptr<Command> > mRedoStack;
    
    wxPen mPen;
    wxBrush mBrush;
    
    wxBitmap mImportBitmap;
};
