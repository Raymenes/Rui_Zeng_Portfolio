#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
    
	virtual ~Shape() { }
    
    int GetPenWidth()
    {
        return mPen.GetWidth();
    }
    
    const wxColour GetBrushColor()
    {
        return mBrush.GetColour();
    }
    
    const wxColour GetPenColor()
    {
        return mPen.GetColour();
    }
    
    wxPen GetPen()
    {
        return mPen;
    }
    
    wxBrush GetBrush()
    {
        return mBrush;
    }
    
    void SetPen(wxPen pen)
    {
        mPen = pen;
    }
    
    void SetBrush(wxBrush brush)
    {
        mBrush = brush;
    }
    
    void SetPenColor(const wxColour &col)
    {
        mPen.SetColour(col);
    }
    
    void SetBrushColor(const wxColour &col)
    {
        mBrush.SetColour(col);
    }
    
    void SetPenWidth(int width)
    {
        mPen.SetWidth(width);
    }
    
    //initialize the default pen and brush
    void InitPenBrush()
    {
        mPen = *wxBLACK_PEN;
        mBrush = *wxWHITE_BRUSH;
    }
    
    //Draw a dashed rectangle selection box around (using offset)
    virtual void DrawSelection(wxDC& dc);
    
    const wxPoint& GetOffset(){return mOffset;}
    
    void SetOffset(wxPoint& point){ mOffset = point; }
    
protected:
    wxPen mPen;
    wxBrush mBrush;
    
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;
    //offset info for moving the shape
    wxPoint mOffset;
};
