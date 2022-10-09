all: $(obj)
$(SRC)/%.o : $(SRC)/%.c $(INCLUDE)/%.h
	$(CC) -fPIC -c $< -o $@ $(flags) $(libs) 
