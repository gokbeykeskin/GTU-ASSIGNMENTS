setlib = TSSet.h TSSet.cpp
utils = Testing/TestUtils.h Testing/TestUtils.cpp
integrationTests = Testing/IntegrationTests/
unitTests = Testing/UnitTests/
links = -std=c++11 -pthread
ALL:
	mkdir executables
	g++ -o executables/IntegrationTest1 $(integrationTests)TestMain1.cpp $(setlib) $(utils)  $(links)
	g++ -o executables/IntegrationTest2 $(integrationTests)TestMain2.cpp $(setlib) $(links)
	g++ -o executables/IntegrationTest3 $(integrationTests)TestMain3.cpp $(setlib) $(utils) $(links)
	g++ -o executables/IntegrationTest4 $(integrationTests)TestMain4.cpp $(setlib) $(links)
	g++ -o executables/UnitTest1 $(unitTests)Insertion.cpp $(setlib) -std=c++11
	g++ -o executables/UnitTest2 $(unitTests)Iteration.cpp $(setlib) -std=c++11
	g++ -o executables/UnitTest3 $(unitTests)Removal.cpp $(setlib) -std=c++11
	g++ -o executables/UnitTest4 $(unitTests)Search.cpp $(setlib) -std=c++11

