#include "ecall_defines.h"

/**
* static global variables
*/
uint8_t byteIndex = 0;
/* starting position of bit in a byte is 8, the leftmost bit of a byte */
uint8_t bitIndex = 8;
uint8_t position;

/* convert character to actual encode value */
uint8_t getEncodeValue( char character);

void updateIndexes( uint8_t consumedBits );

/* serialize MSD bits into MSD buffer */
void appendBits( uint8_t *msdAsByte, uint32_t src, uint8_t length );

/**
* serialize eCall MSD data into byte array and return the consumed size in byte
*/
uint8_t encodeMSD( tECallMessage *eCallMessage, uint8_t *msdAsByte )
{
	uint8_t yes = 1;
	uint8_t no = 0;

	/* eCallMessage */
	/* id */
	appendBits( msdAsByte, eCallMessage->id, 8 );

	/* msd */
	/* it has possible extensions and 1 OPTIONAL data field : optionalAdditionalData */
	tMSD msd = eCallMessage->msd;

	/* no extension */
	appendBits( msdAsByte, no, 1 );
	
	/* set bit for OPTIONAL field, 1 bit */
	appendBits( msdAsByte, msd.optionalAdditionalDataAvlb, 1 );

	/* msdStructure */
	/* 
		it has possible extensions and 3 OPTIONAL data fields :
		recentVehicleLocationN1,
		recentVehicleLocationN2,
		numberOfPassengers
	*/
	tMSDStructure msdStructure = msd.msdStructure;

	/* no extension */
	appendBits( msdAsByte, no, 1 );

	/* set bits for OPTIONAL fields, 1 bits for each, 3 in total */
	appendBits( msdAsByte, msdStructure.recentVehicleLocationN1Avlb, 1 );
	appendBits( msdAsByte, msdStructure.recentVehicleLocationN2Avlb, 1 );
	appendBits( msdAsByte, msdStructure.numberOfPassengersAvlb, 1 );

	/* messageIdentifier */
	appendBits( msdAsByte, msdStructure.messageIdentifier, 8 );

	/* control */
	tControlType control = msdStructure.control;
	/* automaticActivation */
	appendBits( msdAsByte, control.automaticActivation, 1 );

	/* testCall */
	appendBits( msdAsByte, control.testCall, 1 );

	/* positionCanBeTrusted */
	appendBits( msdAsByte, control.positionCanBeTrusted, 1 );

	/* vehicleType, has possible extensions */
	/* no extension */
	appendBits( msdAsByte, no, 1 );

	/* it has 13 possible values, requires thus at least 4 bits for the encoding,  2^3 < 13 < 2^4 */
	appendBits( msdAsByte, control.vehicleType - 1, 4 );

	/* vehicleIdentificationNumber */
	/* it has 33 possible values for each character, requires thus at least 6 bits for the encoding, 2^5 < 33 < 2^6 */
	tVehicleIdentificationNumber vehicleIdentificationNumber = msdStructure.vehicleIdentificationNumber;

	int i;

	/* isowmi */
	/* 3 characters with 6 bits for each */
	for ( i = 0 ; i < ISO_WMI_SIZE ; i++ )
	{
		appendBits( msdAsByte, getEncodeValue( vehicleIdentificationNumber.isowmi[i] ), 6 );
	}

	/* isovds */
	/* 6 characters with 6 bits for each */
	for ( i = 0 ; i < ISO_VDS_SIZE ; i++ )
	{
		appendBits( msdAsByte, getEncodeValue( vehicleIdentificationNumber.isovds[i] ), 6 );
	}

	/* isovisModelyear */
	/* 1 character with 6 bits */
	for ( i = 0 ; i < ISO_VDS_MODEL_YEAR_SIZE ; i++ )
	{
		appendBits( msdAsByte, getEncodeValue( vehicleIdentificationNumber.isovisModelyear[i] ), 6 );
	}

	/* isovisSeqPlant */
	/* 7 characters with 6 bits for each */
	for ( i = 0 ; i < ISO_VDS_SEQ_PLANT_SIZE ; i++ )
	{
		appendBits( msdAsByte, getEncodeValue( vehicleIdentificationNumber.isovisSeqPlant[i] ), 6 );
	}

	/* vehiclePropulsionStorageType, has possible extensions */
	tVehiclePropulsionStorageType vehiclePropulsionStorageType = msdStructure.vehiclePropulsionStorageType;
	/* no extension */
	appendBits( msdAsByte, no, 1 );

	/* set presence of propulsion storage type, 1 bit for each */
	appendBits( msdAsByte, vehiclePropulsionStorageType.gasolineTankPresent, 1 );
	appendBits( msdAsByte, vehiclePropulsionStorageType.dieselTankPresent, 1 );
	appendBits( msdAsByte, vehiclePropulsionStorageType.compressedNaturalGas, 1 );
	appendBits( msdAsByte, vehiclePropulsionStorageType.liquidPropaneGas, 1 );
	appendBits( msdAsByte, vehiclePropulsionStorageType.electricEnergyStorage, 1 );
	appendBits( msdAsByte, vehiclePropulsionStorageType.hydrogenStorage, 1 );

	/* set value for each NON-default propulsion storage type, 1 bit for each */
	if ( vehiclePropulsionStorageType.gasolineTankPresent )
	{
		appendBits( msdAsByte, yes, 1 );
	}

	if ( vehiclePropulsionStorageType.dieselTankPresent )
	{
		appendBits( msdAsByte, yes, 1 );
	}

	if ( vehiclePropulsionStorageType.compressedNaturalGas )
	{
		appendBits( msdAsByte, yes, 1 );
	}

	if ( vehiclePropulsionStorageType.liquidPropaneGas )
	{
		appendBits( msdAsByte, yes, 1 );
	}

	if ( vehiclePropulsionStorageType.electricEnergyStorage )
	{
		appendBits( msdAsByte, yes, 1 );
	}

	if ( vehiclePropulsionStorageType.hydrogenStorage )
	{
		appendBits( msdAsByte, yes, 1 );
	}

	/* timestamp */
	/* it has 4294967296 possible values, requires thus at least 32 bits for the encoding, 2^31 < 4294967296 <= 2^32 */
	appendBits( msdAsByte, msdStructure.timestamp, 32 );

	/* vehicleLocation */
	/* positionLatitude */
	appendBits( msdAsByte, msdStructure.vehicleLocation.positionLatitude + LOCATION_LAT_LON_OFFSET_COMPENSATE, 32 );
	/* positionLongitude */
	appendBits( msdAsByte, msdStructure.vehicleLocation.positionLongitude + LOCATION_LAT_LON_OFFSET_COMPENSATE, 32 );

	/* vehicleDirection */
	/* vehicle direction has 256 possible values, requires thus 8 bits for the encoding, 2^7 < 256 <= 2^8 */
	appendBits( msdAsByte, msdStructure.vehicleDirection, 8 );

	/* recentVehicleLocationN1 */
	if ( msdStructure.recentVehicleLocationN1Avlb )
	{
		tVehicleLocationDelta recentVehicleLocationN1 = msdStructure.recentVehicleLocationN1;
		/* latitudeDelta */
		/* it has 1024 possible values, require thus at least 10 bits for the encoding, 2^9 < 1024 <= 2^10 */
		appendBits( msdAsByte, msdStructure.recentVehicleLocationN1.latitudeDelta + R_LOCATION_LAT_LON_OFFSET_COMPENSATE, 10 );
		/* longitudeDelta */
		/* it has 1024 possible values, require thus at least 10 bits for the encoding, 2^9 < 1024 <= 2^10 */
		appendBits( msdAsByte, msdStructure.recentVehicleLocationN1.longitudeDelta + R_LOCATION_LAT_LON_OFFSET_COMPENSATE, 10 );
	}

	/* recentVehicleLocationN2 */
	if ( msdStructure.recentVehicleLocationN2Avlb )
	{
		tVehicleLocationDelta recentVehicleLocationN2 = msdStructure.recentVehicleLocationN2;
		/* latitudeDelta */
		/* it has 1024 possible values, require thus at least 10 bits for the encoding, 2^9 < 1024 <= 2^10 */
		appendBits( msdAsByte, msdStructure.recentVehicleLocationN1.latitudeDelta + R_LOCATION_LAT_LON_OFFSET_COMPENSATE, 10 );
		/* longitudeDelta */
		/* it has 1024 possible values, require thus at least 10 bits for the encoding, 2^9 < 1024 <= 2^10 */
		appendBits( msdAsByte, msdStructure.recentVehicleLocationN2.longitudeDelta + R_LOCATION_LAT_LON_OFFSET_COMPENSATE, 10 );
	}

	/* numberOfPassengers */
	if ( msdStructure.numberOfPassengersAvlb )
	{
		/* it has 256 possible values, requires thus at least 8 bits for the encoding, 2^7 < 256 < 2^8 */
		appendBits( msdAsByte, msdStructure.numberOfPassengers, 8 );
	}

	/* optionalAdditionalData*/
	if ( msd.optionalAdditionalDataAvlb )
	{
		tAdditionalData optionalAdditionalData = msd.optionalAdditionalData;

		uint8_t oidLength = optionalAdditionalData.oidLength;
		uint8_t length = oidLength;
		for ( i = 0 ; i < length ; i++ )
		{
			if ( optionalAdditionalData.oid[i] > 127 )
			{
				/* each oid field which is greater than 127 needs to be encoded in 2 bytes */
				oidLength++;
			}
		}
		/* oid */
		/* set length of oid */
		appendBits( msdAsByte, oidLength, 8 );

		/* set content of oid */
		for ( i = 0 ; i < length ; i++ )
		{
			if ( optionalAdditionalData.oid[i] > 127 )
			{
				/* each oid field which is greater than 127 needs to be encoded in 2 bytes */
				/* 1st byte */
				appendBits( msdAsByte, ( ( optionalAdditionalData.oid[i] & 0x80 ) >> 7 /* get the 8th bit of this value and make it the 1st bit of 1st byte */) | 0x80/* set the 8th bit of the 1st byte to 1 */, 8 );
				/* 2nd byte */
				appendBits( msdAsByte, optionalAdditionalData.oid[i] & 0x7F/* get the first 7 bits */, 8 );
			}
			else
			{
				appendBits( msdAsByte, optionalAdditionalData.oid[i], 8 );
			}
		}

		/* data */
		/* set length of data */
		appendBits( msdAsByte, optionalAdditionalData.dataLength, 8 );

		/* set content of data */
		for ( i = 0 ; i < optionalAdditionalData.dataLength ; i++ )
		{
			appendBits( msdAsByte, optionalAdditionalData.data[i], 8 );
		}
	}

	return byteIndex + 1;
}

void appendBits( uint8_t *msdAsByte, uint32_t src, uint8_t length )
{
	position = bitIndex;
	if ( length <= position )
	{
		/* unconsumed bits left in the byte enough for bits to be appended */
		msdAsByte[byteIndex] |= src << ( position - length );
		updateIndexes ( length );
	}
	else
	{
		/* unconsumed bits left in the byte NOT enough for bits to be appended, thus more new byte will be required */
		msdAsByte[byteIndex] |= ( src >> length - position ) & 0xFF;
		updateIndexes ( bitIndex );
		appendBits( msdAsByte, src, length - position );
	}
}

void updateIndexes(uint8_t consumedBits )
{
	if ( !( consumedBits % 8 ) )
	{
		/* complete byte consumed, thus bitIndex unchanged and byteIndex increased by ( consumedBits / 8 ) */
		byteIndex += ( consumedBits / 8 );
	}
	else if ( bitIndex <= consumedBits )
	{
		/* extending to new byte */
		bitIndex = ( 8 - ( consumedBits - bitIndex ) % 8 );
		byteIndex += ( ( consumedBits - bitIndex ) / 8 ) + 1;
	}
	else
	{
		/* same byte still not fully consumed */
		bitIndex -= consumedBits;
	}
}

uint8_t getEncodeValue( char character) {
	uint8_t adjustNumber;

	if ( character >= 'A' && character <= 'H' )
	{
		adjustNumber = 55;
	}
	else if ( character >= 'J' && character <= 'N' )
	{
		adjustNumber = 56;
	}
	else if ( character == 'P' )
	{
		adjustNumber = 57;
	}
	else if ( character >= 'R' && character <= 'Z' )
	{
		adjustNumber = 58;
	}
	else if ( character >= '0' && character <= '9' )
	{
		adjustNumber = '0';
	}
	else
	{
		adjustNumber = character;
	}

	return character - adjustNumber;
}
