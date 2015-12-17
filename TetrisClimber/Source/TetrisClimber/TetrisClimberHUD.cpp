// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "TetrisClimberHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "TetrisManager.h"
//#inclue "TetrisClimberCharacter.h"

ATetrisClimberHUD::ATetrisClimberHUD()
{
	//initialize font and displayed message
	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("Font'/Game/Fonts/OpenSans-Semibold'"));
	if (FontObject.Object)
	{
		mFont = FontObject.Object;
	}

	/*mRulesString = "Objective: Survive and reach the highest score you can.\nControls:\n";
	mRulesString+="Jump: Spacebar\nToggle Up: 1";
	*/
    // Set the crosshair texture
    static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
    CrosshairTex = CrosshiarTexObj.Object;
    //Game/Geometry/Green
    //static ConstructorHelpers::FObjectFinder<UTexture2D> FixedTetrominoTexObj (TEXT("/Game/Geometry/Green"));
    //FixedTetrominoTex = FixedTetrominoTexObj.Object;
    static ConstructorHelpers::FObjectFinder<UTexture2D> radarTexObj(TEXT("/Game/Geometry/BlackandWhiteRadar"));
    radarTex = radarTexObj.Object;
    
    static ConstructorHelpers::FObjectFinder<UTexture2D> fixedObj(TEXT("/Game/Geometry/Red"));
    FixedTetrominoTex = fixedObj.Object;
    
    static ConstructorHelpers::FObjectFinder<UTexture2D> greenTexObj (TEXT("/Game/Geometry/Green"));
    greenTex = greenTexObj.Object;

    static ConstructorHelpers::FObjectFinder<UTexture2D> blackTexObj (TEXT("/Game/Geometry/black"));
    blackTex = blackTexObj.Object;
    
    static ConstructorHelpers::FObjectFinder<UTexture2D> rgTexObj (TEXT("/Game/Geometry/greenRed"));
    greenRedTex = rgTexObj.Object;

    static ConstructorHelpers::FObjectFinder<UTexture2D> blueC (TEXT("/Game/Geometry/blueCircle"));
    blueCircleTex = blueC.Object;
    
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            fixed[i][j] = false;
        }
    }
    
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            moving[i][j] = false;
        }
    }

	    
}


void ATetrisClimberHUD::DrawHUD()
{
	//if(Super::DrawHud()){
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas

	
	if (!mDrawPauseMenu)
	{const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//store 3D coordinates
	CrossHairCenter = Center;
	CrossHairCenter.X -= 45;
	CrossHairCenter.Y -= 45;
	Get3DCrossHairPos();

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
		(Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

	FVector2D bottomRightCorner(Canvas->ClipX * 0.85f, Canvas->ClipY - (Canvas->ClipX * 0.13f));

	brc = bottomRightCorner;

	FCanvasTileItem radarBox(bottomRightCorner, radarTex->Resource, FLinearColor::White);
	Canvas->DrawItem(radarBox);


	for (int i = 0; i < 10; i++){

		for (int j = 0; j < 10; j++){

			if (fixed[i][j]){

				FVector2D position = FVector2D(15.0f * i, 15.0f * j);

				//FVector2D bottomRightCorner(Canvas->ClipX * 0.85f, Canvas->ClipY - ( Canvas->ClipX * 0.13f));
				position += brc;
				FCanvasTileItem redBox1(position, FixedTetrominoTex->Resource, FLinearColor::White);
				Canvas->DrawItem(redBox1);
			}

		}

	}


	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){

			if (moving[i][j] && fixed[i][j]){
				FVector2D position = FVector2D(15.0f * i, 15.0f * j);

				//FVector2D bottomRightCorner(Canvas->ClipX * 0.85f, Canvas->ClipY - ( Canvas->ClipX * 0.13f));
				position += brc;
				FCanvasTileItem rgBox1(position, greenRedTex->Resource, FLinearColor::White);
				Canvas->DrawItem(rgBox1);

			}

			else if (moving[i][j]){
				FVector2D position = FVector2D(15.0f * i, 15.0f * j);

				//FVector2D bottomRightCorner(Canvas->ClipX * 0.85f, Canvas->ClipY - ( Canvas->ClipX * 0.13f));
				position += brc;
				FCanvasTileItem greenBox1(position, greenTex->Resource, FLinearColor::White);
				Canvas->DrawItem(greenBox1);

			}
			else if (!(fixed[i][j])){

				FVector2D position = FVector2D(15.0f * i, 15.0f * j);

				//FVector2D bottomRightCorner(Canvas->ClipX * 0.85f, Canvas->ClipY - ( Canvas->ClipX * 0.13f));
				position += brc;
				FCanvasTileItem blackBox1(position, blackTex->Resource, FLinearColor::White);
				Canvas->DrawItem(blackBox1);

			}

		}

	}

	if (character){
		FVector2D position = ATetrisManager::get()->WorldTo2DGridPosition(character->GetActorLocation());
		position *= 15.0f;
		position += brc;
		FCanvasTileItem circle(position, blueCircleTex->Resource, FLinearColor::White);
		Canvas->DrawItem(circle);

	}
}
	if (mDrawPauseMenu)//drawn when game's paused
	{
		FString ObjectiveString = FString("Objective: Get the best score you can\n");
		FString RulesString = FString("Shoot at blocks to select. Then SHIFT or ROTATE them.\nToggling a direction affects rotation and movement.\nMake sure you don't line them up.");
		FString ControlsString;
		//RulesString = FString("Objective: Survive and reach the highest score you can.\nControls:\nJump: Spacebar\n");
		ControlsString += FString("Controls:\nJump: Spacebar\nToggle Left : 1\nToggle Right : 2\nToggle Up : 3\nToggle Down : 4\n");
		ControlsString += FString("Character Movement: Use WSAD\n");
		ControlsString += FString("First Person: F\nThird Person: T\n");
		ControlsString += FString("Move: Left Click\nRotate: Right Click\n");
		ControlsString += FString("Pause: P\n");
		ControlsString += FString("When block is selected:\nShift: Use WSAD\nRotate:Q,E\nDeselect:Spacebar\n");
		FText ControlsText = FText::FromString(ControlsString);
		FText ObjText = FText::FromString(ObjectiveString);
		FText RulesText = FText::FromString(RulesString);
		
		FLinearColor TheControlsFontColour = FLinearColor(1.0f, 1.0f, 1.0f);
		FLinearColor TheObjFontColour = FLinearColor(0.0f, 255.0f, 0.0f);
		FLinearColor TheRulesFontColour = FLinearColor(255.0f, 255.0f, 0.0f);

		FCanvasTextItem NewControlsText(FVector2D(10, 90), ControlsText, mFont, TheControlsFontColour);
		FCanvasTextItem NewRulesText(FVector2D(10, 25), RulesText, mFont, TheRulesFontColour);
		FCanvasTextItem NewObjText(FVector2D(10, 0), ObjText, mFont, TheObjFontColour);

		//Draw
		Canvas->DrawItem(NewObjText);
		Canvas->DrawItem(NewRulesText);
		Canvas->DrawItem(NewControlsText);

	}
	if(!mDrawPauseMenu)//display bullet status only if game is unpaused (screen gets kind of crowded otherwise)
	{
		//display score
		FText ScoreText =FText::FromString(FString("Score: ")+FString::SanitizeFloat(ATetrisManager::get()->getScore()));
		FLinearColor ScoreColour = FLinearColor(0.0f, 255.0f, 0.0f);
		FCanvasTextItem NewScoreText(FVector2D(40, 10), ScoreText, mFont, ScoreColour);
		Canvas->DrawItem(NewScoreText);

		FText NText = FText::FromString("N");
		FText SText = FText::FromString("S");
		FText EText = FText::FromString("E");
		FText WText = FText::FromString("W");

		FLinearColor NormalColour = FLinearColor(1.0f, 1.0f, 1.0f);
		FLinearColor SelectedColour = FLinearColor(255.0f, 0.0f, 0.0f);
		
		FCanvasTextItem NewNText(FVector2D(40, 270), NText, mFont, SelectedColour);
		FCanvasTextItem NewSText(FVector2D(40, 330), SText, mFont, NormalColour);
		FCanvasTextItem NewEText(FVector2D(70, 300), EText, mFont, NormalColour);
		FCanvasTextItem NewWText(FVector2D(10, 300), WText, mFont, NormalColour);

		if (mCurrentProjType == AWeapon::ProjectileType::UP)
		{
			NewNText.SetColor(SelectedColour);
			NewSText.SetColor(NormalColour);
			NewWText.SetColor(NormalColour);
			NewEText.SetColor(NormalColour);
		}

		else if (mCurrentProjType == AWeapon::ProjectileType::RIGHT)
		{
			NewEText.SetColor(SelectedColour);
			NewSText.SetColor(NormalColour);
			NewWText.SetColor(NormalColour);
			NewNText.SetColor(NormalColour);
		}

		else if (mCurrentProjType == AWeapon::ProjectileType::DOWN)
		{
			NewSText.SetColor(SelectedColour);
			NewNText.SetColor(NormalColour);
			NewWText.SetColor(NormalColour);
			NewEText.SetColor(NormalColour);
		}

		else//left
		{
			NewWText.SetColor(SelectedColour);
			NewSText.SetColor(NormalColour);
			NewNText.SetColor(NormalColour);
			NewEText.SetColor(NormalColour);
		}


		Canvas->DrawItem(NewNText);
		Canvas->DrawItem(NewSText);
		Canvas->DrawItem(NewEText);
		Canvas->DrawItem(NewWText);
	}
    
	if (mGameOver)
	{
		UFont* gameOverFont = mFont;
		gameOverFont->SetFontScalingFactor(5.0);
		if (mLost)
		{
			
			FString GameOverString = FString("You messed up.\n");

			FText GameOverText = FText::FromString(GameOverString);
			FText ResetText = FText::FromString(FString("Press Backspace to reset the game."));

			FLinearColor GameOverColour = FLinearColor(255.0f, 0.0f, 0.0f);


			FCanvasTextItem GameOverTextItem(FVector2D(300, 90), GameOverText, gameOverFont, GameOverColour);
			FCanvasTextItem ResetTextItem(FVector2D(300, 140), ResetText, gameOverFont, GameOverColour);

			//Draw
			Canvas->DrawItem(GameOverTextItem);
			//Canvas->DrawItem(ResetTextItem);
		}
		/*else//display win message
		{
			FString WinString = FString("You escaped KAP 160!\n");

			FText WinText = FText::FromString(WinString);
			FText ResetText = FText::FromString(FString("Press Backspace to reset the game."));

			FLinearColor WinColour = FLinearColor(255.0f, 0.0f, 0.0f);


			FCanvasTextItem WinTextItem(FVector2D(300, 90), WinText, gameOverFont, WinColour);
			FCanvasTextItem ResetTextItem(FVector2D(300, 140), ResetText, gameOverFont, WinColour);

			//Draw
			Canvas->DrawItem(WinTextItem);
			Canvas->DrawItem(ResetTextItem);
		}*/
	}

}

void ATetrisClimberHUD::Get3DCrossHairPos()
{
	//Always Check Pointers
	if (!Canvas) return;

	FVector WorldDirectionOfCrossHair2D;
	Canvas->Deproject(CrossHairCenter, CrossHair3DPos, WorldDirectionOfCrossHair2D);

	//moves the cursor pos into full 3D, your chosen distance ahead of player
	CrossHair3DPos += WorldDirectionOfCrossHair2D * mAimDistance;
}


void ATetrisClimberHUD::SetFixedTetromino(int32 x, int32 y){
    
    fixed[x][y] = true;
    
}

void ATetrisClimberHUD::RemoveFixedTetromino(int32 x, int32 y){
    
    fixed[x][y] = false;
    
}

void ATetrisClimberHUD::SetMovingTetromino(int32 x, int32 y){
    moving[x][y] = true;
    
    
}

void ATetrisClimberHUD::RemoveMovingTetromino(int32 x, int32 y){
    
    moving[x][y] = false; 
    
}








