#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"
#include "RectShape.hpp"
#include "DrawCommand.hpp"
#include "EllipseShape.hpp"
#include "LineShape.h"
#include "PencilShape.h"
#include "SetPenCommand.h"
#include "DeleteCommand.h"
#include "MoveCommand.h"


Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;
    std::shared_ptr<Shape> shape;
    
    if (type == CM_DrawRect) {
        shape = std::make_shared<RectShape>(start);
        retVal = std::make_shared<DrawCommand>(start, shape);
        model->AddShape(shape);
    }
    else if (type == CM_DrawEllipse){
        shape = std::make_shared<EllipseShape>(start);
        retVal = std::make_shared<DrawCommand>(start, shape);
        model->AddShape(shape);
    }
    else if (type == CM_DrawLine){
        shape = std::make_shared<LineShape>(start);
        retVal = std::make_shared<DrawCommand>(start, shape);
        model->AddShape(shape);
    }
    else if (type == CM_DrawPencil){
        shape = std::make_shared<PencilShape>(start);
        retVal = std::make_shared<DrawCommand>(start, shape);
        model->AddShape(shape);
    }
    else if(type == CM_SetPen){
        shape = model->GetSelected();
        retVal = std::make_shared<SetPenCommand>(start, shape, model->GetPen());
    }
    else if(type == CM_SetBrush){
        shape = model->GetSelected();
        retVal = std::make_shared<SetBrushCommand>(start, shape, model->GetBrush());
    }
    
    
    if(shape != nullptr){
        shape->SetPen(model->GetPen());
        shape->SetBrush(model->GetBrush());
    }
    
    //put it underneath so delete and move doesn't change the shape to current pen and brush
    if (type == CM_Delete) {
        shape = model->GetSelected();
        retVal = std::make_shared<DeleteCommand>(start, shape);
    }
    else if (type == CM_Move)
    {
        shape = model->GetSelected();
        retVal = std::make_shared<MoveCommand>(start, shape);
    }
    
    
    
	return retVal;
}

