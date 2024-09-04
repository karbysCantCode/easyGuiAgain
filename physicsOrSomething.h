#pragma once
#include "Vector2.h"

#include <unordered_map>


class WorldPhysicsManager {
public:
	vector2<int> ScreenSize;

	double gravity = 9.807;

	class PhysicsObject2d {
	public:
		vector2<double> Size;
		vector2<double> Position;
		vector2<double> Velocity;
		
		int ID;
		int CollisionGroup;
		int CanCollide;
		int Mass;

		vector2<int> GetPixelPosition(const vector2<int>& screenSize) {
			return vector2<int>(Position.x, screenSize.y - Position.y);
		}

		void StepPosition(const double& gravityVelocity) {
			Velocity.y += gravityVelocity;
			Position -= Velocity;
			if (Position.y - (Size.y/2) <= 1) {
				Velocity.y = 0;
				Position.y = Size.y/2;
			}

			std::cout << Position.x << ':' << Position.y << "  :  " << Velocity.x << ':' << Velocity.y << '\n';
			

		}
	};

	std::unordered_map<int, std::shared_ptr<PhysicsObject2d>> AllObjects;

	void StepPhysics(const int& tickRate) {
		const double gravityVelocity = gravity / tickRate;
		for (auto& object : AllObjects) {
			object.second->StepPosition(gravityVelocity);
		}
	}

	std::shared_ptr<PhysicsObject2d> Create(const int& ID) {
		auto obj = std::make_shared<PhysicsObject2d>();
		obj->ID = ID;
		AllObjects.insert({ ID, obj });
		return obj;
	}
};