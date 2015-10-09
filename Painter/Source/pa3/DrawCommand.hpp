//
//  DrawCommand.hpp
//  paint-mac
//
//  Created by Rui Zeng on 10/6/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "Command.h"

class DrawCommand : public Command
{
public:
    DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
    
    void Update(const wxPoint& newPoint) override;
    
    void Finalize(std::shared_ptr<PaintModel> model) override;
    
    void Undo(std::shared_ptr<PaintModel> model) override;
    
    void Redo(std::shared_ptr<PaintModel> model) override;

protected:
};
