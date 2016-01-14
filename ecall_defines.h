#ifndef ECALL_DEFINES_H_
#define ECALL_DEFINES_H_
#endif

#ifndef ECALL_MSD_MAX_LENGTH
#define ECALL_MSD_MAX_LENGTH	(140)	/* maximum length of MSD message in bytes */
#endif

#ifndef ISO_WMI_SIZE
#define ISO_WMI_SIZE			(3)
#endif

#ifndef ISO_VDS_SIZE
#define ISO_VDS_SIZE			(6)
#endif

#ifndef ISO_VDS_MODEL_YEAR_SIZE
#define ISO_VDS_MODEL_YEAR_SIZE	(1)
#endif

#ifndef ISO_VDS_SEQ_PLANT_SIZE
#define ISO_VDS_SEQ_PLANT_SIZE	(7)
#endif

#ifndef VEHICLE_PROPULSION_STORAGE_TYPE_SIZE
#define VEHICLE_PROPULSION_STORAGE_TYPE_SIZE	(6)
#endif

#ifndef LOCATION_LAT_LON_OFFSET_COMPENSATE
#define LOCATION_LAT_LON_OFFSET_COMPENSATE	(2147483648LL)
#endif

#ifndef R_LOCATION_LAT_LON_OFFSET_COMPENSATE
#define R_LOCATION_LAT_LON_OFFSET_COMPENSATE	(512)
#endif

/* Printable SIZE(3) FROM ("A".."H"|"J".."N"|"P"|"R".."Z"|"0".."9"), altogether 33 possible characters, 6 bits are enough for the encoding, 2^5 < 33 < 2^6 */
/* vehicle identificatino number has altogether 17 characters with each character encoded into 6 bits, which needs 12 bytes and 6 bits, thus 13 bytes required */
#ifndef VIN_SIZE_IN_BYTE_PER
#define VIN_SIZE_IN_BYTE_PER	(13)
#endif

#include <stdint.h>

#define bool int
#define true 1
#define false 0

/* 0x015C0681D54970D65C3597CA0420C41464583ADE68AC52E9BB8413F149C07414FB414F6010180813E82181823230 */
/**
*   ----------------------------------
*	Message Structure of an eCall Message
*	----------------------------------
*	tECallMessage
*		|_	uint8_t id
*		|_	tMSD msd
*					|_	tMSDStructure msdStructure
*						|_	uint8_t messageIdentifier
*						|_	tControlType control
*								|_	bool automaticActiviation
*								|_	bool testCall
*								|_	bool positionCanBeTrusted
*								|_	tVehicleType vehicleType
*										|_	PASSENGER_VEHICLE_CLASS_M1
*										|_	BUSES_AND_COACHES_CLASS_M2
*										|_	BUSES_AND_COACHES_CLASS_M3
*										|_	LIGHT_COMMERCIAL_VEHICLES_CLASS_N1
*										|_	HEAVY_DUTY_VEHICLES_CLASS_N2
*										|_	HEAVY_DUTY_VEHICLES_CLASS_N3
*										|_	MOTORCYCLE_CLASS_L1E
*										|_	MOTORCYCLE_CLASS_L2E
*										|_	MOTORCYCLE_CLASS_L3E
*										|_	MOTORCYCLE_CLASS_L4E
*										|_	MOTORCYCLE_CLASS_L5E
*										|_	MOTORCYCLE_CLASS_L6E
*										|_	MOTORCYCLE_CLASS_L7E
*										|_  (Possible Extensions)
*						|_	tVehicleIdentificationNumber vehicleIdentificationNumber
*								|_	char isowmi
*								|_	char isovds
*								|_	char isovisModelyear
*								|_	char isovisSeqPlant
*						|_	tVehiclePropulsionStorageType vehiclePropulsionStorageType
*								|_	bool gasolineTankPresent
*								|_	bool dieselTankPresent
*								|_	bool compressedNaturalGas
*								|_	bool liquidPropaneGas
*								|_	bool electricEnergyStorage
*								|_	bool hydrogenStorage
*								|_  (Possible Extensions)
*						|_	uint32_t timestamp
*						|_	tVehicleLocation vehicleLocation
*								|_	int32_t positionLatitude;
*								|_	int32_t positionLongitude;
*						|_	uint8_t vehicleDirection
*						|_	tVehicleLocationDelta recentVehicleLocationN1
*								|_	int16_t latitudeDelta
*								|_	int16_t longitudeDelta
*						|_	tVehicleLocationDelta recentVehicleLocationN2
*								|_	int16_t latitudeDelta
*								|_	int16_t longitudeDelta
*						|_	uint8_t numberOfPassengers
*						|_  (Possible Extensions)
*				|_	tOptionalAdditionalData
*/

/**
*	Definition of the vehicle type reporting the incident.
*	Vehicle definitions class M, N according directive 2007/46/EC, class L according directive 2002/24/EC
*	Extendable in future versions for new vehicle types
*/
typedef enum{
	PASSENGER_VEHICLE_CLASS_M1 			= 1,
	BUSES_AND_COACHES_CLASS_M2 			= 2,
	BUSES_AND_COACHES_CLASS_M3 			= 3,
	LIGHT_COMMERCIAL_VEHICLES_CLASS_N1	 	= 4,
	HEAVY_DUTY_VEHICLES_CLASS_N2 			= 5,
	HEAVY_DUTY_VEHICLES_CLASS_N3 			= 6,
	MOTORCYCLE_CLASS_L1E 				= 7,
	MOTORCYCLE_CLASS_L2E 				= 8,
	MOTORCYCLE_CLASS_L3E 				= 9,
	MOTORCYCLE_CLASS_L4E 				= 10,
	MOTORCYCLE_CLASS_L5E 				= 11,
	MOTORCYCLE_CLASS_L6E 				= 12,
	MOTORCYCLE_CLASS_L7E 				= 13
}tVehicleType;

typedef struct{
	bool automaticActivation;	/* true = Automatic activiation, false = Manual activation */
	bool testCall;				/* true = Test call, false = Emergency */
	bool positionCanBeTrusted;	/* true = Position can be trusted, false = low confidence in position - "Low confidence in position" if not within limits of +-150m with 95% confidence */
	tVehicleType vehicleType;
}tControlType;

/**
*	Vehicle Identification Number according ISO 3779
*/
typedef struct{
	char const * isowmi;						/* isowmi			: World Manufacturer Index,		PrintableString (SIZE(3)) FROM ("A".."H"|"J".."N"|"P"|"R".."Z"|"0".."9") */
	char const * isovds;						/* isovds			: Vehicle Type Descriptor,		PrintableString (SIZE(6)) FROM ("A".."H"|"J".."N"|"P"|"R".."Z"|"0".."9") */
	char const * isovisModelyear;	/* isovisModelyear	: Vehicle Identifier Sectino,	PrintableString (SIZE(1)) FROM ("A".."H"|"J".."N"|"P"|"R".."Z"|"0".."9") */
	char const * isovisSeqPlant;	/* isovisSeqPlant	: Vehicle Identifier Sectino,	PrintableString (SIZE(7)) FROM ("A".."H"|"J".."N"|"P"|"R".."Z"|"0".."9") */
}tVehicleIdentificationNumber;						/* According ISO 3779 */

/**
*	false = indicates a type of storage not present
*	true  = indicates a type of storage which is present
*	All bits shall be set to zero to indicate an unknown or other type of energy storage
*	Extendiable
*/
typedef struct{
	bool gasolineTankPresent; 	/* DEFAULT FALSE */
	bool dieselTankPresent; 	/* DEFAULT FALSE */
	bool compressedNaturalGas; 	/* DEFAULT FALSE */
	bool liquidPropaneGas; 		/* DEFAULT FALSE */
	bool electricEnergyStorage; 	/* DEFAULT FALSE */
	bool hydrogenStorage; 		/* DEFAULT FALSE */
}tVehiclePropulsionStorageType;

/**
*	The corrent location of the vehicle
*/
typedef struct{
	int32_t positionLatitude;	/* INTEGER(-2147483648..2147483647), value 0x7FFFFFFF = 2147483647 shall be used in case of invalid or unknown */
	int32_t positionLongitude;	/* INTEGER(-2147483648..2147483647), value 0x7FFFFFFF = 2147483647 shall be used in case of invalid or unknown */
}tVehicleLocation;

/**
*	Description of a recent vehicle location before the incident
*/
typedef struct{
	int16_t latitudeDelta;	/* INTEGER(-512..511), + for North and - for South */
	int16_t longitudeDelta; /* INTEGER(-512..511), + for East and - for West */
}tVehicleLocationDelta;

/**
*	The main uplink msd message from the vehicle (excluding ID)
*/
typedef struct{
	uint8_t messageIdentifier;					/* INTEGER(0 .. 255) */
	tControlType control;
	tVehicleIdentificationNumber vehicleIdentificationNumber;
	tVehiclePropulsionStorageType vehiclePropulsionStorageType;
	uint32_t timestamp;						/* INTEGER(0 .. 4294967295), 0 indicates failure value */
	tVehicleLocation vehicleLocation;
	uint8_t vehicleDirection;					/* INTEGER(0 .. 255), 2°-degrees steps from magnetic north, 0- 358, clockwise, 0xFF for invalid or unknown, only 0 - 179 are valid */
	bool recentVehicleLocationN1Avlb;
	tVehicleLocationDelta recentVehicleLocationN1;			/*  optional */
	bool recentVehicleLocationN2Avlb;
	tVehicleLocationDelta recentVehicleLocationN2;			/*  optional */
	bool numberOfPassengersAvlb;
	uint8_t numberOfPassengers;					/* INTEGER(0 .. 255), optional */
}tMSDStructure;

typedef struct{
	uint8_t oidLength;
	uint8_t dataLength;
	uint8_t *oid;
	uint8_t *data;
}tAdditionalData;

typedef struct{
	tMSDStructure msdStructure;
	bool optionalAdditionalDataAvlb;
	tAdditionalData optionalAdditionalData; /* optional */
}tMSD;

typedef struct{
	uint8_t id; /* INTEGER(0 .. 255), MSD format version set to 1 to discriminate from later MSD formats */
	tMSD msd;
}tECallMessage;
