#this is an implementation of the pi controller for the STM
    #last edited: 7/12/2022 by Loren Larrieu
using Pkg
Pkg.add("PiGPIO")
using PiGPIO #to interact with GPIO on raspberry pi
pi = Pi()#connect to pigpiod daemon on localhost

#set pins to be read and written to for controller
    #change these to match the pins that are actually used
ViPin = 18 #tunnelling current voltage
refPin = 23 #refrence voltage. could it be possible to hard set this in the code?
driverPin = 40 #output for piezo tube
set_mode(pi,ViPin,PiGPIO.INPUT)#Set as input
set_mode(pi,refPin,PiGPIO.INPUT)
set_mode(pi,driverPin,PiGPIO.OUTPUT)#set as output

ViSig = PiGPIO.read(pi,ViPin)
refSig = PiGPIO.read(pi,refPin)

#PI controller operations 
#constants 
    #these are determined via trial and error. set to 1 for initial testing
k_prop = 1#Proportionality constant
k_int = 1#integral constant
#initialize error
prevErrorsig = 0
while true #infinite loop! change this later
    #calculate error between signals
    errorSig = ViSig - refSig

    #calculate output signal
    Y = k_prop*errorSig + k_integral*(errorSig + prevErrorsig)#check this math w/ Dr. Cunha
        
    #write Y to Piezo-driver output
    PiGPIO.write(pi, driverPin, Y)#check if this works with analog values...

    prevErrorsig = errorSig #is this needed? 

end
