# ecallmsdperencoder
encode MSD (Minimal Set of Data) for eCall using PER (Packed Encoding Rule)

## example

```c
int cnt;
tECallMessage ECallMessage;
uint8_t msd[160];
uint8_t msdsize;

memset(&ECallMessage, 0, sizeof(ECallMessage)); 
memset(msd, 0, sizeof(msd));                    

ECallMessage.id = 1;
ECallMessage.msd.msdStructure.messageIdentifier = 1;
ECallMessage.msd.msdStructure.control.automaticActivation = true;
ECallMessage.msd.msdStructure.control.testCall = true;
ECallMessage.msd.msdStructure.control.positionCanBeTrusted = true;
ECallMessage.msd.msdStructure.control.vehicleType = PASSENGER_VEHICLE_CLASS_M1;
ECallMessage.msd.msdStructure.vehicleIdentificationNumber.isowmi = "AAA";
ECallMessage.msd.msdStructure.vehicleIdentificationNumber.isovds = "BBBBBB";
ECallMessage.msd.msdStructure.vehicleIdentificationNumber.isovisModelyear = "C";
ECallMessage.msd.msdStructure.vehicleIdentificationNumber.isovisSeqPlant = "DDDDDDD";
ECallMessage.msd.msdStructure.vehiclePropulsionStorageType.compressedNaturalGas = false;
ECallMessage.msd.msdStructure.vehiclePropulsionStorageType.dieselTankPresent = false;
ECallMessage.msd.msdStructure.vehiclePropulsionStorageType.electricEnergyStorage = false;
ECallMessage.msd.msdStructure.vehiclePropulsionStorageType.gasolineTankPresent = true;
ECallMessage.msd.msdStructure.vehiclePropulsionStorageType.hydrogenStorage = false;
ECallMessage.msd.msdStructure.vehiclePropulsionStorageType.liquidPropaneGas = false;
ECallMessage.msd.msdStructure.timestamp = 1506602713;
/*
  lat=47.497912 -> 47°29'52.5"N -> (47×3600)+(29×60)+52.5 = 170992.500 = 170992500
  lon=19.040235 -> 19°02'24.9"E -> (19×3600)+(2×60)+24.9 = 68544.900 = 68544900
 */
ECallMessage.msd.msdStructure.vehicleLocation.positionLatitude = 170992500;
ECallMessage.msd.msdStructure.vehicleLocation.positionLongitude = 68544900;
ECallMessage.msd.msdStructure.vehicleDirection = 45;
ECallMessage.msd.msdStructure.recentVehicleLocationN1Avlb = false;
ECallMessage.msd.msdStructure.recentVehicleLocationN1.latitudeDelta = 0;
ECallMessage.msd.msdStructure.recentVehicleLocationN1.longitudeDelta = 0;
ECallMessage.msd.msdStructure.recentVehicleLocationN2Avlb = false;
ECallMessage.msd.msdStructure.recentVehicleLocationN2.latitudeDelta = 0;
ECallMessage.msd.msdStructure.recentVehicleLocationN2.longitudeDelta = 0;
ECallMessage.msd.msdStructure.numberOfPassengersAvlb = true;
ECallMessage.msd.msdStructure.numberOfPassengers = 3;
ECallMessage.msd.optionalAdditionalDataAvlb = false;
ECallMessage.msd.optionalAdditionalData.oidLength = 0;
ECallMessage.msd.optionalAdditionalData.dataLength = 0;
ECallMessage.msd.optionalAdditionalData.oid = "";
ECallMessage.msd.optionalAdditionalData.data = "";

msdsize = encodeMSD(&ECallMessage, &msd[0]);

printf("MSD[%u]: ", msdsize);
for (cnt = 0; cnt < msdsize; cnt++)
  printf("%.2X", msd[cnt]);
printf("\n");
```
