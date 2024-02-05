BIN = tuyad
CFILES = $(wildcard *.c)
LDFLAGS := -llink_core -lmiddleware_implementation -lplatform_port -lutils_modules -largp -lubus -lubox

all:$(BIN)

$(BIN): $(CFILES)
	$(CC) -o $@ $^ $(LDFLAGS)
clean:
	$(RM) -f $(BIN)
	
.PHONY: all clean