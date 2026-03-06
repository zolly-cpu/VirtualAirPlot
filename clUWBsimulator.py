import math
import socket
import time
#####################################
#Fill in the classname of the object#
#####################################
class clSimulatorUWB:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
        print ('clSimulatorUWB::__init__->start')
        self.ip = "192.168.1.231"
        self.port_01 = 1234
        self.port_02 = 1235
        self.port_03 = 1236

        try:
            print ('clSimulatorUWB::__init__->start')
        except:
            traceback.print_exc()
    ####################################
    #Destructor class (stay's this way)#
    ####################################
    def __del__(self):
        print ('clSimulatorUWB::__del__->start')

    ###########
    #Functions#
    ###########
    def doMovement(self):
        try:
            print('clSimulatorUWB::calculate_all')
            i = 0
            while(True):
                self.client_01 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.client_02 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.client_03 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.client_01.connect((self.ip,self.port_01))
                self.client_02.connect((self.ip,self.port_02))
                self.client_03.connect((self.ip,self.port_03))
                self.client_01.send(("<root><sensor_01 name='sensor01' lenght='" + str( 666 - i) + "'/><sensor_02 name='sensor02' lenght='" + str(764 - i) + "'/></root>").encode())
                self.client_02.send(("<root><sensor_01 name='sensor01' lenght='" + str( 1001 - i) + "'/><sensor_02 name='sensor02' lenght='" + str( 1068 - i) + "'/></root>").encode())
                self.client_03.send(("<root><sensor_01 name='sensor01' lenght='" + str( 1031 - i) + "'/><sensor_02 name='sensor02' lenght='" + str( 1098 - i) + "'/></root>").encode())
                self.client_01.close()
                self.client_02.close()
                self.client_03.close()
                time.sleep(0.125)
                i = i + 10
                if (i == 200):
                    i = 0

        except:
            traceback.print_exc()
            
##########################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():
    meSimulatorUWB = clSimulatorUWB()
    #Test the function manually
    meSimulatorUWB.doMovement()

if __name__ == "__main__":
    main()
