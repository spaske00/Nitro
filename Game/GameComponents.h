#pragma once

namespace Nitro
{

	enum class TileType
	{
		road = 0,
		grass,
		water,

		water_deep,
		
		grass_beach_beach,
		beach_beach_grass,
		
		water_water_beach,
		beach_water_water,
		park1,
		park2,
		beach_trees_grass,
		TileTypeCount
	};

	inline const char* TileTypeToTextureName(TileType t)
	{
		switch(t)
		{
		case TileType::road: return "road";
		case TileType::grass: return "grass";
		case TileType::water: return "water";
			
		case TileType::grass_beach_beach: return "grass_beach_beach";
		case TileType::beach_beach_grass: return "beach_beach_grass";
			
		case TileType::water_water_beach: return "water_water_beach";
		case TileType::beach_water_water: return "beach_water_water";
		case TileType::park1: return "park1";
		case TileType::park2: return "park2";
		case TileType::beach_trees_grass: return "beach_trees_grass";


		case TileType::water_deep: return "water_deep";

		default:
			ASSERT(false, "Unkonwn tile type");
			return "";
		}
	}
	
	
	enum class PlayerTag
	{
		One = 1,
		Two = 2
	};

	inline int
	PlayerTagToInt(PlayerTag p)
	{
		return static_cast<int>(p);
	}

	inline PlayerTag
	PlayerTagFromInt(int p)
	{
		ASSERT(p == 1 || p == 2, "int PLayerTag value must be 1 or 2");
		return static_cast<PlayerTag>(p);
	}

	
	
	struct PlayerTagComponent : public Engine::Component
	{
		PlayerTag m_PlayerTag;
		explicit PlayerTagComponent(PlayerTag playerTag_)
			: m_PlayerTag(playerTag_)
		{
		}
	};


	struct CarPhysicsComponent : public Engine::Component
	{
		float m_Acceleration{};
		float m_CarSpeed{};
		float m_SteerAngle{};
		float m_CarHeading{};
		float m_WheelBase{};
		float m_BreakSpeed{};
		float m_Drag{};
		float m_MaxSpeed{};
		float m_MinSpeed{};
		float m_SteerSpeed{};

	};

	struct TileInfoComponent : public Engine::Component
	{
		TileType m_TileType;

		explicit TileInfoComponent(TileType tileType_)
			: m_TileType(tileType_)
		{
		}
	};

	struct TrackComponent : public Engine::Component
	{
		// TODO(Marko): Switch to shared_ptr if necessary
		Engine::Matrix<Engine::Entity*> m_TracksMatrix;
		vec2 m_TileSize{};
		vec2 m_UpperLeftCornerWorldPosition{};
		int m_LowestLayerIndex{};
		int m_TrackLeftColumnBoundaryBegin{};
		int m_TrackRightColumnBoundaryEnd{};
		
	};

	enum class TextInfoType {
		Kmh,
		Speed,
		Distance
	};

	struct TextInfoComponent : public Engine::Component
	{
		PlayerTag m_PlayerTag;
		//Pointer to player
		Engine::Entity* m_PlayerEntity;
		TextInfoType m_Type;
		TextInfoComponent(PlayerTag PlayerTag, TextInfoType Type, Engine::Entity* Player ) : m_PlayerTag(PlayerTag), m_Type(Type),m_PlayerEntity(Player) {}
	};
	

	struct JumpingComponent : public Engine::Component
	{
		vec2 m_OriginalModelSize;
		bool m_InTheAir{ false };
		float m_AirbornTimeLeft{ -1.f };
		float m_JumpTimeLength;
		float m_JumpTimeCooldownLeft;
		float m_JumpTimeCooldownLength;
	};
	
}