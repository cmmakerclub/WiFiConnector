from SCons.Script import DefaultEnvironment
import glob, shutil

env = DefaultEnvironment()
# uncomment line below to see environment variables
# print env.Dump()
# print ARGUMENTS

# copyfile("src/WiFiConnector.h", "tmp/WiFiConnector/WiFiConnector.h")
# copyfile("src/WiFiConnector.cpp", "tmp/WiFiConnector/WiFiConnector.cpp")

# for file in glob.iglob('src/*.*'):
#     shutil.copy2(file, "tmp/WiFiConnector")
