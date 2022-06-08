progsbase importdeps Java IDE
mkdir -p ../bin
javac -d ../bin -cp main:test:../imports TestLauncher.java 
java -cp ../bin TestLauncher
