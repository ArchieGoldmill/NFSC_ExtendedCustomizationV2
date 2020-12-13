#pragma once

namespace DBPart
{
	enum _DBPart :int
	{
		Invalid = 0,
		Body = 0x17,

		LeftHeadlight = 0x20,
		LeftHeadlightGlass = 0x21,
		RightHeadlight = 0x2A,
		RightHeadlightGlass = 0x2B,

		LeftBrakelight = 0x1E,
		LeftBrakelightGlass = 0x1F,
		RightBrakelight = 0x28,
		RightBrakelightGlass = 0x29,

		LeftMirror = 0x22,
		RightMirror = 0x2C,

		Driver = 0x2D,
		SteeringWheel = 0x2E,

		Attachment0 = 0x38,
		Attachment12 = 0x44,
		Attachment13 = 0x45,
		Attachment14 = 0x46,
		Attachment15 = 0x47,

		LicensePlate = 0x56,
		FrontDecal = 0x64,
		RearDecal = 0x65,

		Spoiler = 0x30,
		RoofScoop = 0x4D,
		Hood = 0x4E,

		Roof = 0x4C,
		Interior = 0x1D,

		Exhaust = 0x2F,

		GenericVinyls = 0x5E,
		VectorVinyls = 0x5F,

		FrontBrake = 0x18,
		FrontRotor = 0x19,
		RearBrake = 0x23,
		RearRotor = 0x24,

		FrontBadging = 0x49,
		RearBadging = 0x4B,

		BasePaint = 0x5A,

		FrontWheels = 0x54,

		FrontBumper = 0x48,
		RearBumper = 0x4A,
		Skirt = 0x4F,
	};
}