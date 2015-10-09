//
//  MoveCommand.h
//  paint-mac
//
//  Created by Rui Zeng on 10/7/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Command.h"

class MoveCommand : public Command
{
public:
    
    MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
    :Command(start, shape), originOffset(mShape->GetOffset())
    {}
    // Called when the command is still updating (such as in the process of drawing)
    void Update(const wxPoint& newPoint)
    {
        wxPoint offset(newPoint.x-mStartPoint.x,newPoint.y-mStartPoint.y) ;
        wxPoint finalOffset(originOffset.x + offset.x, originOffset.y + offset.y);
        mShape->SetOffset(finalOffset);
    }
    // Called when the command is completed
    void Finalize(std::shared_ptr<PaintModel> model) override
    {
        mShape->Finalize();
    }
    // Used to "undo" the command
    void Undo(std::shared_ptr<PaintModel> model) override
    {
        wxPoint offset = mStartPoint - mEndPoint;
        wxPoint finalOffset = originOffset + offset;
        mShape->SetOffset(finalOffset);
    }
    // Used to "redo" the command
    void Redo(std::shared_ptr<PaintModel> model) override
    {
        wxPoint offset = mEndPoint - mStartPoint;
        wxPoint finalOffset = originOffset + offset;
        mShape->SetOffset(finalOffset);
    }
    
private:
    wxPoint originOffset;
    
};
