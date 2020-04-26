#pragma once

namespace Engine
{
	using ColorType = uint8_t;
	// TODO: Change to GLM color defined type
	struct Color
	{
		ColorType r;
		ColorType g;
		ColorType b;
	};

	struct ColorA
	{
		ColorType r;
		ColorType g;
		ColorType b;
		ColorType a;
	};
	

}

