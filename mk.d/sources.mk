##########################
### Sources extraction ###
##########################

SUBDIRS=$(sort $(dir $(wildcard $(SRC_DIR)/*/)))
SOURCES=$(wildcard $(addsuffix *.cpp,$(SUBDIRS)))
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(TMP_DIR)/%.o,$(SOURCES))
DEPS=$(patsubst $(SRC_DIR)/%.cpp,$(TMP_DIR)/%.d,$(SOURCES))
