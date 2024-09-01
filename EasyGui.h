#pragma once

#include "Vector2.h"
#include "SDL.h"

#include <variant>
#include <memory>

#include "unordered_set"
#include "unordered_map"


class GME_Frame {
public:	

	int ID = -1;
	int ParentID = -1;

	vector2 Size;
	vector2 Position;
	SDL_Color Color = { 0,0,0,255 };
	bool Visible = true;

	SDL_Texture* Texture = nullptr;

	GME_Frame() {};

	~GME_Frame() {
		if (Texture) {
			SDL_DestroyTexture(Texture);
		}
	}

	void DisplayDebugData() {
		std::cout << ID << '\n';
		std::cout << ParentID << '\n';
		std::cout << Size.x << '\n';
		std::cout << Size.y << '\n';
		std::cout << Color.r << ' ' << Color.g << ' ' << Color.b << ' ' << Color.a << '\n';
		std::cout << Position.x << '\n';
		std::cout << Position.y << '\n';
		std::cout << Visible << '\n';
		std::cout << '\n';
	};

	void RefreshTexture(SDL_Renderer* renderer) {
		if (Texture != nullptr) {
			SDL_DestroyTexture(Texture);
		}
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, Size.x, Size.y, 32, SDL_PIXELFORMAT_RGBA8888);

		Uint32 color = SDL_MapRGBA(surface->format, Color.r, Color.g, Color.b, Color.a);
		SDL_FillRect(surface, nullptr, color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		Texture = texture;
	}
};

class GME_Button {
public:

	int ID = -1;
	int ParentID = -1;

	vector2 Size = vector2();
	vector2 Position = vector2();
	SDL_Color Color = { 0,0,0,255 };
	bool Visible = true;
	bool Interactable = true;

	SDL_Texture* Texture = nullptr;

	GME_Button() {};

	void DisplayDebugData() {
		std::cout << ID << '\n';
		std::cout << ParentID << '\n';
		std::cout << Size.x << '\n';
		std::cout << Size.y << '\n';
		std::cout << Position.x << '\n';
		std::cout << Position.y << '\n';
		std::cout << Visible << '\n';
		std::cout << '\n';
	};
};

class GME_Test {
public:

	int ID = -1;
	int ParentID = -1;

	vector2 Size = vector2();
	bool Visible = true;
	bool Interactable = true;

	GME_Test() {};
};

enum GME_ObjectTypes {
	Frame,
	Button
};

using GME_GenericObject = std::variant<GME_Frame, GME_Button, GME_Test>;

class GME_ObjectManager {
private: 
	template <typename T>
	T* GetLiteralPointer(std::shared_ptr<GME_GenericObject> genericPointer) {

		T* ptr = std::get_if<T>(&(*genericPointer));

		return ptr;
	}

	template <typename T>
	void RenderIndividualObject(const int id) {
		const auto valuePointer = GetValuePointer<T>(AllObjects[id]);
		SDL_Texture* texture = valuePointer->Texture;
		SDL_Rect rect = {};
		rect.x = valuePointer->Position.x;
		rect.y = valuePointer->Position.y;
		rect.w = valuePointer->Size.x;
		rect.h = valuePointer->Size.y;
		SDL_RenderCopy(Renderer, texture, NULL, &rect);
	}
public:

	SDL_Renderer* Renderer = nullptr;
	int ScreenX = 0;
	int ScreenY = 0;

	std::unordered_set<int> AllocatedIDs;
	std::unordered_set<int> FreedIDs;

	std::unordered_map<int, std::shared_ptr<GME_GenericObject>> AllObjects;
	std::unordered_map<int, GME_ObjectTypes> IDTypePairs;

	std::unordered_map<int, std::unordered_map<int, int>> ParentRecord;

	GME_ObjectManager(SDL_Renderer* renderer, int screenX, int screenY) : Renderer(renderer), ScreenX(screenX), ScreenY(screenY) {}


	int GetID() {
		// Use a new ID
		if (FreedIDs.empty()) {
			const int id = AllocatedIDs.size(); // Get the next ID
			AllocatedIDs.insert(id); // Insert it to allocated
			return id;

		}
		// Reuse an ID from FreedIDs
		const int id = *FreedIDs.begin();  // Get an iterator to the first element in FreedIDs
		FreedIDs.erase(FreedIDs.begin());  // Remove the ID from FreedIDs as it's now allocated
		AllocatedIDs.insert(id);     // Add the ID to AllocatedIDs
		return id;
	}

	void FreeID(const int ID) {
		if (AllocatedIDs.find(ID) != AllocatedIDs.end())
		AllocatedIDs.erase(ID);
		FreedIDs.insert(ID);
	}

	void RenderAll() {
		for (const auto object : AllObjects) {
			const int id = object.first;
			const GME_ObjectTypes type = IDTypePairs[id];

			switch (type)
			{
			case Frame:
				RenderIndividualObject<GME_Frame>(id);
				continue;
				break;
			case Button:
				RenderIndividualObject<GME_Button>(id);
				continue;
				break;
			default:
				continue;
				break;
			}
		}
	}

	// Returns the PTR of the created object.
	template <typename T>
	T* Create() {
		const int id = GetID();
		auto object = std::make_shared<GME_GenericObject>(T());

		AllObjects[id] = object;
		IDTypePairs[id] = Frame;

		T* ptr = std::get_if<T>(&(*object));

		ptr->ID = id;

		return ptr;
	}



};