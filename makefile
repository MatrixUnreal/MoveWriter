all:
	c++ -std=c++14 -o movedetector main.cpp MyCamera.cpp Video.cpp DBAccessor.cpp FaceDetector.cpp Detector.cpp \
	ThreadSafeDetector.cpp `pkg-config opencv  --cflags --libs` -pthread -lstdc++fs -I/usr/include/mysql \
	 -L/usr/lib/x86_64-linux-gnu -lmysqlclient ~/lib/darknet/darknet.so -DOPENCV=1