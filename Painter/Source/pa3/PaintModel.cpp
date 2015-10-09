#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>


PaintModel::PaintModel()
{
    InitPenBrush();
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
    if (mImportBitmap.IsOk()) {
        dc.DrawBitmap(mImportBitmap, 0, 0);
    }
    
    for(auto& s : mShapes){
        s->Draw(dc);
    }
    
    if (mSelected != nullptr) {
        mSelected->DrawSelection(dc);
    }
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
    if (mImportBitmap.IsOk()) {
        mImportBitmap = wxBitmap();
    }
    for (auto& s : mShapes){
        s = nullptr;
    }
    mShapes.clear();
    
    mCommand = nullptr;
    
    while (!mRedoStack.empty()) {
        mRedoStack.pop();
    }
    while (!mUndoStack.empty()) {
        mUndoStack.pop();
    }
    
    InitPenBrush();
    ClearSelect();
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
	}
}

bool PaintModel::HasActiveCommand()
{
    return mCommand != nullptr;
};

void PaintModel::CreateCommand(CommandType type, const wxPoint &start)
{
    mCommand = CommandFactory::Create(shared_from_this(), type, start);
};

void PaintModel::UpdateCommand(const wxPoint& updatePoint)
{
    mCommand->Update(updatePoint);
}

//finish the current command and set active command to false
void PaintModel::FinalizeCommand()
{
    mCommand->Finalize(shared_from_this());
    mUndoStack.push(mCommand);
    while (!mRedoStack.empty()) {
        mRedoStack.pop();
    }
    mCommand = nullptr;
}

bool PaintModel::CanRedo(){ return !(mRedoStack.empty()); }

bool PaintModel::CanUndo(){ return !(mUndoStack.empty()); }

void PaintModel::Redo()
{
    auto command = mRedoStack.top();
    mRedoStack.pop();
    command->Redo(shared_from_this());
    mUndoStack.push(command);
}

void PaintModel::Undo()
{
    auto command = mUndoStack.top();
    mUndoStack.pop();
    command->Undo(shared_from_this());
    mRedoStack.push(command);
}

void PaintModel::LoadFile(std::string filename, wxBitmapType type)
{
    New();
    mImportBitmap.LoadFile(filename, type);
}

void PaintModel::SaveFile(std::string filename, wxSize bit_map_size, wxBitmapType type)
{
    wxBitmap bitmap;
    //Create the bitmap of the specified wxSize
    bitmap.Create(bit_map_size);
    wxMemoryDC dc(bitmap);
    //set background to white
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    //choose not to draw the selection box
    auto tempSelected = mSelected;
    mSelected = nullptr;
    DrawShapes(dc);
    mSelected = tempSelected;
    
    bitmap.SaveFile(filename, type);
}
