from SCons.Script import DefaultEnvironment
from shutil import copyfile

env = DefaultEnvironment()
# uncomment line below to see environment variables
# print env.Dump()
# print ARGUMENTS

copyfile("/Users/Nat/Documents/Arduino/libraries/WiFiConnector/src/WiFiConnector.h",
 "/Users/Nat/Documents/Arduino/libraries/WiFiConnector/tmp/WiFiConnector/WiFiConnector.h")

copyfile("/Users/Nat/Documents/Arduino/libraries/WiFiConnector/src/WiFiConnector.cpp",
 "/Users/Nat/Documents/Arduino/libraries/WiFiConnector/tmp/WiFiConnector/WiFiConnector.cpp")