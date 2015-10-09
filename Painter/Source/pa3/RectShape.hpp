//
//  RectShape.hpp
//  paint-mac
//
//  Created by Rui Zeng on 10/6/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "Shape.h"

class RectShape : public Shape
{
public:
    RectShape(const wxPoint& start)
    :Shape(start)
    {}
    
    void Draw(wxDC& dc) const override
    {
        dc.SetPen(mPen);
        dc.SetBrush(mBrush);
        dc.DrawRectangle(wxRect(mTopLeft+mOffset, mBotRight+mOffset));
    }
    
protected:
    
};