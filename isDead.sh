ps -ef | grep -w './movedetector' | grep -v grep; if [ $? -ne 0 ]; then cd /home/startup506/projects/Rustam/MoveWriter/ && ./movedetector; fi
