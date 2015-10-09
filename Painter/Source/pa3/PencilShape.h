//
//  PencilShape.h
//  paint-mac
//
//  Created by Rui Zeng on 10/6/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include "Shape.h"
#include <vector>

class PencilShape : public Shape
{
public:
    PencilShape(const wxPoint& start)
    : Shape(start)
    {
        mPoints.push_back(start);
    }
    
    void Draw(wxDC& dc) const override
    {
        dc.SetPen(mPen);
        dc.SetBrush(mBrush);
        
        //if pencil only draws 1 point
        if(mPoints.size() == 1){
            dc.DrawPoint(mPoints.at(0));
        }
        else{
            dc.DrawLines((int)mPoints.size(), &mPoints[0], mOffset.x, mOffset.y);
        }
    }
    
    void Update(const wxPoint& newPoint) override
    {
        Shape::Update(newPoint);
        mPoints.push_back(newPoint);
    }
    
    void Finalize() override
    {
        //figuer out what is the actual topLeft and botRight points of the pencil drawing
        //by looping thru every points
        int leftX = mStartPoint.x;
        int rightX = mStartPoint.x;
        int topY = mStartPoint.y;
        int botY = mStartPoint.y;
        for(auto& p : mPoints){
            if(p.x > rightX)
                rightX = p.x;
            if(p.x < leftX)
                leftX = p.x;
            if(p.y > botY)
                botY = p.y;
            if(p.y < topY)
                topY = p.y;
        }
        mTopLeft = wxPoint(leftX, topY);
        mBotRight = wxPoint(rightX, botY);
    }
private:
    std::vector<wxPoint> mPoints;
};