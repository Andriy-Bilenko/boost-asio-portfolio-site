


######################################
# target
######################################
TARGET = darknet_server

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
CPP_SOURCES =  \
darknet_http80_portfolio.cpp \

######################################
# compiler
######################################
CXX = g++
 
#######################################
# CFLAGS
#######################################
# C includes
CXX_INCLUDES =  \
-Ires \
-Iclearnet \

# compile gcc flags
CXXFLAGS += $(CXX_INCLUDES) $(OPT) -Wall -Werror -pedantic

ifeq ($(DEBUG), 1)
CXXFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# build the application
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) -o $@ -lssl -lcrypto -pthread

#added pthread for ubuntu
	
#create build directory
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)


#######################################
# run the application
#######################################
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)


# NOTE: maybe add here documentation with doxygen

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

