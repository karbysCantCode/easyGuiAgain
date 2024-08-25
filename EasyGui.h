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

	vector2 Size = vector2();
	vector2 Position = vector2();
	bool Visible = true;

	SDL_Texture* Texture = nullptr;

	GME_Frame() {};
};

class GME_Button {
public:

	int ID = -1;
	int ParentID = -1;

	vector2 Size = vector2();
	vector2 Position = vector2();
	bool Visible = true;
	bool Interactable = true;

	SDL_Texture* Texture = nullptr;

	GME_Button() {};
};

enum GME_ObjectTypes {
	Frame,
	Button
};

using GME_GenericObject = std::variant<GME_Frame, GME_Button>;

class GME_ObjectManager {
private: 
	std::shared_ptr<GME_GenericObject> GetObjectPointer(int id) {
		auto it = AllObjects.find(id);
		if (it != AllObjects.end()) {
			return it->second;
		}
		return nullptr;
	}

public:

	SDL_Renderer* Renderer = nullptr;
	int ScreenX = 0;
	int ScreenY = 0;

	std::unordered_set<int> AllocatedIDs;
	std::unordered_set<int> FreedIDs;

	std::unordered_map<int, std::shared_ptr<GME_GenericObject>> AllObjects;

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

	// Returns the ID of the created object.
	template <typename T>
	T* Create() {
		const int id = GetID();
		std::shared_ptr<GME_GenericObject> object;

		object = T;




	}

};