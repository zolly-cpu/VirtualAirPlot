#ifndef _SERVERAPPLICATION_INTERFACE
#define _SERVERAPPLICATION_INTERFACE

module UVServerAppServer {

    sequence <string> tyStringSequence;
    interface ServerAppServer
    {
      bool Ping ();
      void Shutdown ();
      bool insertIntoTableDatabase( string paTableName,
                                    tyStringSequence paColumns,
                                    tyStringSequence paValue,
                                    tyStringSequence paTypeValue,
                                    out string paReturnMessage);
      bool updateIntoTableDatabase( string paTableName,
                                    string paId,
                                    tyStringSequence paColumns,
                                    tyStringSequence paValue,
                                    tyStringSequence paTypeValue,
                                    out string paReturnMessage);
      bool deleteIntoTableDatabase( string paTableName,
                                    string paId,
                                    out string paReturnMessage);

      bool getFromTableDatabaseGeneral( string paTableName,
                                        string paStartValue,
                                        string paMaxValue,
                                        out tyStringSequence paReturnId,
                                        out tyStringSequence paReturnName,										
                                        out string paMessage);
      bool getFromTableDatbaseByProperty(   string paTableName,
                                            string paStartValue,
                                            string paMaxValue,
                                            tyStringSequence paProperties,
                                            tyStringSequence paValue,
                                            tyStringSequence paTypeValue,
                                            tyStringSequence paLogExp,
                                            out tyStringSequence paReturnId,
                                            out string paReturnMessage);
      bool getFromTableDatabaseById(    string paTableName,
                                        string paId,
                                        tyStringSequence paProperties,
                                        out tyStringSequence paReturnValue,
                                        out string paReturnMessage);
      bool getAllTablesFromDatabase(    out tyStringSequence paTables,
                                        out string paReturnMessage);

      bool getAllMethodsFromTable(      string paTable,
                                        out tyStringSequence paMethodName,
                                        out tyStringSequence paAlias,
                                        out tyStringSequence paSource,
                                        out tyStringSequence paSourceName,
                                        out tyStringSequence paReturnType,
                                        out string paReturnMessage);

      bool getAllPropertiesFromTable(   string paTable,
                                        out tyStringSequence paPropertyName,
                                        out tyStringSequence paAlias,
                                        out tyStringSequence paType,
                                        out tyStringSequence paExtra,
                                        out tyStringSequence paReference,
                                        out string paReturnMessage);
										
      bool giveWorkstationClientInfo(	string paWorkstationName,
                                        string paPercentageMemUsage,
                                        string paPercentageDiskUsage,
                                        string paPercentageCpuUsage,
                                        out string paReturnMessage);										

    };
};

#endif
