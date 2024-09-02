#pragma once

#include "Vector2.h"
#include "SDL.h"

#include <variant>
#include <memory>
#include <functional>

#include "unordered_set"
#include "unordered_map"


class GME_Frame {
public:	

	int ID = -1;
	int ParentID = -1;

	vector2<int> Size;
	vector2<int> Position;
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

	vector2<int> Size;
	vector2<int> Position;
	SDL_Color Color = { 0,0,0,255 };
	bool Visible = true;
	bool Interactable = true;

	SDL_Texture* Texture = nullptr;

	GME_Button() {};

	~GME_Button() {
		if (Texture) {
			SDL_DestroyTexture(Texture);
		}
	}

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

	std::function<void()> ActivationFunction;
};

class GME_Test {
public:

	int ID = -1;
	int ParentID = -1;

	vector2<int> Size;
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
		const auto valuePointer = GetLiteralPointer<T>(AllObjects[id]);
		if (valuePointer->Texture != nullptr) {
			SDL_Texture* texture = valuePointer->Texture;
			SDL_Rect rect = {};
			rect.x = valuePointer->Position.x;
			rect.y = valuePointer->Position.y;
			rect.w = valuePointer->Size.x;
			rect.h = valuePointer->Size.y;
			SDL_RenderCopy(Renderer, texture, NULL, &rect);
		}
	}

	template <typename T>
	bool IsWithinRange(const T& value, const T& min, const T& max) {
		return (value >= min && value <= max);
	}

	bool CheckPositionInBox(const vector2<int> checkPosition, const vector2<int> objSize, const vector2<int> objPosition) {
		return IsWithinRange<int>(checkPosition.x, objPosition.x, objPosition.x + objSize.x) && IsWithinRange<int>(checkPosition.y, objPosition.y, objPosition.y + objSize.y);
	}

	vector2<int> CalculateSquare(vector2<int> checkPosition) {
		const double xPortion = ScreenX / ClickCells;
		const double yPortion = ScreenY / ClickCells;

		vector2<int> toReturn;
		toReturn.x = static_cast<int>(checkPosition.x / xPortion);
		toReturn.y = static_cast<int>(checkPosition.y / yPortion);
		return toReturn;
	}
public:

	SDL_Renderer* Renderer = nullptr;
	int ScreenX = 0;
	int ScreenY = 0;

	const int ClickCells = 10; // is X and Y, so n^2 is the total amount, the var is just the side witdh

	std::unordered_set<int> AllocatedIDs;
	std::unordered_set<int> FreedIDs;

	std::unordered_map<int, std::shared_ptr<GME_GenericObject>> AllObjects;
	std::unordered_map<int, GME_ObjectTypes> IDTypePairs;

	std::unordered_map<int, std::unordered_map<int, int>> ParentRecord;

	std::vector<std::unordered_map<int, std::shared_ptr<GME_GenericObject>>> ClickableHolders;

	GME_ObjectManager(SDL_Renderer* renderer, int screenX, int screenY) : Renderer(renderer), ScreenX(screenX), ScreenY(screenY) {
		// init ClickableHolders
		ClickableHolders.resize(ClickCells * ClickCells);
		for (int x = 0; x < ClickCells; ++x) {
			for (int y = 0; y < ClickCells; ++y) {
				ClickableHolders[x * ClickCells + y] = std::unordered_map<int, std::shared_ptr<GME_GenericObject>>();
			}
		}
	}


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

	void ProcessClick(const int mouseX, const int mouseY) {
		const vector2<int> cell = CalculateSquare(vector2<int>(mouseX, mouseY));

		std::cout << cell.x << ' ' << cell.y << '\n';

		for (auto& genericObject : ClickableHolders[cell.x * ClickCells + cell.y]) {
			switch (IDTypePairs[genericObject.first])
			{
			case Button:
				auto buttonPtr = GetLiteralPointer<GME_Button>(genericObject.second);
				if (buttonPtr->Interactable && CheckPositionInBox(vector2<int>(mouseX, mouseY), buttonPtr->Size, buttonPtr->Position)) {
					buttonPtr->ActivationFunction();
				}
				break;
			}
		}
	}

	void UpdateClickableSquares(const int objID, vector2<int> oldPosition, vector2<int> oldSize) {
			vector2<int> newSize, newPosition;
			switch (IDTypePairs[objID])
			{
			case Button: {
				auto objPtr = GetLiteralPointer<GME_Button>(AllObjects[objID]);
				newSize = objPtr->Size;
				newPosition = objPtr->Position;
			}
				break;
			default:
				break;
			}

			
			{
				const vector2<int> minCell = CalculateSquare(oldPosition);
				const vector2<int> maxCell = CalculateSquare(oldPosition + oldSize);
				std::cout << minCell.x << ':' << minCell.y << "  :  " << maxCell.x << ':' << maxCell.y << '\n';
				for (int x = minCell.x; x <= maxCell.x; ++x) {
					for (int y = minCell.y; y <= maxCell.y; ++y) {
						ClickableHolders[x * ClickCells + y].erase(objID);
					}
				}
			}
		

			{
				const vector2<int> minCell = CalculateSquare(newPosition);
				const vector2<int> maxCell = CalculateSquare(newPosition + newSize);
				std::cout << minCell.x << ':' << minCell.y << "  :  " << maxCell.x << ':' << maxCell.y << '\n';
				for (int x = minCell.x; x <= maxCell.x; ++x) {
					for (int y = minCell.y; y <= maxCell.y; ++y) {
						ClickableHolders[x * ClickCells + y].insert({ objID, AllObjects[objID] });
					}
				}
			}
		}
	// Returns the PTR of the created object.
	template <typename T>
	T* Create() {
		const int id = GetID();
		auto object = std::make_shared<GME_GenericObject>(T());

		AllObjects[id] = object;

		if constexpr (std::is_same_v<T, GME_Frame>) {
			IDTypePairs[id] = Frame;
		}
		else if constexpr (std::is_same_v<T, GME_Button>) {
			IDTypePairs[id] = Button;
		}
		else {
			// Handle other types or a default case
		}

		T* ptr = std::get_if<T>(&(*object));

		ptr->ID = id;

		return ptr;
	}



};