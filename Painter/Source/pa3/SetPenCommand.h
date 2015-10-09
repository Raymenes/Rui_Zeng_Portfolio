//
//  SetPenCommand.h
//  paint-mac
//
//  Created by Rui Zeng on 10/7/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include "Command.h"

class SetPenCommand : public Command
{
public:
    SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape, wxPen pen)
    : Command(start, shape)
    {
        new_pen = pen;
        old_pen.SetColour(shape->GetPenColor());
        old_pen.SetWidth(shape->GetPenWidth());
    }
    
    void Undo(std::shared_ptr<PaintModel> model) override
    {
        mShape->SetPen(old_pen);
    }
    
    void Redo(std::shared_ptr<PaintModel> model) override
    {
        mShape->SetPen(new_pen);
    }
    
    void Finalize(std::shared_ptr<PaintModel> model) override {}
    
protected:
    wxPen new_pen, old_pen;
};

class SetBrushCommand : public Command
{
public:
    SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape, wxBrush brush)
    : Command(start, shape), new_brush(brush)
    {
        old_brush.SetColour(shape->GetBrushColor());
    }
    
    void Undo(std::shared_ptr<PaintModel> model) override
    {
        mShape->SetBrush(old_brush);
    }
    
    void Redo(std::shared_ptr<PaintModel> model) override
    {
        mShape->SetBrush(new_brush);
    }
    
    void Finalize(std::shared_ptr<PaintModel> model) override {}
    
protected:
    wxBrush new_brush, old_brush;
};