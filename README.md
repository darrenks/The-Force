The two files you will need to get our code working:

helicontrol.ino - arduino code for accepting commands over serial and broadcasting them in inferred to a s107 helicopter (hook IR led to pin 4)

kinecthelicontrol.cs - csharp code to get kinect gestures and send them to the arduino. Take the skeleton example from the kinect sdk, and just replace the main .cs file with this. Assumes starwars.mp3 song is available for playing (put a song there or comment this part out).

Earlier debug/test files:

heliserialtest.cs - csharp test code to connect to the arduino and send basic flight command

kinectskeletaldebug.cs - csharp code based on Microsoft's skeletal example. Outputs coordinates of body parts we are interested in for gestures.
