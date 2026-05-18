CXX = g++
CXXFLAGS = -std=c++17
TARGET = out

build:
	$(CXX) $(CXXFLAGS) main.cpp AngajatFactory.cpp ElectrocasnicFactory.cpp ServiceManager.cpp Cereri/*.cpp Electrocasnice/*.cpp Electrocasnice/Tipuri/*.cpp Muncitori/*.cpp Muncitori/Angajatii/*.cpp Muncitori/Salarii/*.cpp -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: build run clean

