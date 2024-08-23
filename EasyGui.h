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



class ScreenGui {
private:
	static SDL_Renderer* Renderer;
	int ScreenX = 0;
	int ScreenY = 0;
public:

	ScreenGui(SDL_Renderer* renderer, const int screenX, const int screenY) {
		Renderer = renderer;
		ScreenX = screenX;
		ScreenY = screenY;
	}

// DATA CONTAINERS 1/2

	// CHILDREN CONTAINER

	// FIRST: Parent IDs, SECOND: Children IDs
	static std::unordered_map<int, std::unordered_set<int>> ChildrenContainer;

	// POSITON CONTAINER
	static std::unordered_map<int, vector2> PositionContainer;

	// SIZE CONTAINER
	static std::unordered_map<int, vector2> SizeContainer;

	// VISIBLE CONTAINER
	static std::unordered_map<int, bool> VisibleContainer;

	// INTERACTABLE CONTAINER
	static std::unordered_map<int, bool> InteractableContainer;

	// COLOR CONTAINER
	static std::unordered_map<int, SDL_Color> ColorContainer;

	// TEXTURE CONTAINER
	static std::unordered_map<int, SDL_Texture*> TextureContainer;

// ID MANAGEMENT

	// ALLOCATED IDS
	static std::unordered_set<int> AllocatedIDContainer;
	// FREED IDS
	static std::unordered_set<int> FreedIDContainer;

	// GENERATE/DELETE ID
private:
	// Allocates an ID and returns that ID.
	static int GenerateId() {
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

	// Frees an allocated ID.
	// 1 = success.
	// 0 = not freed.
	static int FreeID(const int ID) {
		if (FreedIDContainer.find(ID) == FreedIDContainer.end() &&
			AllocatedIDContainer.find(ID) != AllocatedIDContainer.end())
		{
			AllocatedIDContainer.erase(ID);
			FreedIDContainer.insert(ID);
			return 1;

		}

		return 0;
	}
public:

// GME_OBJECTDATA

	static class GME_ObjectData {
	public:
		// PROPERTIES
		int ObjectID;
		GME_ObjectTypes Type;
		int Parent = -1;
		bool Visible;

		// INITIALIZER

		GME_ObjectData(int objID = GenerateId(), GME_ObjectTypes type = Frame, int visible = 1) : ObjectID(objID), Type(type), Visible(visible) {}

		// UPDATE POSITION

		int UpdatePosition(const vector2 newPosition) {
			if (PositionContainer.find(ObjectID) != PositionContainer.end()) {
				PositionContainer.find(ObjectID)->second = newPosition;

				return 0;
			}
			return 1;
		}

		// UPDATE SIZE

	// This function changes the size of an object. It handles updating the texture so there is no need to call "UpdateTexture()" manually.
	// 0 = success.
	// 1 = object is not in SizeContainer.
	// 2 = texture failed to update.
		int UpdateSize(const vector2 newSize) {
			if (SizeContainer.find(ObjectID) != SizeContainer.end()) {
				SizeContainer.find(ObjectID)->second = newSize;

				const int success = UpdateTextureInt();
				if (!success) {
					return 2;
				}
				return 0;
			}
			return 1;
		}

		// UPDATE TEXTURE

		// The "UpdateTexture" function removes any existing texture for an object ID, creates a new texture, and returns it.
		SDL_Texture* UpdateTexturePTR() {
			if (TextureContainer.find(ObjectID) != TextureContainer.end()) { // remove any preexisitng textures for that obj id
				TextureContainer.erase(TextureContainer.find(ObjectID));
			}
			const vector2 size = SizeContainer.find(ObjectID)->second;
			SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);

			const SDL_Color sdlColor = ColorContainer.find(ObjectID)->second;
			Uint32 color = SDL_MapRGBA(surface->format, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
			SDL_FillRect(surface, nullptr, color);

			// future integration for rounding edges here!

			SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
			TextureContainer.insert({ ObjectID, texture });
			return texture;
		}

		// Returns 0 on success.
		int UpdateTextureInt() {
			if (TextureContainer.find(ObjectID) != TextureContainer.end()) { // remove any preexisitng textures for that obj id
				TextureContainer.erase(TextureContainer.find(ObjectID));
			}
			const vector2 size = SizeContainer.find(ObjectID)->second;
			SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);

			const SDL_Color sdlColor = ColorContainer.find(ObjectID)->second;
			Uint32 color = SDL_MapRGBA(surface->format, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
			SDL_FillRect(surface, nullptr, color);

			// future integration for rounding edges here!

			SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
			TextureContainer.insert({ ObjectID, texture });
			return 0;
		}

		// UPDATE POSITION

		int UpdatePosition(const vector2 newPosition) {
			if (PositionContainer.find(ObjectID) != PositionContainer.end()) {
				PositionContainer.find(ObjectID)->second = newPosition;

				return 0;
			}
			return 1;
		}

		//UPDATE COLOR

		int UpdateColor(const SDL_Color newColor) {
			if (ColorContainer.find(ObjectID) != ColorContainer.end()) {
				ColorContainer.find(ObjectID)->second = newColor;

				return 0;
			}
			return 1;
		}

		// UPDATE VISIBILITY

		int UpdateVisibility(const bool newVisiblity) {
			if (VisibleContainer.find(ObjectID) != VisibleContainer.end()) {
				VisibleContainer.find(ObjectID)->second = newVisiblity;

				return 0;
			}
			return 1;
		}

		// UPDATE INTERACTIBILITY

		int UpdateInteractability(const bool newInteractability) {
			if (InteractableContainer.find(ObjectID) != InteractableContainer.end()) {
				InteractableContainer.find(ObjectID)->second = newInteractability;

				return 0;
			}
			return 1;
		}

		int DestroyObject(const int objectID) {
			if (AllDescendantsContainer.find(objectID) != AllDescendantsContainer.end()) {
				switch (AllDescendantsContainer[objectID]->Type)
				{
				case Frame:
					DestroyFrame(objectID);
					break;
				case Button:
					DestroyButton(objectID);
					break;
				default:
					break;
				}

				OrphanObject(objectID);
				DestroyChildren(objectID);
				DestroyChildrenContainer(objectID);
				FreeID(objectID);
			}
		}
	};

// DATA CONTAINERS 2/2

	// ALL DESCENDANTS
	static std::unordered_map<int, std::shared_ptr<GME_ObjectData>> AllDescendantsContainer;

// OBJECT CREATION

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
			break;
		case Button:
			InitiateButton(ID);
			break;
		default:
			break;
		}

		return objPointer;
	}

// INITIATE OBJECTS

	// INITIATE FRAME
	
	// Returns 0 on success.
	static int InitiateFrame(const int ID) {
		PositionContainer.insert({ ID, {0,0} });
		SizeContainer.insert({ ID, {0,0} });
		VisibleContainer.insert({ ID, 1 });
		ColorContainer.insert({ ID, { 0,0,0,255 } });
		TextureContainer.insert({ ID, nullptr });

		return 0;
	}

	// INITIATE BUTTON

	// Returns 0 on success.
	static int InitiateButton(const int ID) {
		PositionContainer.insert({ ID, {0,0} });
		SizeContainer.insert({ ID, {0,0} });
		VisibleContainer.insert({ ID, 1 });
		ColorContainer.insert({ ID, { 0,0,0,255 } });
		TextureContainer.insert({ ID, nullptr });
		InteractableContainer.insert({ ID, 1 });

		return 0;
	}

// OBJECT MODIFICATION

	// PARENT AND ORPHAN ZONE

	// VERIFY CHILD CONTAINER

	// Verifies that a entry has been created for an ID, if not, it creates an entry.
	// 1 = exists.
	// 0 = does not exist.
	static bool VerifyChildContainer(const int parentID) {
		if (ChildrenContainer.find(parentID) != ChildrenContainer.end()) {
			return 1;
		}
		return 0;
	}

	// CREATE CHILD CONTAINER
	
	// Creates an entry in ChildrenContainer for the parentID if an entry doesn't already exist.
	static int CreateChildContainer(const int parentID) {
		if (!VerifyChildContainer(parentID)) {
			ChildrenContainer.insert({ parentID,std::unordered_set<int>() });
		}
		return 0;
	}

	// PARENT AN OBJECT

	// Makes the parentID the parent of the childID
	// 1 = failed
	// 0 = success
	static int ParentObject(const int parentID, const int childID) {
		CreateChildContainer(parentID);
		ChildrenContainer[parentID].insert(childID);
		AllDescendantsContainer[childID]->Parent = parentID;
		return 0;
	}

	// ORPHAN OBJECT
	
	// Removes an object from its parents childrencontainer and sets the parentID to -1 (default parentID)
	// 0 = success (Is now orphaned, could have been already).
	static int OrphanObject(const int objectID) {
		auto objectParent = AllDescendantsContainer[objectID]->Parent;
		if (objectParent != -1) {
			ChildrenContainer[objectParent].erase(objectID);

			objectParent = -1;
		}
		return 0;
	}

// OBJECT DESTRUCTION

	// DESTROY CHILDRENCONTAINER

	// Destroys the given ID's child container
	// 0 = success (it may not have existed in the first place, but it doesn't exist now.)
	static int DestroyChildrenContainer(const int parentID) {
		if (VerifyChildContainer(parentID)) {
			ChildrenContainer.erase(parentID);
		}
		return 0;
	}

	// DESTROY CHILDREN
	
	// Destroys all children of parentID
	// 0 = success.
	static int DestroyChildren(const int parentID) {
		if (VerifyChildContainer(parentID)) {
			for (auto currentID : ChildrenContainer[parentID]) {
				DestroyObject(currentID);
			}
		}

		return 0;
	}

	// DESTROY FRAME

	// Removes all data that belongs to the frame from all lists it is ever entered into (besides parent/child, this is managed in "DestroyObject")
	static int DestroyFrame(const int objectID) {
		PositionContainer.erase(objectID);
		SizeContainer.erase(objectID);
		VisibleContainer.erase(objectID);
		ColorContainer.erase(objectID);
		TextureContainer.erase(objectID);
		return 0;
	}

	// DESTROY BUTTON
	
	// Removes all data that belongs to the button from all lists it is ever entered into (besides parent/child, this is managed in "DestroyObject")
	static int DestroyButton(const int objectID) {
		PositionContainer.erase(objectID);
		SizeContainer.erase(objectID);
		VisibleContainer.erase(objectID);
		InteractableContainer.erase(objectID);
		ColorContainer.erase(objectID);
		TextureContainer.erase(objectID);
		return 0;
	}

	// DESTROY OBJECT

	// Removes all data relevant to the given object and frees the ID.
	// Destroys all descendants of the object aswell.
	static int DestroyObject(const int objectID) {
		if (AllDescendantsContainer.find(objectID) != AllDescendantsContainer.end()) {
			switch (AllDescendantsContainer[objectID]->Type)
			{
			case Frame:
				DestroyFrame(objectID);
				break;
			case Button:
				DestroyButton(objectID);
				break;
			default:
				break;
			}

			OrphanObject(objectID);
			DestroyChildren(objectID);
			DestroyChildrenContainer(objectID);
			FreeID(objectID);
		}
	}

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

		return 0;
	}
};