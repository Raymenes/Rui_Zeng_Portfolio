//
//  DeleteCommand.h
//  paint-mac
//
//  Created by Rui Zeng on 10/7/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Command.h"

class DeleteCommand : public Command
{
public:
    DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
    : Command(start, shape)
    {}
    // Called when the command is still updating (such as in the process of drawing)
    void Update(const wxPoint& newPoint)
    {}
    // Called when the command is completed
    void Finalize(std::shared_ptr<PaintModel> model) override
    {
        model->RemoveShape(mShape);
    }
    // Used to "undo" the command
    void Undo(std::shared_ptr<PaintModel> model) override
    {
        model->AddShape(mShape);
    }
    // Used to "redo" the command
    void Redo(std::shared_ptr<PaintModel> model) override
    {
        model->RemoveShape(mShape);
    }
};
