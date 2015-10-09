#include "Renderer.h"
#include "Game.h"
#include "SpriteComponent.h"


Renderer::Renderer(Game& game)
	:mGame(game)
	,mWindow(nullptr)
	,mWidth(0)
	,mHeight(0)
{

}

Renderer::~Renderer()
{
	SDL_GL_DeleteContext(mContext);

	if (mWindow != nullptr)
	{
		SDL_DestroyWindow(mWindow);
	}
}

bool Renderer::Init(int width, int height)
{
	// Tell SDL we want to use OpenGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Request some GL parameters, just in case
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create our SDL window
	mWindow = SDL_CreateWindow("Lab 1: Asteroids 2D", 100, 100, width, height, 
		SDL_WINDOW_OPENGL);

	if (!mWindow)
	{
		SDL_Log("Could not create window.");
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);
	
	// Start up GLEW to bind OpenGL functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// Glew may still post an error, so clear it...
	glGetError();


	mWidth = width;
	mHeight = height;

	if (!InitFrameBuffer())
	{
		return false;
	}

	if (!InitShaders())
	{
		return false;
	}

	if (!InitSpriteVerts())
	{
		return false;
	}

	return true;
}

void Renderer::RenderFrame()
{
    Clear();
    DrawComponents();
    Present();
}

void Renderer::AddComponent(DrawComponentPtr component)
{
    if (IsA<SpriteComponent>(component) || IsA<FontComponent>(component)) {
        mDrawComponents2D.emplace(component);
    }
    else if (IsA<MeshComponent>(component))
    {
        mDrawComponents.emplace(component);
    }

}

void Renderer::RemoveComponent(DrawComponentPtr component)
{
    if (IsA<SpriteComponent>(component) || IsA<FontComponent>(component)) {
        auto iter = mDrawComponents2D.find(component);
        if (iter != mDrawComponents2D.end())
        {
            mDrawComponents2D.erase(component);
        }
    }
    else if (IsA<MeshComponent>(component))
    {
        auto iter = mDrawComponents.find(component);
        if (iter != mDrawComponents.end())
        {
            mDrawComponents.erase(component);
        }
    }
}

void Renderer::DrawSprite(TexturePtr texture, const Matrix4& worldTransform)
{
	mSpriteShader->SetActive();
	mSpriteShader->BindWorldTransform(worldTransform);
	mSpriteShader->UploadUniformsToGPU();

	mSpriteShader->BindTexture("uTexture", texture, 0);

	DrawVertexArray(mSpriteVerts);
}

void Renderer::DrawBasicMesh(VertexArrayPtr vertArray, TexturePtr texture, const Matrix4& worldTransform)
{
    // We want to draw with the basic mesh shader

    mBasicMeshShader->SetActive();
    // Save the value of the world transform we want to use
    mBasicMeshShader->BindWorldTransform(worldTransform);
    // Send the shader data (matrices, in this case) to the GPU
    mBasicMeshShader->UploadUniformsToGPU();
    // We need to specify which texture is active
    mBasicMeshShader->BindTexture("uTexture", texture, 0);
    // Now draw the triangles!
    DrawVertexArray(vertArray);
}

void Renderer::DrawVertexArray(VertexArrayPtr vertArray)
{
	vertArray->SetActive();
	glDrawElements(GL_TRIANGLES, vertArray->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear()
{
//    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawComponents()
{
	// TODO
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    for(auto drawComp : mDrawComponents){
        if(drawComp->IsVisible()){
            drawComp->Draw(*this);
        }
    }
    
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    
    for(auto drawComp : mDrawComponents2D){
        if(drawComp->IsVisible()){
            drawComp->Draw(*this);
        }
    }
    
    
}

void Renderer::Present()
{
    SDL_GL_SwapWindow(mWindow);
}

bool Renderer::InitFrameBuffer()
{
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

bool Renderer::InitShaders()
{
	// Load the sprite shader program
	mSpriteShader = mGame.GetAssetCache().Load<Shader>("Shaders/Sprite");
	if (!mSpriteShader)
	{
		SDL_Log("Failed to load sprite shader.");
		return false;
	}
    
    mBasicMeshShader = mGame.GetAssetCache().Load<Shader>("Shaders/BasicMesh");
    if (!mBasicMeshShader)
    {
        SDL_Log("Failed to load basic mesh shader.");
        return false;
    }

	mSpriteShader->SetActive();
    mBasicMeshShader->SetActive();

	// Create the view-projection for sprites and bind it to the shader
	// This is just an orthographic since the camera points at the center
	mSpriteViewProj = Matrix4::CreateOrtho(static_cast<float>(mWidth),
		static_cast<float>(mHeight), 1000.0f, -1000.0f);
	mSpriteShader->BindViewProjection(mSpriteViewProj);
    
    mViewProj = mSpriteViewProj;
    mBasicMeshShader->BindViewProjection(mSpriteViewProj);

	return true;
}

bool Renderer::InitSpriteVerts()
{
    //create the vertex array for sprites
    Vertex verts[] =
    {
        Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f), //top left
        Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f), //top right
        Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f), //bottom right
        Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f), // bottom left
    };
    
    GLuint indices[] =
    {
       0, 1, 2, // <top left, top right, bottom right>
       2, 3, 0,// <bottom right, bottom left, top left>
    };
    
    mSpriteVerts = VertexArray::Create(verts, 4, indices, 6);

	return true;
}
