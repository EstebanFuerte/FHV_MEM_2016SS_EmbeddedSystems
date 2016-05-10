# FHV_MEM_EmbeddesSystemsInCommunicatingEnviroment
The motor of the lab board powers a conveyer belt. The conveyer belts can either be operated locally or tied together to set up a closed chain. The purpose is to transport something along the chain. 

In local service operation mode it shall be possible to start the profile in either direction. Before starting the
profile it shall be possible to modify the speed in the range of [100 … 2200] rpm in steps of 100 rpm.

In chain operation mode the conveyor shall only pass parcels from the left to the right. It shall wait for a
request from the conveyor belt to the left. If the request arrives and the conveyor belt is already performing a
movement, it shall send “WAIT”. Otherwise (if it is in idle state) it shall reply with “READY” and start a slow
movement (a = 100 rpm) for the payload passing time t pp = 1 s. Then it shall inform the conveyor belt to the
left, that it has received the payload with “RELEASE”. It then shall follow the specified profile to move the
payload to the next belt. Then it shall ask the conveyor belt to the right with “REQUEST” if it can pass the
payload. If it receives “WAIT” it shall stop. If it receives “READY” it shall start a slow movement (a = 100 rpm)
until it receives “RELEASE”. Then it shall stop, return to idle state and accept new requests from the
conveyor belt to the left.

The speed of the motor shall be controlled in a closed loop, the code for the control will be provided.
It shall be possible to operate the conveyer belt using either the local keyboard or a telnet connection.
Necessary information on the state of the system shall be shown on the board’s display.
The functions needed to access the board’s hardware are given (see “HwFunc.pdf” in the ILIAS).
It shall be taken into account that requirements changes could occur.
