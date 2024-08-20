#pragma once

#include "SDL.h"
#include "unordered_map"
#include "unordered_set"
#include "Vector2.h"


//  _   _          __  __ _____ _   _  _____        _____ ____  _   ___      ________ _   _ _______ _____ ____  _   _  _____ 
// | \ | |   /\   |  \/  |_   _| \ | |/ ____|      / ____/ __ \| \ | \ \    / /  ____| \ | |__   __|_   _/ __ \| \ | |/ ____|
// |  \| |  /  \  | \  / | | | |  \| | |  __      | |   | |  | |  \| |\ \  / /| |__  |  \| |  | |    | || |  | |  \| | (___  
// | . ` | / /\ \ | |\/| | | | | . ` | | |_ |     | |   | |  | | . ` | \ \/ / |  __| | . ` |  | |    | || |  | | . ` |\___ \ 
// | |\  |/ ____ \| |  | |_| |_| |\  | |__| |     | |___| |__| | |\  |  \  /  | |____| |\  |  | |   _| || |__| | |\  |____) |
// |_| \_/_/    \_\_|  |_|_____|_| \_|\_____|      \_____\____/|_| \_|   \/   |______|_| \_|  |_|  |_____\____/|_| \_|_____/ 
                                                                                                                      
/*
    PROPERTIES OF ANY OBJECT = PascalCase
	FUNCTION NAMES = PascalCase
	TEMPORARY VARIABLES = camelCase









*/

enum GME_ObjectTypes {
	Frame,
	Button,
	UIRounder
};

struct GME_ObjectData {
	int ObjectID;
	GME_ObjectTypes Type;
	bool Visible;
};



class ScreenGui {
private:
	SDL_Renderer* Renderer = nullptr;
	int ScreenX = 0;
	int ScreenY = 0;
public:

	ScreenGui(SDL_Renderer* renderer, const int screenX, const int screenY) {
		Renderer = renderer;
		ScreenX = screenX;
		ScreenY = screenY;
	}

// DATA CONTAINERS
	// ALL DESCENDANTS
	std::unordered_map<int, std::shared_ptr<GME_ObjectData>> AllDescendantsContainer;

	// POSITON CONTAINER
	std::unordered_map<int, vector2> PositionContainer;

	// SIZE CONTAINER
	std::unordered_map<int, vector2> SizeContainer;

	// VISIBLE CONTAINER
	std::unordered_map<int, bool> VisibleContainer;

	// COLOR CONTAINER
	std::unordered_map<int, SDL_Color> ColorContainer;

	// TEXTURE CONTAINER
	std::unordered_map<int, SDL_Texture*> TextureContainer;

	// ALLOCATED IDS
	std::unordered_set<int> AllocatedIDContainer;
	// FREED IDS
	std::unordered_set<int> FreedIDContainer;

// OBJECT CREATION

// GENERATE ID
private:
	// Allocates an ID and returns that ID.
	int GenerateId() {
		if (!FreedIDContainer.empty()) {
			auto it = FreedIDContainer.begin();
			const int id = *it;
			FreedIDContainer.erase(it);
			AllocatedIDContainer.insert(id);
			return id;
		}

		const int id = AllocatedIDContainer.size();
		AllocatedIDContainer.insert(id);
		return id;
	}
public:

// CREATE OBJECT!
	
	// Creates an object and returns the ID of the object.
	int CreateObjectWithID(GME_ObjectTypes type) {
		const int ID = GenerateId();
		
		std::shared_ptr<GME_ObjectData> objPointer = std::make_shared<GME_ObjectData>();
		objPointer->ObjectID = ID;
		objPointer->Type = type;

		switch (type) {

		case Frame:
			InitiateFrame(ID);
		}

		return ID;
	}
	// Creates an object and returns the shared_ptr of the object.
	std::shared_ptr<GME_ObjectData> CreateObjectWithPTR(GME_ObjectTypes type) {
		const int ID = GenerateId();

		std::shared_ptr<GME_ObjectData> objPointer = std::make_shared<GME_ObjectData>();
		objPointer->ObjectID = ID;
		objPointer->Type = type;

		switch (type) {

		case Frame:
			InitiateFrame(ID);
		}

		return objPointer;
	}

// INITIATE OBJECTS

	// INITIATE FRAME
	
	// Returns 0 on success.
	int InitiateFrame(const int ID) {
		PositionContainer.insert({ ID, {0,0} });
		SizeContainer.insert({ ID, {0,0} });
		VisibleContainer.insert({ ID, 1 });
		ColorContainer.insert({ ID, { 0,0,0,255 } });
		TextureContainer.insert({ ID, nullptr });

		return 0;
	}

// OBJECT MODIFICATION

	// UPDATE SIZE

	// This function changes the size of an object. It handles updating the texture so there is no need to call "UpdateTexture()" manually.
	// 0 = success.
	// 1 = object is not in SizeContainer.
	// 2 = texture failed to update.
	int UpdateSize(const int objectID, const vector2 newSize) {
		if (SizeContainer.find(objectID) != SizeContainer.end()) {
			vector2 oldSize = SizeContainer.find(objectID)->second;
			oldSize = newSize;
			const int success = UpdateTextureInt(objectID);
			if (!success) {
				return 2;
			}
			return 0;
		}
		return 1;
	}

	// UPDATE TEXTURE

	// The "UpdateTexture" function removes any existing texture for an object ID, creates a new texture, and returns it.
	SDL_Texture* UpdateTexturePTR(const int objectID) {
		if (TextureContainer.find(objectID) != TextureContainer.end()) { // remove any preexisitng textures for that obj id
			TextureContainer.erase(TextureContainer.find(objectID));
		}
		const vector2 size = SizeContainer.find(objectID)->second;
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);

		const SDL_Color sdlColor = ColorContainer.find(objectID)->second;
		Uint32 color = SDL_MapRGBA(surface->format, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
		SDL_FillRect(surface, nullptr, color);

		// future integration for rounding edges here!

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
		TextureContainer.insert({ objectID, texture });
		return texture;
	}
	
	// Returns 0 on success.
	int UpdateTextureInt(const int objectID) {
		if (TextureContainer.find(objectID) != TextureContainer.end()) { // remove any preexisitng textures for that obj id
			TextureContainer.erase(TextureContainer.find(objectID));
		}
		const vector2 size = SizeContainer.find(objectID)->second;
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);

		const SDL_Color sdlColor = ColorContainer.find(objectID)->second;
		Uint32 color = SDL_MapRGBA(surface->format, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
		SDL_FillRect(surface, nullptr, color);

		// future integration for rounding edges here!

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
		TextureContainer.insert({ objectID, texture });
		return 0;
	}

// OBJECT DESTRUCTION

	// 

// RENDER OBJECTS!

	int RenderObjects() {
		for (const auto currentPair : AllDescendantsContainer) {
			if (currentPair.second->Visible) {
				const int id = currentPair.first;
				SDL_Rect* rect;
				rect->x = PositionContainer[id].x;
				rect->y = PositionContainer[id].y;
				rect->w = SizeContainer[id].x;
				rect->h = SizeContainer[id].y;
				SDL_RenderCopy(Renderer, TextureContainer[id], nullptr, rect);
			}
		}
	}
};