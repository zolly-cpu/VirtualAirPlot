VirtualAirPlot
-----------------

Database is getting generated there i have the generate 1000.000.000 records for the distances. (Dimensions {1000,1000,1000})
Reached the 7.000.000 records for the moment ... .
This will be uploaded.
The arduino code will also be uploaded.
The basics to supply the coord of the match dimensions are now in the code.
In the DatabaseStructure.xml you can find in the last entry the configuration of the database for the virtual air plot.
______________________________________________________________________________________________________________________________
	<TABLE name="VIRTUAL_AIR_PLOT_COORD">
		<COLUMN name="PKEY" alias="Primary key" type="uuid" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_01_X_COORD" alias="BEACON_01_X_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_01_Y_COORD" alias="BEACON_01_Y_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_01_Z_COORD" alias="BEACON_01_Z_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_02_X_COORD" alias="BEACON_02_X_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_02_Y_COORD" alias="BEACON_02_Y_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_02_Z_COORD" alias="BEACON_02_Z_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_03_X_COORD" alias="BEACON_03_X_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_03_Y_COORD" alias="BEACON_03_Y_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_03_Z_COORD" alias="BEACON_03_Z_COORD" type="int" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_01_LENGHT" alias="BEACON_01_LENGHT" type="float8" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_02_LENGHT" alias="BEACON_02_LENGHT" type="float8" extra="" reference="no"></COLUMN>
		<COLUMN name="BEACON_03_LENGHT" alias="BEACON_03_LENGHT" type="float8" extra="" reference="no"></COLUMN>
	</TABLE>
______________________________________________________________________________________________________________________________
In the 2UVFramework you can find the LogServer and the Server to run in the background needed for the query's of the VirtualAirPlot.
