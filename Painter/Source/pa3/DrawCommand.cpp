//
//  DrawCommand.cpp
//  paint-mac
//
//  Created by Rui Zeng on 10/6/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "DrawCommand.hpp"
#include "Shape.h"
#include "PaintModel.h"

DrawCommand::DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
: Command(start, shape)
{

}

void DrawCommand::Update(const wxPoint& newPoint)
{
    Command::Update(newPoint);
    mShape->Update(newPoint);
}

void DrawCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
}

void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(mShape);
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->AddShape(mShape);
}
